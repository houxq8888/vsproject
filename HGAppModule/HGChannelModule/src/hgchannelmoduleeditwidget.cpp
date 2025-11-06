#include "hgchannelmoduleeditwidget.h"
#include "common.h"


HGChannelModuleEditWidget::HGChannelModuleEditWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_displayFlag=false;
    m_modules.clear();
    m_swirlWs.clear();
    m_phDDWs.clear();
    m_ddModuleWs.clear();
    m_cancelBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Cancel")));//"取消");
    m_okBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Ok")));//"确定");
    m_applyBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Apply")));//"应用");

    connect(m_cancelBtn,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(m_okBtn,SIGNAL(clicked()),this,SLOT(slotOk()));
    connect(m_applyBtn,SIGNAL(clicked()),this,SLOT(slotApply()));

    m_channelCombo=new QComboBox();
    m_channelCombo->addItems({"1","2","3","4"});
    
    m_toolBox=new QToolBox();
    m_toolBox->addItem(m_channelCombo,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
        QString::fromStdString(loadTranslation(m_lang,"SetChannel")));//"配置通道");
    connect(m_toolBox,SIGNAL(currentChanged(int)),this,SLOT(togglePage(int)));

    m_toolBox->setStyleSheet(R"(
        QToolBox::tab::selected {
            font-weight: bold;
            color:while;
            background-color: #0078d4;
            border: 1px solid #005a9e;
            padding: 5px;
        }
        QToolBox::tab {
            background: lightgray;
            padding: 5px;
        }
    )");
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_layout->addWidget(m_toolBox,0,0,1,3);
    m_layout->addWidget(m_cancelBtn,1,0);
    m_layout->addWidget(m_okBtn,1,1);
    m_layout->addWidget(m_applyBtn,1,2);
}
bool HGChannelModuleEditWidget::closeWindow()
{
    for (auto &widget:m_phDDWs){
        if (widget.second){
            if (widget.second->closeWindow()){
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget:m_ddModuleWs){
        if (widget.second){
            if (widget.second->closeWindow()){
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget:m_swirlWs){
        if (widget.second){
            if (widget.second->closeWindow()){
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget:m_auxPumpWs){
        if (widget.second){
            if (widget.second->closeWindow()){
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget:m_eleBalanceWs){
        if (widget.second){
            if (widget.second->closeWindow()){
                SAFE_DELETE(widget.second);
            }
        }
    }
    return true;
}
void HGChannelModuleEditWidget::togglePage(int index){
    int cnt=m_toolBox->count();
    for (int i=0;i<cnt;i++){
        if (i==index) m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow-down 1.png"))));
        else m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))));
    }
}
void HGChannelModuleEditWidget::showChannel(Channel info){
    m_displayFlag=true;
    while (m_toolBox->count() > 1){
        m_toolBox->removeItem(1);
    }
    m_channel.Clear();
    m_modules.clear();
    m_channel=info;
    std::vector<ModuleOfChannel> modules=info.modules;
    m_channelCombo->setCurrentText(QString::number(info.channel));
    for (const auto &moduleOfChannel: modules){
        createModule(moduleOfChannel.name,moduleOfChannel.param);
    }
    fninitChannelData(true);
}
void HGChannelModuleEditWidget::createModule(std::string name,std::string param){
    ModuleOfChannel moduleOfChannel;
    moduleOfChannel.name=name;
    moduleOfChannel.index=m_modules.size()+1;
    if (name=="pH电极"){
        moduleOfChannel.nameOfSameModule=name+std::to_string(m_phDDWs.size());
        moduleOfChannel.indexOfSameModule=int(m_phDDWs.size());
        moduleOfChannel.typeName="检测器";
        m_phDDWs[moduleOfChannel.indexOfSameModule]=new HGphDDWidget(m_lang,moduleOfChannel.nameOfSameModule);
        m_toolBox->addItem(m_phDDWs[moduleOfChannel.indexOfSameModule],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_phDDWs[moduleOfChannel.indexOfSameModule]->setParamStr(param);
        connect(m_phDDWs[moduleOfChannel.indexOfSameModule],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if (name=="颜色传感器"){
        moduleOfChannel.typeName="检测器";
    } else if (name=="光度计") {
        moduleOfChannel.typeName="检测器";
    } else if (name=="指标电极(容量法)") {
        moduleOfChannel.typeName="检测器";
    } else if (name.find("滴定组件")!=std::string::npos){
        moduleOfChannel.typeName="馈液单元";
        moduleOfChannel.nameOfSameModule=name+std::to_string(m_ddModuleWs.size());
        moduleOfChannel.indexOfSameModule=int(m_ddModuleWs.size());
        std::string type=name.substr(name.find_first_of("(")+1,name.find_last_of(")")-name.find_first_of("(")-1);
        m_ddModuleWs[moduleOfChannel.indexOfSameModule]=new HGDDModuleWidget(m_lang,type,moduleOfChannel.nameOfSameModule);
        m_toolBox->addItem(m_ddModuleWs[moduleOfChannel.indexOfSameModule],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_ddModuleWs[moduleOfChannel.indexOfSameModule]->setParamStr(param);
        connect(m_ddModuleWs[moduleOfChannel.indexOfSameModule],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if (name==("搅拌器(无加热)")){
        moduleOfChannel.typeName="搅拌器";
        moduleOfChannel.nameOfSameModule=name+std::to_string(m_swirlWs.size());
        moduleOfChannel.indexOfSameModule=int(m_swirlWs.size());
        m_swirlWs[moduleOfChannel.indexOfSameModule]=new HGSwirlWidget(m_lang,false,moduleOfChannel.nameOfSameModule);
        m_toolBox->addItem(m_swirlWs[moduleOfChannel.indexOfSameModule],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_swirlWs[moduleOfChannel.indexOfSameModule]->setParamStr(param);
        connect(m_swirlWs[moduleOfChannel.indexOfSameModule],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if (name==("搅拌器(带加热)")){
        moduleOfChannel.typeName="搅拌器";
        moduleOfChannel.nameOfSameModule=name+std::to_string(m_swirlWs.size());
        moduleOfChannel.indexOfSameModule=int(m_swirlWs.size());
        m_swirlWs[moduleOfChannel.indexOfSameModule]=new HGSwirlWidget(m_lang,true,moduleOfChannel.nameOfSameModule);
        m_toolBox->addItem(m_swirlWs[moduleOfChannel.indexOfSameModule],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_swirlWs[moduleOfChannel.indexOfSameModule]->setParamStr(param);
        connect(m_swirlWs[moduleOfChannel.indexOfSameModule],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if (name=="辅助泵"){
        moduleOfChannel.typeName="辅助泵";
        moduleOfChannel.nameOfSameModule=name+std::to_string(m_auxPumpWs.size());
        moduleOfChannel.indexOfSameModule=int(m_auxPumpWs.size());
        m_auxPumpWs[moduleOfChannel.indexOfSameModule]=new HGAuxPumpWidget(m_lang,moduleOfChannel.nameOfSameModule);
        m_toolBox->addItem(m_auxPumpWs[moduleOfChannel.indexOfSameModule],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_auxPumpWs[moduleOfChannel.indexOfSameModule]->setParamStr(param);
        connect(m_auxPumpWs[moduleOfChannel.indexOfSameModule],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));

    } else if (name=="电子天平"){
        moduleOfChannel.typeName="外接设备";
        moduleOfChannel.nameOfSameModule=name+std::to_string(m_eleBalanceWs.size());
        moduleOfChannel.indexOfSameModule=int(m_eleBalanceWs.size());
        m_eleBalanceWs[moduleOfChannel.indexOfSameModule]=new HGEleBalanceWidget(m_lang,moduleOfChannel.nameOfSameModule);
        m_toolBox->addItem(m_eleBalanceWs[moduleOfChannel.indexOfSameModule],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_eleBalanceWs[moduleOfChannel.indexOfSameModule]->setParamStr(param);
        connect(m_eleBalanceWs[moduleOfChannel.indexOfSameModule],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));

    }
    m_modules.push_back(moduleOfChannel);
}
Channel HGChannelModuleEditWidget::getChannel() {
    fnUpdateChannelInfo();
    return m_channel;
}
void HGChannelModuleEditWidget::fninitChannelData(bool flag){
    m_channelCombo->setEnabled(!flag);
    for (int i = 0; i < int(m_channel.modules.size()); i++)
    {
        if (m_channel.modules[i].name == "pH电极")
        {
            for (auto &widget : m_phDDWs)
            {
                widget.second->setReadOnly(flag);
            }
        }
        else if (m_channel.modules[i].name.find("滴定组件") != std::string::npos)
        {
            for (auto &widget : m_ddModuleWs)
            {
                widget.second->setReadOnly(flag);
            }
        }
        else if (m_channel.modules[i].name.find("搅拌器") != std::string::npos)
        {
            for (auto &widget : m_swirlWs)
            {
                widget.second->setReadOnly(flag);
            }
        }
        else if (m_channel.modules[i].name == "辅助泵")
        {
            for (auto &widget : m_auxPumpWs)
            {
                widget.second->setReadOnly(flag);
            }
        }
        else if (m_channel.modules[i].name == "电子天平")
        {
            for (auto &widget : m_eleBalanceWs)
            {
                widget.second->setReadOnly(flag);
            }
        }
    }
}
void HGChannelModuleEditWidget::fnSetChannelData(){
    QString numberStr=QString::number(m_channel.channel);
    if (m_channelCombo->findText(QString::number(m_channel.channel))!=-1){
        m_channelCombo->setCurrentText(numberStr);
    } else m_channelCombo->setCurrentIndex(-1);
}
void HGChannelModuleEditWidget::setEditFlag(bool flag){
    fninitChannelData(!flag);
}
void HGChannelModuleEditWidget::slotOk()
{
    fnUpdateChannelInfo();
    fnDisplayDB();
    fninitChannelData(true);
    m_displayFlag=true;
}
void HGChannelModuleEditWidget::fnDisplayDB(){
    
    emit updateChannelInfo(m_channel);
}
void HGChannelModuleEditWidget::slotApply(){
    
}
void HGChannelModuleEditWidget::slotCancel(){
    m_displayFlag=false;
    m_channel.Clear();
    m_modules.clear();
    int count=m_toolBox->count();
    while (count >1){
        m_toolBox->removeItem(1);
        count--;
    }
}
void HGChannelModuleEditWidget::fnUpdateChannelInfo()
{
    m_channel.channel = m_channelCombo->currentText().toInt();
    m_channel.modules.clear();
    for (int i = 0; i < int(m_modules.size()); i++)
    {
        std::string name = m_modules[i].name;
        int index = m_modules[i].indexOfSameModule;
        if ("pH电极" == name)
        {
            m_modules[i].param = m_phDDWs[index]->getParamStr();
        }
        else if (name == "颜色传感器")
        {
        }
        else if (name == "光度计")
        {
        }
        else if (name == "指标电极(容量法)")
        {
        }
        else if (name.find("滴定组件") != std::string::npos)
        {
            m_modules[i].param=m_ddModuleWs[index]->getParamStr();
        }
        else if (name.find("搅拌器")!=std::string::npos)
        {
            m_modules[i].param=m_swirlWs[index]->getParamStr();
        }
        else if (name == "辅助泵")
        {
            m_modules[i].param=m_auxPumpWs[index]->getParamStr();
        }
        else if (name == "电子天平")
        {
            m_modules[i].param=m_eleBalanceWs[index]->getParamStr();
        }
        m_channel.modules = m_modules;
    }
}
void HGChannelModuleEditWidget::slotManipulateObject(std::string name,int type){
    int currentIndex=m_toolBox->currentIndex();    
    switch (type){
        case MANIPULATE_DOWN:
        {
            break;
        }
        case MANIPULATE_UP:
        {
            break;
        }
        case MANIPULATE_CLOSE:{
            int indexOfSameModule=-1,index=-1;
            std::string typeName="";
            m_toolBox->removeItem(currentIndex);
            for (int i=0;i<int(m_modules.size());i++){
                if (m_modules[i].nameOfSameModule==name){
                    indexOfSameModule=m_modules[i].indexOfSameModule;
                    index=m_modules[i].index;
                    typeName=m_modules[i].name;
                    m_modules.erase(m_modules.begin()+i);
                    break;
                }
            }
            if (indexOfSameModule==-1) return;
            for (int i=0;i<int(m_modules.size());i++){
                if (name.find(m_modules[i].name)!=std::string::npos){
                    if (m_modules[i].indexOfSameModule > indexOfSameModule)
                    {
                        m_modules[i].indexOfSameModule-=1;
                        m_modules[i].nameOfSameModule=m_modules[i].name+std::to_string(m_modules[i].indexOfSameModule);
                    } 
                }
                if (m_modules[i].index > index){
                    m_modules[i].index-=1;
                }
            }
            // delete map
            if ("pH电极"==typeName){
                auto it=m_phDDWs.find(indexOfSameModule);
                if (it!=m_phDDWs.end()){
                    m_phDDWs.erase(it);
                }
                for (auto w:m_phDDWs){
                    if (w.first > indexOfSameModule){
                        HGphDDWidget *phDDW=w.second;
                        auto it=m_phDDWs.find(w.first);
                        if (it!=m_phDDWs.end()){
                            m_phDDWs.erase(it);
                            m_phDDWs[w.first-1]=phDDW;
                        }
                    }
                }
            }else if (typeName.find("滴定组件")!=std::string::npos){
                auto it=m_ddModuleWs.find(indexOfSameModule);
                if (it!=m_ddModuleWs.end()){
                    m_ddModuleWs.erase(it);
                }
                for (auto w:m_ddModuleWs){
                    if (w.first > indexOfSameModule){
                        HGDDModuleWidget *ddModuleW=w.second;
                        auto it=m_ddModuleWs.find(w.first);
                        if (it!=m_ddModuleWs.end()){
                            m_ddModuleWs.erase(it);
                            m_ddModuleWs[w.first-1]=ddModuleW;
                        }
                    }
                }
            } else if (typeName.find("搅拌器")!=std::string::npos){
                auto it=m_swirlWs.find(indexOfSameModule);
                if (it!=m_swirlWs.end()){
                    m_swirlWs.erase(it);
                }
                for (auto w:m_swirlWs){
                    if (w.first > indexOfSameModule){
                        HGSwirlWidget *swirlW=w.second;
                        auto it=m_swirlWs.find(w.first);
                        if (it!=m_swirlWs.end()){
                            m_swirlWs.erase(it);
                            m_swirlWs[w.first-1]=swirlW;
                        }
                    }
                }     
            } else if ("辅助泵"==typeName){
                auto it=m_auxPumpWs.find(indexOfSameModule);
                if (it!=m_auxPumpWs.end()){
                    m_auxPumpWs.erase(it);
                }
                for (auto w:m_auxPumpWs){
                    if (w.first > indexOfSameModule){
                        HGAuxPumpWidget *auxPumpW=w.second;
                        auto it=m_auxPumpWs.find(w.first);
                        if (it!=m_auxPumpWs.end()){
                            m_auxPumpWs.erase(it);
                            m_auxPumpWs[w.first-1]=auxPumpW;
                        }
                    }
                }
            } else if ("电子天平"==typeName){
                auto it=m_eleBalanceWs.find(indexOfSameModule);
                if (it!=m_eleBalanceWs.end()){
                    m_eleBalanceWs.erase(it);
                }
                for (auto w:m_eleBalanceWs){
                    if (w.first > indexOfSameModule){
                        HGEleBalanceWidget *eleBalanceW=w.second;
                        auto it=m_eleBalanceWs.find(w.first);
                        if (it!=m_eleBalanceWs.end()){
                            m_eleBalanceWs.erase(it);
                            m_eleBalanceWs[w.first-1]=eleBalanceW;
                        }
                    }
                }
            } 
        
            break;
        }
    }
}