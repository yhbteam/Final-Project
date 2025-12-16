#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QStyle>
#include <QCoreApplication>
#include <QSqlRecord>
#include "photobrowser.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setObjectName("Widget");
    this->setWindowState(Qt::WindowFullScreen);

    ui->widget_camera->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_photoBrowser = nullptr;
    flag = false;
    serialPort = new QSerialPort(this);
    m_logModel = nullptr;

    // 状态变量初始化
    m_temp = 0.0;
    m_hum = 0.0;
    m_infrared = 0;
    m_led = 0;
    m_motor_dir = -1;
    m_motor_speed = 0;
    m_buzzer = 0;
    m_servo_angle = 90;

    // 初始化数据库 (logs表)
    initDatabase();

    // -----------------------------------------------------------------
    // [核心修改] 独立的时间更新逻辑 (电子钟功能)
    // -----------------------------------------------------------------
    // 1. 创建定时器
    QTimer *sysTimer = new QTimer(this);

    // 2. 连接超时信号到 Lambda 表达式，更新标签为当前系统时间
    connect(sysTimer, &QTimer::timeout, this, [=](){
        if (ui->label_dangqianshijianvalue) {
            ui->label_dangqianshijianvalue->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        }
    });

    // 3. 启动定时器，每 1000 毫秒 (1秒) 触发一次
    sysTimer->start(1000);

    // 4. 立即刷新一次，避免启动时的短暂空白
    if (ui->label_dangqianshijianvalue) {
        ui->label_dangqianshijianvalue->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    }
    // -----------------------------------------------------------------


    // MQTT 初始化
    m_client = new QMqttClient(this);
    m_client->setHostname("118.196.46.169");
    m_client->setPort(1883);
    m_client->setUsername("zg");
    m_client->setPassword("12345678");

    connect(m_client, &QMqttClient::connected, this, &Widget::onMqttConnected);
    connect(m_client, &QMqttClient::messageReceived, this, &Widget::onMqttMessageReceived);
    connect(m_client, &QMqttClient::stateChanged, this, &Widget::onMqttStateChanged);

    m_client->connectToHost();

    // 控件初始化
    ui->pushButton_controlled->setCheckable(true);
    ui->pushButton_controlmotor->setCheckable(true);
    ui->pushButton_openserial->setCheckable(true);

    ui->label_wenduvalue->setProperty("type", "hot");
    ui->label_shiduvalue->setProperty("type", "wet");
    ui->label_hongwaizhuangtaivalue->setProperty("type", "safe");
    ui->label_fengmingqivalue->setText("关");

    ui->horizontalSlider_led->setRange(0, 100);
    ui->horizontalSlider_led->setValue(m_led);

    ui->horizontalSlider_servo->setRange(0, 180);
    ui->horizontalSlider_servo->setValue(m_servo_angle);

    ui->horizontalSlider_motor->setRange(0, 100);
    ui->horizontalSlider_motor->setValue(m_motor_speed);

    ui->pushButton_controlmotor->setText("开电机");
    ui->pushButton_controlled->setText("开灯");

    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &serialportinfo, list) {
        ui->comboBox->addItem(serialportinfo.portName());
    }
}

Widget::~Widget()
{
    if (serialPort->isOpen()) serialPort->close();
    if (m_client->state() == QMqttClient::Connected) m_client->disconnectFromHost();
    if (db.isOpen()) db.close();
    if (m_photoBrowser) delete m_photoBrowser;
    delete ui;
}

void Widget::initDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/system.db");
    }

    if (!db.open()) return;

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS logs (id INTEGER PRIMARY KEY AUTOINCREMENT, time TEXT, content TEXT)");

    if (ui->tableView_caozuojilu) {
        m_logModel = new QSqlTableModel(this, db);
        m_logModel->setTable("logs");
        m_logModel->setSort(0, Qt::DescendingOrder);
        m_logModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        m_logModel->select();
        m_logModel->setHeaderData(1, Qt::Horizontal, "时间");
        m_logModel->setHeaderData(2, Qt::Horizontal, "操作内容");

        ui->tableView_caozuojilu->setModel(m_logModel);
        ui->tableView_caozuojilu->setColumnHidden(0, true);
        ui->tableView_caozuojilu->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ui->tableView_caozuojilu->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->tableView_caozuojilu->setAlternatingRowColors(true);
        ui->tableView_caozuojilu->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

void Widget::addLog(const QString &content)
{
    if (!db.isOpen()) return;
    QSqlQuery query;
    query.prepare("INSERT INTO logs (time, content) VALUES (:time, :content)");
    query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":content", content);
    if (query.exec() && m_logModel) m_logModel->select();
}

void Widget::onMqttConnected()
{
    qDebug() << "MQTT Connected Success!";
    m_client->subscribe(QMqttTopicFilter("status_topic"));
    addLog("MQTT 服务器连接成功");
}

void Widget::onMqttStateChanged(QMqttClient::ClientState state)
{
    qDebug() << "MQTT State:" << state;
}

void Widget::onSendData()
{
    if (m_client->state() != QMqttClient::Connected) return;

    QJsonObject json;
    json["led"] = m_led;
    json["motor_dir"] = m_motor_dir;
    json["motor_speed"] = m_motor_speed;
    json["buzzer"] = m_buzzer;
    json["servo_angle"] = m_servo_angle;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    m_client->publish(QMqttTopicName("cmd_topic"), data);
}

void Widget::onMqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "Received Topic:" << topic.name() << "Data:" << message;

    // [修改] 此处不再更新 label_dangqianshijianvalue，完全交给定时器处理

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(message, &err);

    if (err.error == QJsonParseError::NoError && doc.isObject()) {
        QJsonObject obj = doc.object();

        if (obj.contains("temp")) m_temp = obj.value("temp").toDouble();
        if (obj.contains("hum")) m_hum = obj.value("hum").toDouble();
        if (obj.contains("infrared")) m_infrared = obj.value("infrared").toInt();

        if (obj.contains("buzzer")) {
            m_buzzer = obj.value("buzzer").toInt();
            ui->label_fengmingqivalue->setText(m_buzzer == 1 ? "开" : "关");
        }

        if (obj.contains("led")) {
            m_led = obj.value("led").toInt();
            ui->horizontalSlider_led->blockSignals(true);
            ui->horizontalSlider_led->setValue(m_led);
            ui->horizontalSlider_led->blockSignals(false);
            ui->label_ledvalue->setText(QString::number(m_led));
            bool isOn = (m_led > 0);
            ui->pushButton_controlled->setChecked(isOn);
            ui->pushButton_controlled->setText(isOn ? "关灯" : "开灯");
            ui->pushButton_controlled->style()->unpolish(ui->pushButton_controlled);
            ui->pushButton_controlled->style()->polish(ui->pushButton_controlled);
        }

        if (obj.contains("motor_speed")) {
            m_motor_speed = obj.value("motor_speed").toInt();
            if (obj.contains("motor_dir")) {
                m_motor_dir = obj.value("motor_dir").toInt();
            }
            if (m_motor_speed == 0) {
                m_motor_dir = -1;
                ui->label_dianjizhuanxiangvalue->setText("停止");
            } else if (m_motor_dir == 1) {
                ui->label_dianjizhuanxiangvalue->setText("正转");
            } else if (m_motor_dir == 0) {
                ui->label_dianjizhuanxiangvalue->setText("反转");
            }
            ui->horizontalSlider_motor->blockSignals(true);
            ui->horizontalSlider_motor->setValue(m_motor_speed);
            ui->horizontalSlider_motor->blockSignals(false);
            ui->label_dianjisuduvalue->setText(QString::number(m_motor_speed));
            bool isMotorOn = (m_motor_speed > 0);
            ui->pushButton_controlmotor->setChecked(isMotorOn);
            ui->pushButton_controlmotor->setText(isMotorOn ? "关电机" : "开电机");
            ui->pushButton_controlmotor->style()->unpolish(ui->pushButton_controlmotor);
            ui->pushButton_controlmotor->style()->polish(ui->pushButton_controlmotor);
        }

        if (obj.contains("servo_angle")) {
            m_servo_angle = obj.value("servo_angle").toInt();
            ui->horizontalSlider_servo->blockSignals(true);
            ui->horizontalSlider_servo->setValue(m_servo_angle);
            ui->horizontalSlider_servo->blockSignals(false);
            ui->label_duojijiaoduvalue->setText(QString::number(m_servo_angle));
        }

        ui->label_wenduvalue->setText(QString::number(m_temp, 'f', 1) + "°C");
        ui->label_shiduvalue->setText(QString::number(m_hum, 'f', 1) + "%");

        if (m_infrared == 1) {
            ui->label_hongwaizhuangtaivalue->setText("无遮挡");
            ui->label_hongwaizhuangtaivalue->setProperty("type", "safe");
        } else {
            ui->label_hongwaizhuangtaivalue->setText("遇阻");
            ui->label_hongwaizhuangtaivalue->setProperty("type", "warn");
        }
        ui->label_hongwaizhuangtaivalue->style()->unpolish(ui->label_hongwaizhuangtaivalue);
        ui->label_hongwaizhuangtaivalue->style()->polish(ui->label_hongwaizhuangtaivalue);
    } else {
        qDebug() << "JSON Parse Error:" << err.errorString();
    }
}

void Widget::on_pushButton_openserial_clicked()
{
    if (!flag) {
        serialPort->setPortName(ui->comboBox->currentText());
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);

        if (serialPort->open(QIODevice::ReadWrite)) {
            flag = true;
            ui->pushButton_openserial->setText("关闭串口");
            ui->pushButton_openserial->setChecked(true);
            addLog("打开串口成功: " + ui->comboBox->currentText());
        } else {
            ui->pushButton_openserial->setChecked(false);
            ui->pushButton_openserial->setText("打开失败");
            QString oldStyle = ui->pushButton_openserial->styleSheet();
            ui->pushButton_openserial->setStyleSheet("background-color: #FF5252; color: white; border: 2px solid #FFCDD2;");
            QTimer::singleShot(2000, this, [=](){
                ui->pushButton_openserial->setText("打开串口");
                ui->pushButton_openserial->setStyleSheet("");
            });
            addLog("打开串口失败");
        }
    } else {
        serialPort->close();
        flag = false;
        ui->pushButton_openserial->setText("打开串口");
        ui->pushButton_openserial->setChecked(false);
        addLog("关闭串口");
    }
}

void Widget::on_pushButton_paizhao_clicked()
{
    QImage img = ui->widget_camera->getCurrentImage();
    if (img.isNull()) return;
    QString saveDirStr = QCoreApplication::applicationDirPath() + "/photos";
    QDir saveDir(saveDirStr);
    if (!saveDir.exists()) saveDir.mkpath(".");
    QString currentTime = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString fileName = QString("IMG_%1.jpg").arg(currentTime);
    QString fullPath = saveDir.filePath(fileName);
    if (img.save(fullPath, "JPG")) {
        ui->pushButton_paizhao->setText("成功!");
        QTimer::singleShot(1000, this, [=](){ ui->pushButton_paizhao->setText("拍照"); });
        addLog("执行拍照: " + fileName);
    }
}

void Widget::on_pushButton_chakantupian_clicked()
{
    addLog("点击查看图片相册");
    QString saveDirStr = QCoreApplication::applicationDirPath() + "/photos";
    QDir dir(saveDirStr);
    if (!dir.exists()) dir.mkpath(".");
    if (!m_photoBrowser) {
        m_photoBrowser = new PhotoBrowser(saveDirStr, nullptr);
    } else {
        m_photoBrowser->loadImages();
    }
    m_photoBrowser->setWindowState(Qt::WindowFullScreen);
    m_photoBrowser->show();
    m_photoBrowser->raise();
    m_photoBrowser->activateWindow();
}

void Widget::on_horizontalSlider_led_valueChanged(int value) {
    m_led = value;
    ui->label_ledvalue->setText(QString::number(m_led));
    bool isOn = (value > 0);
    ui->pushButton_controlled->setChecked(isOn);
    ui->pushButton_controlled->setText(isOn ? "关灯" : "开灯");
    ui->pushButton_controlled->style()->unpolish(ui->pushButton_controlled);
    ui->pushButton_controlled->style()->polish(ui->pushButton_controlled);
    onSendData();
}

void Widget::on_pushButton_controlled_clicked() {
    QString action;
    if (ui->horizontalSlider_led->value() > 0) {
        ui->horizontalSlider_led->setValue(0);
        action = "点击操作: 关灯";
    } else {
        ui->horizontalSlider_led->setValue(50);
        action = "点击操作: 开灯 (亮度50)";
    }
    addLog(action);
}

void Widget::on_horizontalSlider_servo_valueChanged(int value) {
    m_servo_angle = value;
    ui->label_duojijiaoduvalue->setText(QString::number(m_servo_angle));
    onSendData();
}

void Widget::on_horizontalSlider_motor_valueChanged(int value) {
    m_motor_speed = value;
    if (m_motor_speed == 0) {
        m_motor_dir = -1;
        ui->label_dianjizhuanxiangvalue->setText("停止");
    }
    else if (m_motor_dir == -1) {
        m_motor_dir = 1;
        ui->label_dianjizhuanxiangvalue->setText("正转");
    }
    ui->label_dianjisuduvalue->setText(QString::number(m_motor_speed));
    bool isMotorOn = (m_motor_speed > 0);
    ui->pushButton_controlmotor->setChecked(isMotorOn);
    ui->pushButton_controlmotor->setText(isMotorOn ? "关电机" : "开电机");
    ui->pushButton_controlmotor->style()->unpolish(ui->pushButton_controlmotor);
    ui->pushButton_controlmotor->style()->polish(ui->pushButton_controlmotor);
    onSendData();
}

void Widget::on_pushButton_controlmotor_clicked() {
    QString action;
    if (ui->horizontalSlider_motor->value() > 0) {
        ui->horizontalSlider_motor->setValue(0);
        action = "点击操作: 关电机";
    } else {
        ui->horizontalSlider_motor->setValue(50);
        action = "点击操作: 开电机 (速度50)";
    }
    addLog(action);
}

void Widget::on_pushButton_motorforward_clicked() {
    m_motor_dir = 1;
    if (m_motor_speed == 0) {
        m_motor_speed = 50;
        ui->horizontalSlider_motor->setValue(50);
    } else {
        onSendData();
    }
    ui->label_dianjizhuanxiangvalue->setText("正转");
    addLog("点击操作: 电机正转");
}

void Widget::on_pushButton_motorback_clicked() {
    m_motor_dir = 0;
    if (m_motor_speed == 0) {
        m_motor_speed = 50;
        ui->horizontalSlider_motor->setValue(50);
    } else {
        onSendData();
    }
    ui->label_dianjizhuanxiangvalue->setText("反转");
    addLog("点击操作: 电机反转");
}
