#include "hgsharedfilewidget.h"
#include <QFileDialog>


HGSharedFileWidget::HGSharedFileWidget(std::string name,QWidget *parent)
    : QWidget(parent)
{
    m_uploader=nullptr;
    m_lang = name; 
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
    m_baselayout=new QGridLayout();
    m_advancedLayout=new QGridLayout();
    m_baseGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"BaseSet")));
    m_advancedGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"advanceSet")));
    m_baseGroup->setLayout(m_baselayout);
    m_advancedGroup->setLayout(m_advancedLayout);

    m_interfaceLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Interface")));
    m_interfaceComboBox=new QComboBox();
    m_interfaceComboBox->addItems({"Wifi"});
    m_encodeTypeLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Encode")));
    m_encodeTypeComboBox=new QComboBox();
    m_encodeTypeComboBox->addItems({"AES","MD5"});
    m_encodeTypeComboBox->setCurrentIndex(0);
    // connect(m_encodeTypeComboBox,&QComboBox::currentIndexChanged,this,&HGEBalanceWidget::fnChangeParam);
    m_outputEnableLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Output")));
    m_outputEnableComboBox=new QComboBox();
    m_outputEnableComboBox->addItems({QString::fromStdString(loadTranslation(m_lang,"On")),
            QString::fromStdString(loadTranslation(m_lang,"Off"))});
    m_outputEnableComboBox->setCurrentIndex(0);
    m_ipaddressLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"IPAddress")));
    m_ipaddressEdit=new QLineEdit();
    m_usernameLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"User")));
    m_usernameEdit=new QLineEdit();
    m_passwdLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"Password")));
    m_passwdEdit=new QLineEdit();
    m_savepathLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"SavePath")));
    m_savepathEdit=new QLineEdit();
    m_connectBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Connect")));
    m_sendFileBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Send")));
    connect(m_connectBtn,&QPushButton::clicked,this,&HGSharedFileWidget::slotConnectShared);
    connect(m_sendFileBtn,&QPushButton::clicked,this,&HGSharedFileWidget::slotSendFile);

    fnChangeParam();

    m_baselayout->addWidget(m_interfaceLabel,0,0);
    m_baselayout->addWidget(m_interfaceComboBox,0,1);
    m_baselayout->addWidget(m_encodeTypeLabel,1,0);
    m_baselayout->addWidget(m_encodeTypeComboBox,1,1);
    m_baselayout->addWidget(m_outputEnableLabel,2,0);
    m_baselayout->addWidget(m_outputEnableComboBox,2,1);

    m_advancedLayout->addWidget(m_ipaddressLabel,0,0);
    m_advancedLayout->addWidget(m_ipaddressEdit,0,1);
    m_advancedLayout->addWidget(m_usernameLabel,1,0);
    m_advancedLayout->addWidget(m_usernameEdit,1,1);
    m_advancedLayout->addWidget(m_passwdLabel,2,0);
    m_advancedLayout->addWidget(m_passwdEdit,2,1);
    m_advancedLayout->addWidget(m_savepathLabel,3,0);
    m_advancedLayout->addWidget(m_savepathEdit,3,1);
    m_advancedLayout->addWidget(m_connectBtn,4,0);
    m_advancedLayout->addWidget(m_sendFileBtn,4,1);
    m_layout->addWidget(m_baseGroup,0,0);
    m_layout->addWidget(m_advancedGroup,1,0);
}
HGSharedFileWidget::~HGSharedFileWidget(){

}
bool HGSharedFileWidget::closeWindow(){
    return true;
}
void HGSharedFileWidget::fnChangeParam(){
    removeWidgetsFromLayout(m_advancedLayout);
    switch (m_interfaceComboBox->currentIndex()){
        case 0:{
            
            break;
        }
    }
}
bool HGSharedFileWidget::eventFilter(QObject* obj,QEvent* event){
    return QObject::eventFilter(obj,event);
}
void HGSharedFileWidget::slotConnectShared()
{
    if (m_uploader == nullptr)
    {
        m_uploader = new SshUploader(m_ipaddressEdit->text().toStdString(),
                                     22, m_usernameEdit->text().toStdString(),
                                     m_passwdEdit->text().toStdString());
        if (!m_uploader->connect())
        {
            std::cerr << "failed to connect SSH server\n";
            return;
        }
        m_connectBtn->setText(QString::fromStdString(loadTranslation(m_lang, "Disconnect")));
    }
    else
    {
        if (!m_uploader->disconnect())
        {
            m_connectBtn->setText(QString::fromStdString(loadTranslation(m_lang, "Connect")));
        }
    }
}
void HGSharedFileWidget::slotSendFile(){
    std::string file_path=(QFileDialog::getOpenFileName(NULL,"选择要发送的文件","./",tr("All files(*.*)"))).toStdString();
    std::string filename;
       // 找到最后一个 '/' 的位置，提取文件名
    size_t last_slash = file_path.find_last_of("/\\");
    std::string file_name = (last_slash == std::string::npos) ? file_path : file_path.substr(last_slash + 1);

    // 找到最后一个 '.' 的位置，提取扩展名
    size_t last_dot = file_name.find_last_of(".");
    std::string base_name = (last_dot == std::string::npos) ? file_name : file_name.substr(0, last_dot);
    std::string extension = (last_dot == std::string::npos) ? "" : file_name.substr(last_dot + 1);

    filename=base_name+"."+extension;
    std::cout << "完整文件名: " << file_name << std::endl;
    std::cout << "文件名（无扩展名）: " << base_name << std::endl;
    std::cout << "扩展名: " << extension << std::endl;

    if (!m_uploader->uploadFile(file_path, m_savepathEdit->text().toStdString() + filename))
    {
        std::cerr << "file upload failed\n";
        return;
    }
    std::cout << "upload sucess\n";
}