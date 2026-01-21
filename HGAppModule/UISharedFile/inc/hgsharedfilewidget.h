#ifndef HGSHAREDFILEWIDGET_H
#define HGSHAREDFILEWIDGET_H

#include <QWidget>
#include "hgqlabel.h"
#include <QSplitter>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include "mykeyboard.h"
#include "common.h"
#include "hgcomwithssh.h"


class HGSharedFileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGSharedFileWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~HGSharedFileWidget();

signals:
  

private slots:
    void slotConnectShared();
    void slotSendFile();

private:
    bool eventFilter(QObject* obj,QEvent* event);
    void fnChangeParam();
    
private:
    QGridLayout* m_layout, *m_baselayout,*m_advancedLayout;
    QGroupBox* m_baseGroup, *m_advancedGroup;
    std::string m_lang;

    QLabel* m_interfaceLabel, *m_encodeTypeLabel, *m_outputEnableLabel;
    QComboBox *m_interfaceComboBox, *m_encodeTypeComboBox, *m_outputEnableComboBox;

    QLabel* m_ipaddressLabel, *m_usernameLabel, *m_passwdLabel, *m_savepathLabel;
    QLineEdit *m_ipaddressEdit, *m_usernameEdit, *m_passwdEdit, *m_savepathEdit;

    QPushButton* m_connectBtn, *m_sendFileBtn;

private:
    SshUploader* m_uploader;
};

#endif // HGSHAREDFILEWIDGET_H
