#ifndef HGREAGENTINNERWIDGET_H
#define HGREAGENTINNERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QPushButton>

class HGReagentInnerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGReagentInnerWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    int getRowCount();
    std::vector<std::string> getFields();

signals:

private:
    QGridLayout* m_layout;
    QTableWidget* m_tableW;
    QPushButton* m_addBtn;
    std::string m_lang;
};

#endif // HGREAGENTINNERWIDGET_H
