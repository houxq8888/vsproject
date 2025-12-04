#ifndef HGCURVEWIDGET_H
#define HGCURVEWIDGET_H

#include <QColorDialog>
#include <QMouseEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include "labelwithimg.h"




class HGCurveWidget : public QWidget {
    Q_OBJECT

public:
    explicit HGCurveWidget(bool flag,const std::string& lang,
        const std::string& name,
        const QColor& color,QWidget *parent = nullptr);
    bool closeWindow();
    void setImg(std::string);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void slotLabelClicked(std::string);
signals:
    void clickLeftName(std::string);

private:
    LabelWithImg* m_label;
    QComboBox* m_unitCombo;
    QLabel* m_colorLabel;
    
    QHBoxLayout* m_layout;
    std::string m_lang;
};

#endif // HGCURVEWIDGET_H