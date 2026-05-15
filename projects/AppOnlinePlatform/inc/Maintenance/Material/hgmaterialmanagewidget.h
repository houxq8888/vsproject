#ifndef HGMATERIALMANAGEWIDGET_H
#define HGMATERIALMANAGEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include "hgqlabel.h"
#include <QPushButton>

class HGMaterialManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGMaterialManageWidget(int,std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGMaterialManageWidget();

signals:

private slots:
    void slotSearch();
    void slotOpenXLSX();
    void slotSaveXLSX();
    void slotDeleteRecord();
    
private:
    void fnReadDB();
    void fnWriteDB(const std::map<std::string,std::string> &);
    void fnDisplayMaterialInfo(int count,std::map<std::string,std::string> fillContent);
    int hasMaterialNo(const std::string &materialNo);

private:
    HGQLabel *m_searchHGLabel;
    QGroupBox *m_infoGroup;
    QGridLayout *m_infoLayout, *m_layout;
    QLabel* m_searchLabel;
    QLineEdit* m_searchEdit;
    QTableWidget* m_tableW;

    QLabel* m_materialNumberLabel,*m_getNumberLabel;
    QLineEdit* m_materialNumberEdit,*m_getNumberEdit;
    HGQLabel* m_openLabel,*m_saveLabel,*m_deleteLabel;
    QPushButton *m_inputCabinBtn,*m_getMaterialBtn;
    std::string m_lang;

    QLabel* m_markLabel1, *m_markLabel2;
    std::vector<std::map<std::string,std::string>> m_materialInfos;
    int m_loginAuthority;
};

#endif // HGMATERIALMANAGEWIDGET_H
