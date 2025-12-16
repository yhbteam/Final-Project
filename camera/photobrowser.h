#ifndef PHOTOBROWSER_H
#define PHOTOBROWSER_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class PhotoBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoBrowser(const QString &folderPath, QWidget *parent = nullptr);
    void loadImages();

private:
    QListWidget *fileListWidget;
    QLabel *imageLabel;
    QPushButton *closeBtn;
    QString m_folderPath;
};

#endif
