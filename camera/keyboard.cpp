#include "keyboard.h"
#include <QDebug>

Keyboard::Keyboard(QWidget *parent) : QWidget(parent), targetLineEdit(nullptr), isCaps(false)
{
    createLayout();

    // --- 键盘样式 (适配亮色主题) ---
    this->setStyleSheet(
        // 键盘背景：透明或极浅的灰色
        "QWidget { background-color: transparent; }"

        // 普通按键：白底，底部有阴影模拟立体感
        "QPushButton { "
        "  background-color: #FFFFFF; color: #555555; "
        "  border: 1px solid #DDDDDD; border-bottom: 4px solid #CCCCCC;" // 3D效果
        "  border-radius: 10px; font-size: 24px; min-width: 50px; min-height: 55px; "
        "  font-family: 'Arial'; font-weight: bold;"
        "}"

        // 按下效果：变成清新的蓝色，边框变平
        "QPushButton:pressed { "
        "  background-color: #00E5FF; color: white; "
        "  border-bottom: 0px solid #CCCCCC; margin-top: 4px;" // 物理下沉
        "}"

        // 功能键 (Caps)
        "QPushButton[objectName='funcKey'] { "
        "  background-color: #F0F0F0; color: #888; border-bottom: 4px solid #BBBBBB;"
        "}"
        "QPushButton[objectName='funcKey']:checked { " // Caps 锁定状态
        "  background-color: #FF9966; color: white; border: none; margin-top: 4px;"
        "}"

        // 删除键 (Del)
        "QPushButton[objectName='delKey'] { "
        "  background-color: #FFEEEE; color: #FF5E62; border-bottom: 4px solid #FFCCCC;"
        "}"
        "QPushButton[objectName='delKey']:pressed { "
        "  background-color: #FF5E62; color: white; margin-top: 4px; border: none;"
        "}"
    );
}

void Keyboard::setOutputWidget(QLineEdit *line)
{
    targetLineEdit = line;
}

void Keyboard::createLayout()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(5, 10, 5, 10);

    // 键位定义
    const QStringList row0 = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    const QStringList row1 = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"};
    const QStringList row2 = {"a", "s", "d", "f", "g", "h", "j", "k", "l"};
    const QStringList row3 = {"z", "x", "c", "v", "b", "n", "m"};

    int r = 0;
    int c = 0;

    // Row 0
    for(const QString &key : row0) {
        QPushButton *btn = new QPushButton(key, this);
        layout->addWidget(btn, r, c++);
        connect(btn, &QPushButton::clicked, [=](){ onKeyPressed(btn->text()); });
    }
    r++; c = 0;

    // Row 1
    for(const QString &key : row1) {
        QPushButton *btn = new QPushButton(key.toUpper(), this);
        m_letterBtns.append(btn);
        layout->addWidget(btn, r, c++);
        connect(btn, &QPushButton::clicked, [=](){ onKeyPressed(btn->text()); });
    }
    r++; c = 0;

    // Row 2 (Spacer for visual offset)
    QWidget *spacerLeft = new QWidget(this);
    layout->addWidget(spacerLeft, r, c++);
    for(const QString &key : row2) {
        QPushButton *btn = new QPushButton(key.toUpper(), this);
        m_letterBtns.append(btn);
        layout->addWidget(btn, r, c++);
        connect(btn, &QPushButton::clicked, [=](){ onKeyPressed(btn->text()); });
    }
    r++; c = 0;

    // Row 3
    QPushButton *capsBtn = new QPushButton("CAPS", this);
    capsBtn->setObjectName("funcKey");
    capsBtn->setCheckable(true);
    capsBtn->setFixedSize(80, 55);
    layout->addWidget(capsBtn, r, c++, 1, 2);
    connect(capsBtn, &QPushButton::clicked, this, &Keyboard::onCapsPressed);

    for(const QString &key : row3) {
        QPushButton *btn = new QPushButton(key.toUpper(), this);
        m_letterBtns.append(btn);
        layout->addWidget(btn, r, c++);
        connect(btn, &QPushButton::clicked, [=](){ onKeyPressed(btn->text()); });
    }

    QPushButton *backBtn = new QPushButton("DEL", this);
    backBtn->setObjectName("delKey");
    backBtn->setFixedSize(80, 55);
    layout->addWidget(backBtn, r, c++, 1, 2);
    connect(backBtn, &QPushButton::clicked, this, &Keyboard::onBackspacePressed);

    isCaps = false;
    updateKeys();
}

void Keyboard::onCapsPressed()
{
    isCaps = !isCaps;
    updateKeys();
}

void Keyboard::updateKeys()
{
    for(QPushButton *btn : m_letterBtns) {
        QString text = btn->text();
        btn->setText(isCaps ? text.toUpper() : text.toLower());
    }
}

void Keyboard::onKeyPressed(const QString &key)
{
    if (targetLineEdit) {
        targetLineEdit->insert(key);
    }
}

void Keyboard::onBackspacePressed()
{
    if (targetLineEdit) {
        targetLineEdit->backspace();
    }
}
