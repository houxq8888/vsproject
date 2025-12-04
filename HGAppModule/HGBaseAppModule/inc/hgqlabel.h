/*
**
**	版权:	
**
**	名称:	hgqlabel.h
**
**	作者:	侯 晓 琴
**
**	日期:	2024/10/28
**
**	描述:
**
**	功能:	重写QLabel类
**
**	备注:
**
*/

#ifndef HGQLABEL_H
#define HGQLABEL_H


#include <QLabel>
#include <QMouseEvent>
#include <QModelIndex>
#include <QTimer>
#include <QResizeEvent>
#include <QPixmap>

class HGQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit HGQLabel(QWidget *parent = nullptr);
    HGQLabel(bool flipVer,const std::string &imgPath,QWidget* parent=nullptr);
    ~HGQLabel();
    QString getLabelText();
    void changePixmap(const std::string &imgPath,int size);
    void init();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent* event) override;
protected:
    virtual bool event(QEvent *e);

signals:
    void leftClicked();
    void leftdoubleClicked();
    void rightClicked();
    void rightdoubleClicked();
private:
    QLabel* m_text;
    QPixmap pixmap;
    bool flag;

};


#endif // HGQLABEL_H
