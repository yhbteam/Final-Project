#include "photobrowser.h"
#include <QDir>
#include <QFileInfoList>
#include <QPixmap>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

PhotoBrowser::PhotoBrowser(const QString &folderPath, QWidget *parent)
    : QWidget(parent), m_folderPath(folderPath)
{
    this->setWindowTitle("图片查看器");
    this->setFixedSize(800, 600);
    this->move(0, 0);
    this->setWindowFlags(Qt::Window);

    this->setStyleSheet("QWidget { background-color: #2b2b2b; color: white; }"
                        "QListWidget { background-color: #3d3d3d; border: 1px solid #555; color: white; font-size: 18px; }"
                        "QListWidget::item:selected { background-color: #00e5ff; color: black; }"
                        "QLabel { background-color: black; border: 1px solid #555; }"
                        "QPushButton { background-color: #ff4444; color: white; border-radius: 5px; padding: 10px; font-size: 18px; }"
                        "QPushButton:pressed { background-color: #cc0000; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    fileListWidget = new QListWidget(this);
    fileListWidget->setFixedWidth(250);

    closeBtn = new QPushButton("返回 / 关闭", this);
    connect(closeBtn, &QPushButton::clicked, this, &PhotoBrowser::close);

    leftLayout->addWidget(fileListWidget);
    leftLayout->addWidget(closeBtn);

    imageLabel = new QLabel("请选择图片", this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(imageLabel);

    loadImages();

    connect(fileListWidget, &QListWidget::currentItemChanged, this, [=](QListWidgetItem *current, QListWidgetItem *){
        if(!current) return;
        QString fileName = current->text();
        QString fullPath = QDir(m_folderPath).filePath(fileName);
        QPixmap pix(fullPath);
        if (!pix.isNull()) {
            imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            imageLabel->setText("无法加载图片");
        }
    });
}

void PhotoBrowser::loadImages()
{
    fileListWidget->clear();
    imageLabel->clear();

    QDir dir(m_folderPath);
    if (!dir.exists()) {
        imageLabel->setText("相册目录不存在！");
        return;
    }

    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Time);

    QFileInfoList fileList = dir.entryInfoList();

    if (fileList.isEmpty()) {
        imageLabel->setText("暂无照片");
        return;
    }

    for (const QFileInfo &fileInfo : fileList) {
        fileListWidget->addItem(fileInfo.fileName());
    }

    fileListWidget->setCurrentRow(0);
}
