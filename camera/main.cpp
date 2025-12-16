#include "mjpgstreamviewer.h"
#include <QApplication>
#include "widget.h"
#include "logindialog.h"
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    qunsetenv("QT_IM_MODULE");
    QApplication a(argc, argv);

    // 加载样式表
    QFile file(":/qss/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    } else {
        qWarning() << "Failed to load stylesheet";
    }

    LoginDialog *login = new LoginDialog();
    Widget *w = nullptr;

    QObject::connect(login, &LoginDialog::loginSuccess, [&](){
        login->close();
        w = new Widget();
        w->setWindowTitle("CameraApp");
        // [修改] 主界面也使用全屏
        w->setWindowState(Qt::WindowFullScreen);
        w->show();
        login->deleteLater();
    });

    login->show();

    return a.exec();
}
