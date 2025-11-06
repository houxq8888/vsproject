#include "hguartwidget.h"

HGUartWidget::HGUartWidget(std::string lang,QWidget *parent)
    : QWidget(parent)
{ 
    m_lang = lang;
    m_serialPort=new HGSerial();
    m_layout=new QGridLayout;
    this->setLayout(m_layout);
    m_baseLayout=new QGridLayout;
    m_baseGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"BaseSet")));
    m_baseGroup->setLayout(m_baseLayout);
    m_advanceLayout=new QGridLayout;
    m_advanceGroup=new QGroupBox(QString::fromStdString(loadTranslation(m_lang,"advanceSet")));
    m_advanceGroup->setLayout(m_advanceLayout);

    m_comSelectLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"comSelect")));
    m_comSelectComboBox=new QComboBox;
    std::vector<std::string> coms=listAvailableSerialPorts();
    for (auto com : coms){
        m_comSelectComboBox->addItem(QString::fromStdString(com));
    }
    m_baudrateLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"baudrate")));
    m_baudrateComboBox=new QComboBox;
    m_baudrateComboBox->addItems({"110","300","600","1200","2400","4800","9600","14400","19200","38400","56000","57600","115200","128000","230400","256000","460800","921600","1000000","2000000","Customize"});
    m_dataBitsLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"dataBits")));
    m_dataBitsComboBox=new QComboBox;
    m_dataBitsComboBox->addItems({"8","7","6","5"});
    m_stopBitsLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"stopBits")));
    m_stopBitsComboBox=new QComboBox;
    m_stopBitsComboBox->addItems({"1","1.5","2"});
    m_parityLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"parity")));
    m_parityComboBox=new QComboBox;
    m_parityComboBox->addItems({"none","odd","even","mark","space"});
    m_flowControlLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"flowControl")));
    m_flowControlComboBox=new QComboBox;
    m_flowControlComboBox->addItems({"off","hardware","software"});
    m_openButton=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"openCom")));

    m_receiveTextEdit=new QTextEdit;
    m_receiveTextEdit->setReadOnly(true);
    m_captureLabel=new QLabel(QString::fromStdString(loadTranslation(m_lang,"captureCode")));
    m_captureLineEdit=new QLineEdit;
    m_enterCheck=new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"enter")));
    m_returnCheck=new QCheckBox(QString::fromStdString(loadTranslation(m_lang,"Return")));
    m_sendButton=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"sendCaptureCode")));

    connect(m_openButton,&QPushButton::clicked,this,[=](){ 
        if (m_serialPort->isOpen()){
            m_serialPort->close();
            m_openButton->setText(QString::fromStdString(loadTranslation(m_lang,"openCom")));
        }
        else{
            m_serialPort->open(m_comSelectComboBox->currentText().toStdString().c_str());
            if (m_serialPort->isOpen()){
                m_openButton->setText(QString::fromStdString(loadTranslation(m_lang,"closeCom")));
            }
            std::string portName=m_comSelectComboBox->currentText().toStdString();
            unsigned int speed=m_baudrateComboBox->currentText().toInt();
            int bits=m_dataBitsComboBox->currentIndex()+5;
            char event=m_parityComboBox->currentIndex()+'0';
            int stop=m_stopBitsComboBox->currentIndex()+1;
            m_serialPort->set(portName,speed,bits,m_parityComboBox->currentText().toStdString()[0],m_stopBitsComboBox->currentIndex()+1);
        }
    });
    connect(m_sendButton,&QPushButton::clicked,this,[=](){ 

    });
    m_advanceLayout->addWidget(m_receiveTextEdit,0,0,1,4);
    m_advanceLayout->addWidget(m_captureLabel,m_advanceLayout->rowCount(),0);
    m_advanceLayout->addWidget(m_captureLineEdit,m_advanceLayout->rowCount()-1,1);
    m_advanceLayout->addWidget(m_enterCheck,m_advanceLayout->rowCount()-1,2);
    m_advanceLayout->addWidget(m_returnCheck,m_advanceLayout->rowCount()-1,3);
    m_advanceLayout->addWidget(m_sendButton,m_advanceLayout->rowCount(),0,1,2);
    

    m_baseLayout->addWidget(m_comSelectLabel,0,0);
    m_baseLayout->addWidget(m_comSelectComboBox,0,1);
    m_baseLayout->addWidget(m_baudrateLabel,m_baseLayout->rowCount(),0);
    m_baseLayout->addWidget(m_baudrateComboBox,m_baseLayout->rowCount()-1,1);
    m_baseLayout->addWidget(m_dataBitsLabel,m_baseLayout->rowCount(),0);
    m_baseLayout->addWidget(m_dataBitsComboBox,m_baseLayout->rowCount()-1,1);
    m_baseLayout->addWidget(m_stopBitsLabel,m_baseLayout->rowCount(),0);
    m_baseLayout->addWidget(m_stopBitsComboBox,m_baseLayout->rowCount()-1,1);
    m_baseLayout->addWidget(m_parityLabel,m_baseLayout->rowCount(),0);
    m_baseLayout->addWidget(m_parityComboBox,m_baseLayout->rowCount()-1,1);
    m_baseLayout->addWidget(m_flowControlLabel,m_baseLayout->rowCount(),0);
    m_baseLayout->addWidget(m_flowControlComboBox,m_baseLayout->rowCount()-1,1);
    m_baseLayout->addWidget(m_openButton,m_baseLayout->rowCount(),0,1,2);

    m_layout->addWidget(m_baseGroup,0,0);
    m_layout->addWidget(m_advanceGroup,0,1);
}
HGUartWidget::~HGUartWidget(){

}
bool HGUartWidget::closeWindow(){
    return true;
}

bool HGUartWidget::eventFilter(QObject* obj,QEvent* event){
    return QObject::eventFilter(obj,event);
}