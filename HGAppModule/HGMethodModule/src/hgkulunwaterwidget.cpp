#include "hgkulunwaterwidget.h"
#include "common.h"
#include <QHeaderView>

HGKulunWaterWidget::HGKulunWaterWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_driftflag=false;
    m_driftAutoFlag=DRIFT_OFF;
    m_layout=new QGridLayout();
    this->setLayout(m_layout);
            
    m_methodLabel=new QLabel("方法名称");
    m_methodEdit=new QLineEdit();
    m_detectMLabel=new QLabel("检测器");
    m_detectCombo=new QComboBox();
    m_detectCombo->addItems({"指示电极"});
    m_dianjieDJLabel=new QLabel("电解电极");
    m_dianjieDJCombo=new QComboBox();
    // for (auto shiftMethod:kulunMethodShift){
    //     m_dianjieDJCombo->addItem(QString::fromStdString(shiftMethod.second));
    // }
    m_targetElementLabel=new QLabel("目标成分");
    m_targetElementEdit=new QLineEdit();
    m_ddjiLabel=new QLabel("滴定剂名称");
    m_ddjiCombo=new QComboBox();
    m_ddjiCombo->addItems({"卡费 PN2024110"});
    m_ddduLabel=new QLabel("滴定度");
    m_ddduEdit=new QLineEdit();
    m_ddModeMinLabel=new QLabel("最短时长");
    m_ddModeMaxLabel=new QLabel("最长时长");
    m_cuiquLabel=new QLabel("萃取时间");
    m_endTimeLabel=new QLabel("终止时间");
    m_endTimeUnitLabel=new QLabel("m");
    m_ddModeMinUnitLabel=new QLabel("m");
    m_ddModeMaxUnitLabel=new QLabel("m");
    m_cuiquUnitLabel=new QLabel("m");

    m_ddModeMinEdit=new QLineEdit();
    m_ddModeMaxEdit=new QLineEdit();
    m_cuiquEdit=new QLineEdit();
    m_endTimeEdit=new QLineEdit();

    m_targetDriftLabel=new QLabel("终点漂移");
    m_targetDriftEdit=new QLineEdit();
    m_targetDriftUnitLabel=new QLabel("ug/m");
    m_startDriftLabel=new QLabel("起始漂移");
    m_startDriftEdit=new QLineEdit();
    m_startDriftUnitLabel=new QLabel("ug/m");

    m_targetDWLabel=new QLabel("终点电位");
    m_targetDWEdit=new QLineEdit();
    m_targetDWUnitLabel=new QLabel("mV");

    m_targetMethodLabel=new QLabel("终点方式");
    m_targetMethodCombo=new QComboBox();
    // for (auto methodT:targetType){
    //     m_targetMethodCombo->addItem(QString::fromStdString(methodT.second));
    // }
    m_targetYSLabel=new QLabel("终点延时");
    m_targetYSEdit=new QLineEdit();
    m_targetYSUnitLabel=new QLabel("s");
    
    
    m_jiarebanguanLabel=new QLabel("加热伴管");
    m_jiarebanguanEdit=new QLineEdit();
    m_jiarebanguanUnitLabel=new QLabel("%");
    m_jiaozhengyinziLabel=new QLabel("校正因子");
    m_jiaozhengyinziEdit=new QLineEdit();

    m_dynamicRangeLabel=new QLabel("动态范围");
    m_dynimicRangeEdit=new QLineEdit();
    m_limitConditionLabel=new QLabel("限制条件");
    m_limitConditionCombo=new QComboBox();
    m_limitConditionCombo->addItems({"最大体积"});
    m_cubeLabel=new QLabel("体积");
    m_cubeCombo=new QComboBox();
    m_cubeCombo->addItems({"30 ml"});

    m_fanLabel=new QLabel("风扇");
    m_fanCombo=new QComboBox();
    m_fanCombo->addItems({"常开", "常关", "自动"});

    m_initTempLabel=new QLabel("初始温度");
    m_initTempEdit=new QLineEdit();
    m_initTempUnitLabel=new QLabel("°C");
    m_stableTempTimeLabel=new QLabel("恒温时间");
    m_stableTempTimeEdit=new QLineEdit();
    m_stableTempTimeUnitLabel=new QLabel("m");

    m_dataProcessBox=new QGroupBox("数据处理");
    m_dataProcessLayout=new QGridLayout();
    m_dataProcessBox->setLayout(m_dataProcessLayout);
    m_driftLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xIOS开关.png"),"漂移扣除");
    m_autoDriftLabel=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),"自动漂移扣除");
    m_humanDriftLabel=new LabelWithImg(IMGLEFT,12,getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"),"手动漂移扣除");
    m_humanDriftEdit=new QLineEdit();
    m_humanDriftUnitLabel=new QLabel("ug/m");
    m_humanDriftReadBtn=new QPushButton("读取");
    m_humanDriftTimeEdit=new QLineEdit();
    m_humanDriftTimeUnitLabel=new QLabel("s");
    m_manualblankCheckBox = new QRadioButton("手动空白");
    m_manualblankEdit = new QLineEdit();
    m_manualblankUnitLabel = new QLabel("ug");
    m_autoBlankCheckBox = new QRadioButton("自动空白");
    m_manualblankCheckBox->setChecked(true);  // 默认选中“手动空白”
    // 创建按钮组
    QButtonGroup *groupBlank = new QButtonGroup(this);

    // 加入组中（可选指定 id）
    groupBlank->addButton(m_manualblankCheckBox, 0);
    groupBlank->addButton(m_autoBlankCheckBox, 1);
    // 监听选中事件
    connect(groupBlank, &QButtonGroup::idClicked, this, [=](int id)
    { 
        if (id==    0){
            m_manualblankEdit->setEnabled(true);
        } else if (id==1){
            m_manualblankEdit->setEnabled(false);
        } else {}
        // qDebug() << "选中了按钮 ID：" << id; 
    });

    connect(m_driftLabel,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    connect(m_autoDriftLabel,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));
    connect(m_humanDriftLabel,SIGNAL(clickLeftName(std::string)),this,SLOT(slotLabelClicked(std::string)));

    m_dataProcessLayout->addWidget(m_driftLabel,0,0);
    m_dataProcessLayout->addWidget(m_autoDriftLabel,1,0);
    m_dataProcessLayout->addWidget(m_humanDriftLabel,2,0);
    m_dataProcessLayout->addWidget(m_humanDriftEdit,2,1);
    m_dataProcessLayout->addWidget(m_humanDriftUnitLabel,2,2);
    m_dataProcessLayout->addWidget(m_humanDriftReadBtn,2,3);
    m_dataProcessLayout->addWidget(m_humanDriftTimeEdit,2,4);
    m_dataProcessLayout->addWidget(m_humanDriftTimeUnitLabel,2,5);
    m_dataProcessLayout->addWidget(m_manualblankCheckBox,3,0);
    m_dataProcessLayout->addWidget(m_manualblankEdit,3,1);
    m_dataProcessLayout->addWidget(m_manualblankUnitLabel,3,2);
    m_dataProcessLayout->addWidget(m_autoBlankCheckBox,3,5);

    // m_formulateEdit=new QTextEdit();
    // m_displayUnitLabel=new QLabel("显示单位");
    // m_displayUnitCombo=new QComboBox();
    // m_displayUnitCombo->addItems({"g/ml"});

    m_hor1=new QFrame;
    m_hor1->setFrameShape(QFrame::HLine);
    m_hor1->setFrameShadow(QFrame::Sunken);
    m_hor2=new QFrame;
    m_hor2->setFrameShape(QFrame::HLine);
    m_hor2->setFrameShadow(QFrame::Sunken);
    m_hor3=new QFrame;
    m_hor3->setFrameShape(QFrame::HLine);
    m_hor3->setFrameShadow(QFrame::Sunken);

    m_tableW=new QTableWidget(4,4);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->verticalHeader()->setVisible(false);
    m_tableW->horizontalHeader()->setVisible(false);
       
    
    QString data[4][4]={
        {"","速率°C/m","温度°C","保温m"},
        {"温度1","0", "0", "0"},
        {"温度2","0", "0", "0"},
        {"温度3", "0", "0", "0"}
    };
    for (int row=0;row<4;++row){
        for (int col=0;col<4;++col){
            QTableWidgetItem* item=new QTableWidgetItem(data[row][col]);
            m_tableW->setItem(row,col,item);
            if (col==0) item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
    m_programAscendTemperatureCheckBox=new QCheckBox("程序升温");
    m_tableW->setCellWidget(0,0,m_programAscendTemperatureCheckBox);
    connect(m_programAscendTemperatureCheckBox, &QCheckBox::stateChanged, this, [=](int state){
        bool editable = (state == Qt::Checked);
    
        // 控制其它单元格是否可编辑（示例：第0行第1列）
        for (int i=1;i<m_tableW->rowCount();i++){
            for (int j=1;j<m_tableW->columnCount();j++){
                QTableWidgetItem *item = m_tableW->item(i, j);
                if (!item) {
                    item = new QTableWidgetItem("0");
                    m_tableW->setItem(i, j, item);
                }
            
                if (editable) {
                    item->setFlags(item->flags() | Qt::ItemIsEditable);
                } else {
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                }
            }
        }
    });



    m_layout->addWidget(m_methodLabel,0,0);
    m_layout->addWidget(m_methodEdit,0,1);
    m_layout->addWidget(m_detectMLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_detectCombo,m_layout->rowCount()-1,1);
    // m_layout->addWidget(m_dianjieDJLabel,m_layout->rowCount(),0);
    // m_layout->addWidget(m_dianjieDJCombo,m_layout->rowCount()-1,1);
    // m_layout->addWidget(m_fanLabel,m_layout->rowCount(),0);
    // m_layout->addWidget(m_fanCombo,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_targetElementLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_targetElementEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_ddjiLabel,m_layout->rowCount()-1,3);
    m_layout->addWidget(m_ddjiCombo,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_ddduLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_ddduEdit,m_layout->rowCount()-1,7);
    m_layout->addWidget(m_hor1,m_layout->rowCount(),0,1,10);
    m_layout->addWidget(m_ddModeMinLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_ddModeMinEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_ddModeMinUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_targetDWLabel,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_targetDWEdit,m_layout->rowCount()-1,5);
    m_layout->addWidget(m_targetDWUnitLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_dianjieDJLabel,m_layout->rowCount()-1,8);
    m_layout->addWidget(m_dianjieDJCombo,m_layout->rowCount()-1,9);

    m_layout->addWidget(m_ddModeMaxLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_ddModeMaxEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_ddModeMaxUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_targetDriftLabel,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_targetDriftEdit,m_layout->rowCount()-1,5);
    m_layout->addWidget(m_targetDriftUnitLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_fanLabel, m_layout->rowCount()-1,8);
    m_layout->addWidget(m_fanCombo,m_layout->rowCount()-1,9);

    m_layout->addWidget(m_cuiquLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_cuiquEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_cuiquUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_startDriftLabel,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_startDriftEdit,m_layout->rowCount()-1,5);
    m_layout->addWidget(m_startDriftUnitLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_jiarebanguanLabel,m_layout->rowCount()-1,8);
    m_layout->addWidget(m_jiarebanguanEdit,m_layout->rowCount()-1,9);
    m_layout->addWidget(m_jiarebanguanUnitLabel,m_layout->rowCount()-1,10);

    m_layout->addWidget(m_endTimeLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_endTimeEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_endTimeUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_targetYSLabel,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_targetYSEdit,m_layout->rowCount()-1,5);
    m_layout->addWidget(m_targetYSUnitLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_jiaozhengyinziLabel,m_layout->rowCount()-1,8);
    m_layout->addWidget(m_jiaozhengyinziEdit,m_layout->rowCount()-1,9);
    m_layout->addWidget(m_hor2,m_layout->rowCount(),0,1,10);
    // m_layout->addWidget(m_targetDWLabel,m_layout->rowCount(),0);
    // m_layout->addWidget(m_targetDWEdit,m_layout->rowCount()-1,1);
    // m_layout->addWidget(m_targetDriftLabel,m_layout->rowCount(),0);
    // m_layout->addWidget(m_targetDriftEdit,m_layout->rowCount()-1,1);
    // m_layout->addWidget(m_targetDriftUnitLabel,m_layout->rowCount()-1,2);
    // m_layout->addWidget(m_startDriftLabel,m_layout->rowCount(),0);
    // m_layout->addWidget(m_startDriftEdit,m_layout->rowCount()-1,1);
    // m_layout->addWidget(m_startDriftUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_targetMethodLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_targetMethodCombo,m_layout->rowCount()-1,1);
    // m_layout->addWidget(m_targetYSLabel,m_layout->rowCount(),0);
    // m_layout->addWidget(m_targetYSEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_dynamicRangeLabel,m_layout->rowCount()-1,3);
    m_layout->addWidget(m_dynimicRangeEdit,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_limitConditionLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_limitConditionCombo,m_layout->rowCount()-1,7);
    m_layout->addWidget(m_cubeLabel,m_layout->rowCount()-1,9);
    m_layout->addWidget(m_cubeCombo,m_layout->rowCount()-1,10);
    m_layout->addWidget(m_hor3,m_layout->rowCount(),0,1,10);
    m_layout->addWidget(m_initTempLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_initTempEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_initTempUnitLabel,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_stableTempTimeLabel,m_layout->rowCount()-1,4);
    m_layout->addWidget(m_stableTempTimeEdit,m_layout->rowCount()-1,5);
    m_layout->addWidget(m_stableTempTimeUnitLabel,m_layout->rowCount()-1,6);
    m_layout->addWidget(m_tableW, m_layout->rowCount(),0,1,7);
    m_layout->addWidget(m_dataProcessBox,m_layout->rowCount(),0,1,2);

}
bool HGKulunWaterWidget::closeWindow()
{
    return true;
}
void HGKulunWaterWidget::slotLabelClicked(std::string name){
    if (name=="漂移扣除"){
        m_driftflag=!m_driftflag;
        if (!m_driftflag) {
            m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
            m_driftAutoFlag=DRIFT_OFF;
        }
        else {
            m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
            m_driftAutoFlag=DRIFT_AUTO;
        }
    } else if (name=="自动漂移扣除"){
        m_driftAutoFlag=DRIFT_AUTO;
    } else if (name=="手动漂移扣除"){
        m_driftAutoFlag=DRIFT_MANUAL;
    }
    setDriftParam();
}
void HGKulunWaterWidget::setDriftParam()
{
    switch (m_driftAutoFlag) {
        case DRIFT_OFF:
        m_autoDriftLabel->setEnabled(false);
        m_humanDriftLabel->setEnabled(false);
        m_humanDriftEdit->setEnabled(false);
        // m_humanDriftReadBtn->setEnabled(false);
        // m_humanDriftTimeEdit->setEnabled(false);
        // m_blankLabel->setEnabled(false);
        // m_blankEdit->setEnabled(false);
        break;
        case DRIFT_AUTO:
        m_autoDriftLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
        m_humanDriftLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_autoDriftLabel->setEnabled(true);
        m_humanDriftLabel->setEnabled(true);
        m_humanDriftEdit->setEnabled(false);
        // m_humanDriftReadBtn->setEnabled(false);
        // m_humanDriftTimeEdit->setEnabled(false);
        // m_blankLabel->setEnabled(false);
        // m_blankEdit->setEnabled(false);
        break;
        case DRIFT_MANUAL:
        m_autoDriftLabel->setImg(getPath("/resources/V1/@1xmd-radio_button_unchecked 1.png"));
        m_humanDriftLabel->setImg(getPath("/resources/V1/@1xze-certificate 1.png"));
        m_autoDriftLabel->setEnabled(true);
        m_humanDriftLabel->setEnabled(true);
        m_humanDriftEdit->setEnabled(true);
        // m_humanDriftReadBtn->setEnabled(true);
        // m_humanDriftTimeEdit->setEnabled(true);
        // m_blankLabel->setEnabled(true);
        // m_blankEdit->setEnabled(true);
        break;
    }
}
void HGKulunWaterWidget::setParam(std::string param){
    setControlText(m_layout,param);
    // setControlText(m_dataProcessLayout,param);
    std::map<std::string,std::string> wparam=getParamMap(param);
    if (wparam["漂移扣除"]=="true"){
        m_driftflag=true;
        m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关_enable.png"));
    } else {
        m_driftflag=false;
        m_driftLabel->setImg(getPath("/resources/V1/@1xIOS开关.png"));
    }

    m_fanCombo->setCurrentIndex(std::atoi(wparam["风扇"].c_str()));
    m_dianjieDJCombo->setCurrentIndex(std::atoi(wparam["电解电极"].c_str()));
    if (wparam["自动漂移扣除"]=="auto"){
        m_driftAutoFlag=DRIFT_AUTO;
    } else if (wparam["自动漂移扣除"]=="manual"){
        m_driftAutoFlag=DRIFT_MANUAL;
    } else if (wparam["自动漂移扣除"]=="off"){
        m_driftAutoFlag=DRIFT_OFF;
    }
    setDriftParam();
    m_humanDriftEdit->setText(QString::fromStdString(wparam["手动漂移扣除"]));
    m_manualblankEdit->setText(QString::fromStdString(wparam["手动空白"]));
    if (wparam["空白"]=="manual"){
        m_manualblankCheckBox->setChecked(true);
        m_manualblankEdit->setEnabled(true);
    } else if (wparam["空白"]=="auto"){
        m_autoBlankCheckBox->setChecked(true);
        m_manualblankEdit->setEnabled(false);
    } else {
        m_autoBlankCheckBox->setChecked(true);
    }

    if (wparam[m_programAscendTemperatureCheckBox->text().toStdString()]=="1") 
        m_programAscendTemperatureCheckBox->setChecked(true);
    else m_programAscendTemperatureCheckBox->setChecked(false);
    bool editable=m_programAscendTemperatureCheckBox->isChecked();
    for (int i=1;i<m_tableW->rowCount();i++){
        for (int j=1;j<m_tableW->columnCount();j++){
            QTableWidgetItem *item = m_tableW->item(i, j);
            if (!item) {
                item = new QTableWidgetItem("0");
                m_tableW->setItem(i, j, item);
            }
        
            if (editable) {
                item->setFlags(item->flags() | Qt::ItemIsEditable);
            } else {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }

    m_tableW->item(1,1)->setText(QString::fromStdString(wparam["速率1"]));
    m_tableW->item(1,2)->setText(QString::fromStdString(wparam["速率2"]));
    m_tableW->item(1,3)->setText(QString::fromStdString(wparam["速率3"]));
    m_tableW->item(2,1)->setText(QString::fromStdString(wparam["温度1"]));
    m_tableW->item(2,2)->setText(QString::fromStdString(wparam["温度2"]));
    m_tableW->item(2,3)->setText(QString::fromStdString(wparam["温度3"]));
    m_tableW->item(3,1)->setText(QString::fromStdString(wparam["保温1"]));
    m_tableW->item(3,2)->setText(QString::fromStdString(wparam["保温2"]));
    m_tableW->item(3,3)->setText(QString::fromStdString(wparam["保温3"]));
}
std::map<std::string,std::vector<uint8_t>> HGKulunWaterWidget::getSendMethodInfo(){
    std::map<std::string,std::vector<uint8_t>> sendMethodInfo;
    std::string paramstr=getParamStr();
    std::map<std::string,std::string> wparam=getParamMap(paramstr);
    #ifdef ENABLE_CAS6
    for (const auto& pair : gMapSendMethod){
        if (wparam.find(pair.first) == wparam.end()) continue;
        std::string paramStr=wparam[pair.first];
        std::vector<uint8_t> input=std::vector<uint8_t>(paramStr.begin(), paramStr.end());
        sendMethodInfo[pair.first]=input;
    }
    #endif
    return sendMethodInfo;
}
std::string HGKulunWaterWidget::getParamStr(){
    std::string driftStr="";
    if (m_driftflag) driftStr="true";
    else driftStr="false";
    std::string autoDriftStr="";
    switch (m_driftAutoFlag){
        case DRIFT_AUTO:
        autoDriftStr="auto";
        break;
        case DRIFT_MANUAL:
        autoDriftStr="manual";
        break;
        case DRIFT_OFF:
        autoDriftStr="off";
        break;
    }
    std::string blankMode="";
    if (m_manualblankCheckBox->isChecked()) blankMode="manual";
    if (m_autoBlankCheckBox->isChecked()) blankMode="auto";


    std::ostringstream param;
    param<<m_methodLabel->text().toStdString()<<":" \
      <<m_methodEdit->text().toStdString()<<"," \
      <<m_detectMLabel->text().toStdString()<<":" \
      <<m_detectCombo->currentText().toStdString()<<"," \
      <<m_dianjieDJLabel->text().toStdString()<<":" \
      <<std::to_string(m_dianjieDJCombo->currentIndex())<<"," \
      <<m_targetElementLabel->text().toStdString()<<":" \
      <<m_targetElementEdit->text().toStdString()<<"," \
      <<m_ddjiLabel->text().toStdString()<<":" \
      <<m_ddjiCombo->currentText().toStdString()<<"," \
      <<m_fanLabel->text().toStdString()<<":" \
      <<std::to_string(m_fanCombo->currentIndex())<<"," \
      <<m_ddduLabel->text().toStdString()<<":" \
      <<m_ddduEdit->text().toStdString()<<"," \
      <<m_ddModeMinLabel->text().toStdString()<<":" \
      <<m_ddModeMinEdit->text().toStdString()<<"," \
      <<m_ddModeMaxLabel->text().toStdString()<<":" \
      <<m_ddModeMaxEdit->text().toStdString()<<"," \
      <<m_endTimeLabel->text().toStdString()<<":" \
      <<m_endTimeEdit->text().toStdString()<<"," \
      <<m_cuiquLabel->text().toStdString()<<":" \
      <<m_cuiquEdit->text().toStdString()<<"," \
      <<m_programAscendTemperatureCheckBox->text().toStdString()<<":" \
      <<(m_programAscendTemperatureCheckBox->isChecked()?"1":"0")<<"," \
      <<"速率1"<<":" \
      <<m_tableW->item(1, 1)->text().toStdString()<<"," \
      <<"速率2"<<":" \
      <<m_tableW->item(1, 2)->text().toStdString()<<"," \
      <<"速率3"<<":" \
      <<m_tableW->item(1, 3)->text().toStdString()<<"," \
      <<"温度1"<<":" \
      <<m_tableW->item(2, 1)->text().toStdString()<<"," \
      <<"温度2"<<":" \
      <<m_tableW->item(2, 2)->text().toStdString()<<"," \
      <<"温度3"<<":" \
      <<m_tableW->item(2, 3)->text().toStdString()<<"," \
      <<"保温1"<<":" \
      <<m_tableW->item(3, 1)->text().toStdString()<<"," \
      <<"保温2"<<":" \
      <<m_tableW->item(3, 2)->text().toStdString()<<"," \
      <<"保温3"<<":" \
      <<m_tableW->item(3, 3)->text().toStdString()<<"," \
      <<m_targetDWLabel->text().toStdString()<<":" \
      <<m_targetDWEdit->text().toStdString()<<"," \
      <<m_targetDriftLabel->text().toStdString()<<":" \
      <<m_targetDriftEdit->text().toStdString()<<"," \
      <<m_startDriftLabel->text().toStdString()<<":" \
      <<m_startDriftEdit->text().toStdString()<<"," \
      <<m_jiarebanguanLabel->text().toStdString()<<":" \
      <<m_jiarebanguanEdit->text().toStdString()<<"," \
      <<m_jiaozhengyinziLabel->text().toStdString()<<":" \
      <<m_jiaozhengyinziEdit->text().toStdString()<<"," \
      <<m_initTempLabel->text().toStdString()<<":" \
      <<m_initTempEdit->text().toStdString()<<"," \
      <<m_stableTempTimeLabel->text().toStdString()<<":" \
      <<m_stableTempTimeEdit->text().toStdString()<<"," \
      <<m_targetMethodLabel->text().toStdString()<<":" \
      <<m_targetMethodCombo->currentText().toStdString()<<"," \
      <<m_targetYSLabel->text().toStdString()<<":" \
      <<m_targetYSEdit->text().toStdString()<<"," \
      <<m_dynamicRangeLabel->text().toStdString()<<":" \
      <<m_dynimicRangeEdit->text().toStdString()<<"," \
      <<m_limitConditionLabel->text().toStdString()<<":" \
      <<m_limitConditionCombo->currentText().toStdString()<<"," \
      <<m_humanDriftReadBtn->text().toStdString()<<":" \
      <<m_humanDriftTimeEdit->text().toStdString()<<"," \
      <<m_manualblankCheckBox->text().toStdString()<<":" \
      <<m_manualblankEdit->text().toStdString()<<"," \
      <<m_cubeLabel->text().toStdString()<<":" \
      <<m_cubeCombo->currentText().toStdString()<<"," \
      <<"空白"<<":" \
      <<blankMode<<"," \
      <<m_driftLabel->getTextLabel()->text().toStdString()<<":" \
      <<driftStr<<"," \
      <<m_autoDriftLabel->getTextLabel()->text().toStdString()<<":" \
      <<autoDriftStr<<"," \
      <<m_humanDriftLabel->getTextLabel()->text().toStdString()<<":" \
      <<m_humanDriftEdit->text().toStdString(); // <<"," \
    //   <<m_blankLabel->text().toStdString()<<":" \
    //   <<m_blankEdit->text().toStdString();
    return param.str();
}