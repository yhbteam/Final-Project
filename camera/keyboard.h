#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QList>

class Keyboard : public QWidget
{
    Q_OBJECT
public:
    explicit Keyboard(QWidget *parent = nullptr);
    void setOutputWidget(QLineEdit *line);

private slots:
    void onKeyPressed(const QString &key);
    void onBackspacePressed();
    void onCapsPressed(); // [新增] 大小写切换槽函数

private:
    QLineEdit *targetLineEdit;
    bool isCaps; // [新增] 当前是否大写
    QList<QPushButton*> m_letterBtns; // [新增] 存储所有字母键，方便改字

    void createLayout();
    void updateKeys(); // [新增] 刷新按键显示
};

#endif // KEYBOARD_H
