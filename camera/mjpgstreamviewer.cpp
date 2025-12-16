#include "mjpgstreamviewer.h"
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>

// 请根据实际情况修改 IP 地址
#define STREAM_URL "http://127.0.0.1:8080/?action=stream"

MjpgStreamViewer::MjpgStreamViewer(QWidget *parent)
    : QWidget(parent)
    , networkManager(new QNetworkAccessManager(this))
    , reply(nullptr)
{
    imageLabel = new QLabel(this);
    imageLabel->setText("正在初始化视频组件...");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: black; color: white;");
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(imageLabel);

    connectTimer = new QTimer(this);
    connectTimer->setSingleShot(true);
    connect(connectTimer, &QTimer::timeout, this, &MjpgStreamViewer::onConnectTimeout);

    QTimer::singleShot(500, this, [=](){
        startStream(QUrl(STREAM_URL));
    });
}

// [关键] 必须要有这个函数的实现
QImage MjpgStreamViewer::getCurrentImage() const
{
    if (imageLabel && imageLabel->pixmap() && !imageLabel->pixmap()->isNull()) {
        return imageLabel->pixmap()->toImage();
    }
    return QImage();
}

void MjpgStreamViewer::startStream(const QUrl &url)
{
    if (reply) {
        reply->abort();
        reply->deleteLater();
    }

    imageLabel->setText("正在尝试连接视频流...");
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    connectTimer->start(3000);

    reply = networkManager->get(request);
    connect(reply, &QNetworkReply::readyRead, this, &MjpgStreamViewer::onReadyRead);
    connect(reply, &QNetworkReply::finished, this, &MjpgStreamViewer::onFinished);
}

void MjpgStreamViewer::onConnectTimeout()
{
    if (reply && reply->isRunning()) {
        qWarning() << "连接超时";
        reply->abort();
        imageLabel->setText("连接超时\n(请检查网络或摄像头)");
    }
}

void MjpgStreamViewer::onFinished()
{
    connectTimer->stop();
    if (reply && reply->error() != QNetworkReply::NoError) {
        if (reply->error() != QNetworkReply::OperationCanceledError) {
             imageLabel->setText("连接断开:\n" + reply->errorString());
        }
    }
    if (reply) {
        reply->deleteLater();
        reply = nullptr;
    }
    headersParsed = false;
    buffer.clear();
    boundary.clear();
}

void MjpgStreamViewer::parseHeaders(const QByteArray &data)
{
    int boundaryStart = data.toLower().indexOf("boundary=");
    if (boundaryStart != -1) {
        QByteArray boundaryData = data.mid(boundaryStart + 9).trimmed();
        boundary = QString::fromUtf8(boundaryData);
        if (boundary.startsWith('"') && boundary.endsWith('"')) {
            boundary = boundary.mid(1, boundary.length() - 2);
        }
        headersParsed = true;
    }
}

void MjpgStreamViewer::onReadyRead()
{
    if (!reply) return;
    if (connectTimer->isActive()) {
        connectTimer->stop();
        imageLabel->setText("");
    }

    if (!headersParsed) {
        parseHeaders(reply->rawHeader("Content-Type"));
        if (!headersParsed) return;
    }

    QByteArray newData = reply->readAll();
    if (newData.isEmpty()) return;
    buffer.append(newData);
    processFrame();
}

void MjpgStreamViewer::processFrame()
{
    if (boundary.isEmpty()) return;
    QByteArray boundary_marker = ("\r\n--" + boundary).toUtf8();
    const QByteArray jpeg_separator = "\r\n\r\n";

    if (buffer.startsWith("--" + boundary.toUtf8())) {
        buffer.remove(0, ("--" + boundary).length());
        int rn_pos = buffer.indexOf("\r\n");
        if (rn_pos != -1) buffer.remove(0, rn_pos + 2);
    }

    int nextBoundaryStart = buffer.indexOf(boundary_marker);
    while (nextBoundaryStart != -1) {
        QByteArray frameData = buffer.left(nextBoundaryStart);
        int jpegStart = frameData.indexOf(jpeg_separator);
        if (jpegStart != -1) {
            QByteArray jpegData = frameData.mid(jpegStart + 4);
            QPixmap pixmap;
            if (pixmap.loadFromData(jpegData, "JPG")) {
                imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
        buffer.remove(0, nextBoundaryStart + boundary_marker.length());
        nextBoundaryStart = buffer.indexOf(boundary_marker);
    }
}
