/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mjpgstreamviewer.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_dangqianshijian;
    QLabel *label_dangqianshijianvalue;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_4;
    MjpgStreamViewer *widget_camera;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButton_paizhao;
    QPushButton *pushButton_chakantupian;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox;
    QPushButton *pushButton_openserial;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_dianjizhuanxiang;
    QLabel *label_dianjizhuanxiangvalue;
    QLabel *label_dianjisudu;
    QLabel *label_dianjisuduvalue;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_hongwaizhuangtai;
    QLabel *label_hongwaizhuangtaivalue;
    QLabel *label_duojijiaodu;
    QLabel *label_duojijiaoduvalue;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_wendu;
    QLabel *label_wenduvalue;
    QLabel *label_shidu;
    QLabel *label_shiduvalue;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_fengmingqi;
    QLabel *label_fengmingqivalue;
    QLabel *label_led;
    QLabel *label_ledvalue;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_controlled;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *horizontalSlider_led;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_2;
    QSlider *horizontalSlider_servo;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_controlmotor;
    QPushButton *pushButton_motorforward;
    QPushButton *pushButton_motorback;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_3;
    QSlider *horizontalSlider_motor;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QTableView *tableView_caozuojilu;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 918);
        Widget->setMinimumSize(QSize(0, 250));
        Widget->setStyleSheet(QString::fromUtf8("font: 20pt \"Ubuntu\";"));
        verticalLayout_5 = new QVBoxLayout(Widget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_dangqianshijian = new QLabel(Widget);
        label_dangqianshijian->setObjectName(QString::fromUtf8("label_dangqianshijian"));

        horizontalLayout_11->addWidget(label_dangqianshijian);

        label_dangqianshijianvalue = new QLabel(Widget);
        label_dangqianshijianvalue->setObjectName(QString::fromUtf8("label_dangqianshijianvalue"));

        horizontalLayout_11->addWidget(label_dangqianshijianvalue);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_2);


        verticalLayout_5->addLayout(horizontalLayout_11);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widget_camera = new MjpgStreamViewer(Widget);
        widget_camera->setObjectName(QString::fromUtf8("widget_camera"));
        widget_camera->setMinimumSize(QSize(0, 270));
        widget_camera->setMaximumSize(QSize(16777215, 300));

        verticalLayout_4->addWidget(widget_camera);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        pushButton_paizhao = new QPushButton(Widget);
        pushButton_paizhao->setObjectName(QString::fromUtf8("pushButton_paizhao"));

        horizontalLayout_7->addWidget(pushButton_paizhao);

        pushButton_chakantupian = new QPushButton(Widget);
        pushButton_chakantupian->setObjectName(QString::fromUtf8("pushButton_chakantupian"));

        horizontalLayout_7->addWidget(pushButton_chakantupian);


        verticalLayout_4->addLayout(horizontalLayout_7);


        verticalLayout_5->addLayout(verticalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboBox = new QComboBox(Widget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_4->addWidget(comboBox);

        pushButton_openserial = new QPushButton(Widget);
        pushButton_openserial->setObjectName(QString::fromUtf8("pushButton_openserial"));

        horizontalLayout_4->addWidget(pushButton_openserial);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_dianjizhuanxiang = new QLabel(Widget);
        label_dianjizhuanxiang->setObjectName(QString::fromUtf8("label_dianjizhuanxiang"));

        horizontalLayout_2->addWidget(label_dianjizhuanxiang);

        label_dianjizhuanxiangvalue = new QLabel(Widget);
        label_dianjizhuanxiangvalue->setObjectName(QString::fromUtf8("label_dianjizhuanxiangvalue"));

        horizontalLayout_2->addWidget(label_dianjizhuanxiangvalue);

        label_dianjisudu = new QLabel(Widget);
        label_dianjisudu->setObjectName(QString::fromUtf8("label_dianjisudu"));

        horizontalLayout_2->addWidget(label_dianjisudu);

        label_dianjisuduvalue = new QLabel(Widget);
        label_dianjisuduvalue->setObjectName(QString::fromUtf8("label_dianjisuduvalue"));

        horizontalLayout_2->addWidget(label_dianjisuduvalue);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_hongwaizhuangtai = new QLabel(Widget);
        label_hongwaizhuangtai->setObjectName(QString::fromUtf8("label_hongwaizhuangtai"));

        horizontalLayout_3->addWidget(label_hongwaizhuangtai);

        label_hongwaizhuangtaivalue = new QLabel(Widget);
        label_hongwaizhuangtaivalue->setObjectName(QString::fromUtf8("label_hongwaizhuangtaivalue"));

        horizontalLayout_3->addWidget(label_hongwaizhuangtaivalue);

        label_duojijiaodu = new QLabel(Widget);
        label_duojijiaodu->setObjectName(QString::fromUtf8("label_duojijiaodu"));

        horizontalLayout_3->addWidget(label_duojijiaodu);

        label_duojijiaoduvalue = new QLabel(Widget);
        label_duojijiaoduvalue->setObjectName(QString::fromUtf8("label_duojijiaoduvalue"));

        horizontalLayout_3->addWidget(label_duojijiaoduvalue);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_wendu = new QLabel(Widget);
        label_wendu->setObjectName(QString::fromUtf8("label_wendu"));

        horizontalLayout_5->addWidget(label_wendu);

        label_wenduvalue = new QLabel(Widget);
        label_wenduvalue->setObjectName(QString::fromUtf8("label_wenduvalue"));

        horizontalLayout_5->addWidget(label_wenduvalue);

        label_shidu = new QLabel(Widget);
        label_shidu->setObjectName(QString::fromUtf8("label_shidu"));

        horizontalLayout_5->addWidget(label_shidu);

        label_shiduvalue = new QLabel(Widget);
        label_shiduvalue->setObjectName(QString::fromUtf8("label_shiduvalue"));

        horizontalLayout_5->addWidget(label_shiduvalue);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_fengmingqi = new QLabel(Widget);
        label_fengmingqi->setObjectName(QString::fromUtf8("label_fengmingqi"));

        horizontalLayout_6->addWidget(label_fengmingqi);

        label_fengmingqivalue = new QLabel(Widget);
        label_fengmingqivalue->setObjectName(QString::fromUtf8("label_fengmingqivalue"));

        horizontalLayout_6->addWidget(label_fengmingqivalue);

        label_led = new QLabel(Widget);
        label_led->setObjectName(QString::fromUtf8("label_led"));

        horizontalLayout_6->addWidget(label_led);

        label_ledvalue = new QLabel(Widget);
        label_ledvalue->setObjectName(QString::fromUtf8("label_ledvalue"));

        horizontalLayout_6->addWidget(label_ledvalue);


        verticalLayout->addLayout(horizontalLayout_6);


        verticalLayout_5->addLayout(verticalLayout);

        widget_2 = new QWidget(Widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_12 = new QHBoxLayout(widget_2);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton_controlled = new QPushButton(widget_2);
        pushButton_controlled->setObjectName(QString::fromUtf8("pushButton_controlled"));

        verticalLayout_2->addWidget(pushButton_controlled);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSlider_led = new QSlider(widget_2);
        horizontalSlider_led->setObjectName(QString::fromUtf8("horizontalSlider_led"));
        horizontalSlider_led->setMaximum(100);
        horizontalSlider_led->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_led);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_8->addWidget(label_2);

        horizontalSlider_servo = new QSlider(widget_2);
        horizontalSlider_servo->setObjectName(QString::fromUtf8("horizontalSlider_servo"));
        horizontalSlider_servo->setMaximum(180);
        horizontalSlider_servo->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(horizontalSlider_servo);


        verticalLayout_2->addLayout(horizontalLayout_8);


        horizontalLayout_12->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton_controlmotor = new QPushButton(widget_2);
        pushButton_controlmotor->setObjectName(QString::fromUtf8("pushButton_controlmotor"));

        verticalLayout_3->addWidget(pushButton_controlmotor);

        pushButton_motorforward = new QPushButton(widget_2);
        pushButton_motorforward->setObjectName(QString::fromUtf8("pushButton_motorforward"));

        verticalLayout_3->addWidget(pushButton_motorforward);

        pushButton_motorback = new QPushButton(widget_2);
        pushButton_motorback->setObjectName(QString::fromUtf8("pushButton_motorback"));

        verticalLayout_3->addWidget(pushButton_motorback);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_9->addWidget(label_3);

        horizontalSlider_motor = new QSlider(widget_2);
        horizontalSlider_motor->setObjectName(QString::fromUtf8("horizontalSlider_motor"));
        horizontalSlider_motor->setMaximum(100);
        horizontalSlider_motor->setOrientation(Qt::Horizontal);

        horizontalLayout_9->addWidget(horizontalSlider_motor);


        verticalLayout_3->addLayout(horizontalLayout_9);


        horizontalLayout_12->addLayout(verticalLayout_3);


        verticalLayout_5->addWidget(widget_2);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_10->addWidget(label_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer);


        verticalLayout_5->addLayout(horizontalLayout_10);

        tableView_caozuojilu = new QTableView(Widget);
        tableView_caozuojilu->setObjectName(QString::fromUtf8("tableView_caozuojilu"));
        tableView_caozuojilu->setMinimumSize(QSize(0, 150));

        verticalLayout_5->addWidget(tableView_caozuojilu);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_dangqianshijian->setText(QCoreApplication::translate("Widget", "\345\275\223\345\211\215\346\227\266\351\227\264\357\274\232", nullptr));
        label_dangqianshijianvalue->setText(QCoreApplication::translate("Widget", "2025/12/15 12:17:10", nullptr));
        pushButton_paizhao->setText(QCoreApplication::translate("Widget", "\346\213\215\347\205\247", nullptr));
        pushButton_chakantupian->setText(QCoreApplication::translate("Widget", "\346\237\245\347\234\213\345\233\276\347\211\207", nullptr));
        pushButton_openserial->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        label_dianjizhuanxiang->setText(QCoreApplication::translate("Widget", "\347\224\265\346\234\272\350\275\254\345\220\221\357\274\232", nullptr));
        label_dianjizhuanxiangvalue->setText(QCoreApplication::translate("Widget", "\346\255\243\350\275\254", nullptr));
        label_dianjisudu->setText(QCoreApplication::translate("Widget", "\347\224\265\346\234\272\351\200\237\345\272\246\357\274\232", nullptr));
        label_dianjisuduvalue->setText(QCoreApplication::translate("Widget", "0", nullptr));
        label_hongwaizhuangtai->setText(QCoreApplication::translate("Widget", "\347\272\242\345\244\226\344\274\240\346\204\237\345\231\250\347\212\266\346\200\201\357\274\232", nullptr));
        label_hongwaizhuangtaivalue->setText(QCoreApplication::translate("Widget", "\351\201\207\351\230\273", nullptr));
        label_duojijiaodu->setText(QCoreApplication::translate("Widget", "\350\210\265\346\234\272\350\247\222\345\272\246\357\274\232", nullptr));
        label_duojijiaoduvalue->setText(QCoreApplication::translate("Widget", "0", nullptr));
        label_wendu->setText(QCoreApplication::translate("Widget", "\346\270\251\345\272\246\357\274\232", nullptr));
        label_wenduvalue->setText(QCoreApplication::translate("Widget", "50\302\260C", nullptr));
        label_shidu->setText(QCoreApplication::translate("Widget", "\346\271\277\345\272\246\357\274\232", nullptr));
        label_shiduvalue->setText(QCoreApplication::translate("Widget", "30%", nullptr));
        label_fengmingqi->setText(QCoreApplication::translate("Widget", "\350\234\202\351\270\243\345\231\250\347\212\266\346\200\201\357\274\232", nullptr));
        label_fengmingqivalue->setText(QCoreApplication::translate("Widget", "\345\274\200", nullptr));
        label_led->setText(QCoreApplication::translate("Widget", "led\344\272\256\345\272\246\357\274\232", nullptr));
        label_ledvalue->setText(QCoreApplication::translate("Widget", "20", nullptr));
        pushButton_controlled->setText(QCoreApplication::translate("Widget", "\345\274\200\347\201\257", nullptr));
        label->setText(QCoreApplication::translate("Widget", "led\344\272\256\345\272\246  \357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\350\210\265\346\234\272\350\247\222\345\272\246\357\274\232", nullptr));
        pushButton_controlmotor->setText(QCoreApplication::translate("Widget", "\345\274\200\347\224\265\346\234\272", nullptr));
        pushButton_motorforward->setText(QCoreApplication::translate("Widget", "\346\255\243\350\275\254", nullptr));
        pushButton_motorback->setText(QCoreApplication::translate("Widget", "\345\217\215\350\275\254", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\351\251\254\350\276\276\350\275\254\351\200\237\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\346\223\215\344\275\234\350\256\260\345\275\225\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
