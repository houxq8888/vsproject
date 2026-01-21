/*
**
**	版权:	
**
**	名称:	mykeyboard.h
**
**	作者:	侯 晓 琴
**
**	日期:	2024/11/25
**
**	描述:
**
**	功能:	虚拟键盘类
**
**	备注:
**
*/

#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QString>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>


class KBTimeWithHourEdit:public QWidget{
    Q_OBJECT
public:
    explicit KBTimeWithHourEdit(bool flag,QWidget *parent = nullptr);
    ~KBTimeWithHourEdit();
    void init(int MyHeight,int MyWidth);
    void setValue(const std::string& str);
signals:
    void MykeyboardSignal(const std::string& str);

private:
    void fillDayCombo(int month);

public:
    QLabel *m_yearLabel,*m_monthLabel,*m_dayLabel,*m_hourLabel,*m_minuteLabel,*m_secondLabel;
    QComboBox *m_yearEdit,*m_monthEdit,*m_dayEdit,*m_hourEdit,*m_minuteEdit,*m_secondEdit;
    QPushButton* m_okBtn,*m_cancelBtn;
    QGridLayout *gridLayout;
    QRadioButton* m_everRadioBtn;
    QString str;
    bool m_flag;

};

class KBTimeEdit:public QWidget{
    Q_OBJECT
public:
    explicit KBTimeEdit(bool flag,QWidget *parent = nullptr);
    ~KBTimeEdit();
    void init(int MyHeight,int MyWidth);
    void setValue(const std::string& str);
signals:
    void MykeyboardSignal(QString str);

private:
    void fillDayCombo(int month);

public:
    QLabel *m_yearLabel,*m_monthLabel,*m_dayLabel;
    QComboBox *m_yearEdit,*m_monthEdit,*m_dayEdit;
    QPushButton* m_okBtn,*m_cancelBtn;
    QGridLayout *gridLayout;
    QRadioButton* m_everRadioBtn;
    QString str;
    bool m_flag;

};
class KBUIntEdit : public QWidget
{
    Q_OBJECT
public:
    explicit KBUIntEdit(bool secretFlag, QWidget *parent = nullptr);
    ~KBUIntEdit();
    void init(int MyHeight,int MyWidth);
    void SetLimit(unsigned int LowerLimit=0, unsigned int UpperLimit=65530){m_dLowerLimit = LowerLimit; m_dUpperLimit = UpperLimit;}
    void setValue(unsigned int value);
signals:
    void MykeyboardSignal(QString str);
private slots:
private:
    void keyPressEvent(QKeyEvent *event);
    void saveEditInfo();
    void insertContent(QString text);
private:
    bool m_isSelectedAllFlag;
public:
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QPushButton *pushButton_6;
    QPushButton *pushButton_8;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_5;
    QPushButton *pushButton_esc;
    QPushButton *pushButton_0;
    QPushButton *pushButton_enter;
    QPushButton *pushButton_1;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_space;

    QString str;
protected:
    unsigned int m_dUpperLimit;
    unsigned int m_dLowerLimit;

private:
    bool m_secretFlag;

};

class KBIntEdit : public QWidget
{
    Q_OBJECT
public:
    explicit KBIntEdit(QWidget *parent = nullptr);
    ~KBIntEdit();
    void init(int MyHeight,int MyWidth);
    void setValue(int value);
    void SetLimit(int LowerLimit=-65500, int UpperLimit=65500){m_LowerLimit = LowerLimit; m_UpperLimit = UpperLimit;}
private:
    void keyPressEvent(QKeyEvent *event);
    void saveEditInfo();
    void insertContent(QString text);
private slots:
signals:
    void MykeyboardSignal(QString);
private:
    bool m_isSelectedAllFlag;
public:
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_space;
    QPushButton *pushButton_1;
    QPushButton *pushButton_enter;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_9;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_0;
    QPushButton *pushButton_7;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_esc;
    QPushButton *pushButton_add;
    QPushButton *pushButton_point;
    QPushButton *pushButton_8;
    QLineEdit *lineEdit;
    QString str;
protected:
    int m_UpperLimit;
    int m_LowerLimit;

};

class KBFLoatEdit : public QWidget
{
    Q_OBJECT
public:
    explicit KBFLoatEdit(QWidget *parent = nullptr);
    ~KBFLoatEdit();
    void init(int MyHeight,int MyWidth);
    void setValue(double value);
    void SetLimit(double dLowerLimit = -65500.00, double dUpperLimit=65500.00){m_dLowerLimit = dLowerLimit; m_dUpperLimit = dUpperLimit;}
private:
    void keyPressEvent(QKeyEvent *event);
    void saveEditInfo();
    void insertContent(QString text);
private slots:
signals:
    void MykeyboardSignal(QString);
private:
    bool m_isSelectedAllFlag;
public:
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_space;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_enter;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_0;
    QPushButton *pushButton_esc;
    QPushButton *pushButton_add;
    QPushButton *pushButton_point;
    QLineEdit *lineEdit;
    QString str;
protected:
    double m_dUpperLimit;
    double m_dLowerLimit;

};

class KBCharEdit : public QWidget
{
    Q_OBJECT
public:
    explicit KBCharEdit(QWidget *parent = nullptr);
    ~KBCharEdit();
    void init(int MyHeight,int MyWidth,int flag);
    void setValue(std::string str);
private:
    void keyPressEvent(QKeyEvent *event);
    void saveEditInfo();
    void insertContent(QString text);
private slots:
    void button_capsslot();
signals:
    void MykeyboardSignal(QString);
private:
    bool m_isSelectedAllFlag;
public:
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QPushButton *zhChBtn;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_lbracked;
    QPushButton *pushButton_rbracked;
    QPushButton *pushButton_shoulder;
    QPushButton *pushButton_colom;
    QPushButton *pushButton_bar;
    QPushButton *pushButton_underbar;
    QLineEdit *lineEdit;
    QLabel* zhLabel;
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_s;
    QPushButton *pushButton_d;
    QPushButton *pushButton_space;
    QPushButton *pushButton_f;
    QPushButton *pushButton_r;
    QPushButton *pushButton_e;
    QPushButton *pushButton_t;
    QPushButton *pushButton_a;
    QPushButton *pushButton_7;
    QPushButton *pushButton_6;
    QPushButton *pushButton_q;
    QPushButton *pushButton_j;
    QPushButton *pushButton_1;
    QPushButton *pushButton_h;
    QPushButton *pushButton_v;
    QPushButton *pushButton_8;
    QPushButton *pushButton_3;
    QPushButton *pushButton_g;
    QPushButton *pushButton_l;
    QPushButton *pushButton_b;
    QPushButton *pushButton_p;
    QPushButton *pushButton_k;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_c;
    QPushButton *pushButton_4;
    QPushButton *pushButton_9;
    QPushButton *pushButton_5;
    QPushButton *pushButton_w;
    QPushButton *pushButton_o;
    QPushButton *pushButton_m;
    QPushButton *pushButton_i;
    QPushButton *pushButton_2;
    QPushButton *pushButton_0;
    QPushButton *pushButton_n;
    QPushButton *pushButton_enter;
    QPushButton *pushButton_esc;
    QPushButton *pushButton_u;
    QPushButton *pushButton_x;
    QPushButton *pushButton_y;
    QPushButton *pushButton_z;
    QPushButton *pushButton_caps;
    QPushButton *pushButton_point;

    QString str;
    bool flagcaps;
    bool flagZh;

};
#endif // MYKEYBOARD_H
