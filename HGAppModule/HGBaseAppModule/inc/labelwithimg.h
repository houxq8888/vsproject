#ifndef LABELWITHIMG_H
#define LABELWITHIMG_H

#include <QWidget>
#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include "hgqlabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

enum {
    IMGTOP,
    IMGLEFT,
    IMGRIGHT,
    IMGRIGHTBOTTOM,
    NOIMG
};


class LabelWithImg : public QWidget
{
    Q_OBJECT
public:
    explicit LabelWithImg(int imgPos,int fontSize,const std::string& imgPath, const std::string& text, QWidget *parent = nullptr);
    void setLabelText(const std::string& text);
    QLabel* getTextLabel() {return m_textLabel;}
    HGQLabel* getImgLabel() {return m_imgLabel;}
    void setImg(const std::string &imgpath);
    void setRectPainter(int flag){m_painterFlag=flag;};
    void setFillFlag(bool flag);
    std::string getImgPath() {return m_imgPath;};


private slots:
    void clickImg();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void clickLeftButton();
    void clickLeftName(std::string);
    void clickImgLabel();
    void createModule(std::string);

private:
    QGridLayout* m_layout;
    HGQLabel* m_imgLabel;
    QLabel *m_textLabel;
    int m_painterFlag;
    bool m_fillFlag;
    bool m_isDragging;

    QRect m_currentRect;
    QPoint m_lastMousePosition;
    QPoint m_releaseMousePosition;

    std::string m_name;
    std::string m_imgPath;
    
};

#endif // LABELWITHIMG_H
