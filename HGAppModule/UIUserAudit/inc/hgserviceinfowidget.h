#ifndef HGSERVICEINFOWIDGET_H
#define HGSERVICEINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>


class HGServiceInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGServiceInfoWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    
signals:
private:
    void fnWriteDB();
    void fnReadDB();

private:
    QLabel* m_userIdentifierLabel, *m_serviceProviderLabel, *m_customerServiceLabel;
    QLineEdit* m_userIdentifierEdit, *m_serviceProviderEdit, *m_customerServiceEdit;
    
    QGroupBox* m_group;
    QGridLayout* m_layout;
    std::string m_lang;
};

#endif // HGSERVICEINFOWIDGET_H
