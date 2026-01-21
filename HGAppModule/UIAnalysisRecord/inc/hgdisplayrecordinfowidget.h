#ifndef HGDISPLAYRECORDINFOWIDGET_H
#define HGDISPLAYRECORDINFOWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QTableWidget>
#include <QDialog>
#include <QDockWidget>
#include "DragDialog.h"
#include "labelwithimg.h"



class HGDisplayRecordInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGDisplayRecordInfoWidget(std::string,const std::string &name,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGDisplayRecordInfoWidget();
    void showTestInfo(std::map<std::string,std::string> info);

signals:

private slots:
    void slotDisplayType();
    void slotLabelClicked(std::string);

private:
    void fnUpdateDisplay(bool,std::string);
    void fnWriteDB();
    void fnReadDB();

private:
    HGQLabel* m_displayTypeLabel, *m_closeLabel;
    QGridLayout* m_layout;
    QLabel* m_nameLabel;
    QTableWidget *m_tableW;
    
    std::map<std::string,recordLabel> m_recordLabels;

    DraggableDialog *m_dragDialog;
    std::string m_lang;
};

#endif // HGDISPLAYRECORDINFOWIDGET_H
