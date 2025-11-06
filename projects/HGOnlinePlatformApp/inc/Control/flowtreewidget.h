/*
**
**	版权:	
**
**	名称:	flowtreewidget.h
**
**	作者:	侯 晓 琴
**
**	日期:	2024/10/25
**
**	描述:
**
**	功能:	流程编辑组件类
**
**	备注:
**
*/

#pragma once
#include <QApplication>
#include <QTreeWidgetItem>
#include <QDrag>
#include <QDebug>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QMimeData>

#define M_Key "FlowTreeWidget/text"

class  FlowTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit FlowTreeWidget(QWidget *parent = nullptr);
    static QString myMimeType() { return M_Key; }

signals:
    void getName(std::string name);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint startPos;
    bool _isDrag;
};
