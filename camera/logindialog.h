#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget> // 注意：这里用 QWidget 而不是 QDialog 以避免全屏模式下的某些 Bug
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "keyboard.h"

class LoginDialog : public QWidget
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void loginSuccess(); // 登录成功信号

private slots:
    void onLoginClicked();
    void onFocusChanged(QWidget *old, QWidget *now); // 监控焦点变化

private:
    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QPushButton *loginBtn;
    QPushButton *exitBtn;
    QLabel *statusLabel; // 用于显示"密码错误"等信息
    Keyboard *keyboard;  // 嵌入式键盘

    void initDb();       // 初始化数据库
    bool checkUser(const QString &user, const QString &pass);
};

#endif // LOGINDIALOG_H
