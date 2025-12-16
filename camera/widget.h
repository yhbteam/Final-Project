#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QMqttClient>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel> // [新增] SQL表格模型

class PhotoBrowser;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_openserial_clicked();
    void onSendData();
    void on_pushButton_paizhao_clicked();
    void on_pushButton_chakantupian_clicked();

    // --- 控制按钮 ---
    void on_pushButton_controlled_clicked();
    void on_pushButton_controlmotor_clicked();
    void on_pushButton_motorforward_clicked();
    void on_pushButton_motorback_clicked();

    // --- 滑动条槽函数 ---
    void on_horizontalSlider_led_valueChanged(int value);
    void on_horizontalSlider_servo_valueChanged(int value);
    void on_horizontalSlider_motor_valueChanged(int value);

    // MQTT 槽函数
    void onMqttConnected();
    void onMqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void onMqttStateChanged(QMqttClient::ClientState state);

private:
    // 初始化数据库和写日志函数
    void initDatabase();
    void addLog(const QString &content);

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    bool flag;
    QByteArray m_buffer;

    QMqttClient *m_client;
    PhotoBrowser *m_photoBrowser;

    // 数据库对象
    QSqlDatabase db;
    // [新增] 用于管理表格视图的模型
    QSqlTableModel *m_logModel;

    // 状态变量
    double m_temp;
    double m_hum;
    int m_infrared;
    int m_led;
    int m_motor_dir;
    int m_motor_speed;
    int m_buzzer;
    int m_servo_angle;
};
#endif // WIDGET_H
