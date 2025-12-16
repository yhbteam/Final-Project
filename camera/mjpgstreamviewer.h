#ifndef MJPGSTREAMVIEWER_H
#define MJPGSTREAMVIEWER_H

#include <QWidget>
#include <QtNetwork>
#include <QLabel>
#include <QTimer>
#include <QImage>

class MjpgStreamViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MjpgStreamViewer(QWidget *parent = nullptr);

    // [关键] 编译器报错就是因为找不到这一行，请确保它存在！
    QImage getCurrentImage() const;

private slots:
    void onReadyRead();
    void onFinished();
    void onConnectTimeout();

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    QLabel *imageLabel;
    QByteArray buffer;
    QString boundary;
    bool headersParsed = false;
    QTimer *connectTimer;
    void startStream(const QUrl &url);
    void parseHeaders(const QByteArray &data);
    void processFrame();
};

#endif // MJPGSTREAMVIEWER_H
