#include "logindialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QApplication>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QGuiApplication>

LoginDialog::LoginDialog(QWidget *parent) : QWidget(parent)
{
    // 1. 设置窗口属性：无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // 使用全屏模式
    this->setWindowState(Qt::WindowFullScreen);
    this->move(0, 0);

    // 2. 设置界面样式
    this->setStyleSheet(
        "QWidget#LoginDialog { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FFFFFF, stop:1 #F0F4F8); "
        "}"
        "QLabel#titleLabel { "
        "   color: #546E7A; font-family: 'Microsoft YaHei'; font-size: 42px; font-weight: 900; margin-bottom: 20px;"
        "}"
        "QLineEdit { "
        "   background-color: #FFFFFF; color: #455A64; font-size: 26px; font-weight: bold;"
        "   border: 2px solid #CFD8DC; border-radius: 20px; padding: 15px; margin: 10px 0;"
        "}"
        "QLineEdit:focus { "
        "   border: 3px solid #29B6F6; background-color: #FAFFFF;"
        "}"
        "QPushButton#loginBtn { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4FC3F7, stop:1 #29B6F6); "
        "   color: white; font-size: 28px; font-weight: bold; border-radius: 35px; height: 70px; margin-top: 20px; border: none;"
        "}"
        "QPushButton#loginBtn:pressed { background-color: #0288D1; margin-top: 24px; }"
        "QPushButton#exitBtn { "
        "   background-color: #FFFFFF; color: #EF5350; font-size: 22px; font-weight: bold;"
        "   border: 3px solid #FFCDD2; border-radius: 35px; height: 70px; margin-top: 10px;"
        "}"
        "QLabel#statusLabel { color: #EF5350; font-size: 20px; font-weight: bold; margin-top: 10px; }"
    );

    this->setObjectName("LoginDialog");

    // 3. 界面布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(100, 80, 100, 100);

    QLabel *title = new QLabel("WELCOME SYSTEM", this);
    title->setObjectName("titleLabel");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    userEdit = new QLineEdit(this);
    userEdit->setPlaceholderText(" Username");
    mainLayout->addWidget(userEdit);

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText(" Password");
    passEdit->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(passEdit);

    statusLabel = new QLabel("", this);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFixedHeight(40);
    mainLayout->addWidget(statusLabel);

    loginBtn = new QPushButton("LOGIN", this);
    loginBtn->setObjectName("loginBtn");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 6);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(41, 182, 246, 100));
    loginBtn->setGraphicsEffect(shadow);

    exitBtn = new QPushButton("EXIT", this);
    exitBtn->setObjectName("exitBtn");

    mainLayout->addWidget(loginBtn);
    mainLayout->addWidget(exitBtn);

    mainLayout->addStretch(1);

    keyboard = new Keyboard(this);
    mainLayout->addWidget(keyboard);

    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(qApp, &QApplication::focusChanged, this, &LoginDialog::onFocusChanged);

    userEdit->setFocus();
    keyboard->setOutputWidget(userEdit);

    initDb();
}

LoginDialog::~LoginDialog() {}

void LoginDialog::onFocusChanged(QWidget *old, QWidget *now) {
    Q_UNUSED(old);
    if (now == userEdit) keyboard->setOutputWidget(userEdit);
    else if (now == passEdit) keyboard->setOutputWidget(passEdit);
}

void LoginDialog::initDb() {
    QSqlDatabase db;
    // [关键修改] 如果连接已存在则复用，否则新建，统一使用 system.db
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/system.db");
    }

    if (!db.open()) { statusLabel->setText("DB Error"); return; }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users (username VARCHAR(50) PRIMARY KEY, password VARCHAR(50))");
    query.exec("SELECT count(*) FROM users");
    if (query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
        query.bindValue(":u", "admin"); query.bindValue(":p", "123456"); query.exec();
    }
}

bool LoginDialog::checkUser(const QString &user, const QString &pass) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :u");
    query.bindValue(":u", user);
    if (query.exec() && query.next()) return query.value(0).toString() == pass;
    return false;
}

void LoginDialog::onLoginClicked() {
    QString user = userEdit->text().trimmed();
    QString pass = passEdit->text().trimmed();
    if (user.isEmpty() || pass.isEmpty()) { statusLabel->setText("Please enter input"); return; }
    if (checkUser(user, pass)) {
        statusLabel->setStyleSheet("color: #29B6F6; font-size: 20px; font-weight: bold;");
        statusLabel->setText("Login Success...");
        emit loginSuccess();
    } else {
        statusLabel->setStyleSheet("color: #FF7043; font-size: 20px; font-weight: bold;");
        statusLabel->setText("Error Password");
        passEdit->clear();
    }
}
