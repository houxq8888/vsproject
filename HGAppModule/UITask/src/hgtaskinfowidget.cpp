#include "hgtaskinfowidget.h"
#include <sstream>
#include <QButtonGroup>
#include "common.h"

HGTaskInfoWidget::HGTaskInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{   
    m_editType=TASK_NOEDIT;
    m_sampleNameLabel=new QLabel("样品名称");
    m_sampleNameEdit=new QLineEdit();
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_rectifyBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Rectify")));//"修改");
    m_saveBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"Save")));//"保存");
    m_createBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"New")));//"新建");
    connect(m_rectifyBtn,       SIGNAL(clicked()),this,SLOT(clickRectifyTask()));
    connect(m_saveBtn,          SIGNAL(clicked()),this,SLOT(clickSaveTask()));
    connect(m_createBtn,        SIGNAL(clicked()),this,SLOT(clickCreateTask()));
    m_testFlowLabel=new QLabel("测试流程");
    m_testFlowCombo=new QComboBox();
    std::vector<std::string> names=RWDb::getFlowNames();
    for (int i=0;i<int(names.size());i++){
        m_testFlowCombo->addItem(QString::fromStdString(names[i]));
    }
    m_testChannelLabel=new QLabel("测试通道");
    m_testChannelEdit=new QSpinBox();
    #ifdef ENABLE_CAS6
    m_testChannelEdit->setRange(1,1);
    #else
    m_testChannelEdit->setRange(1,4);
    #endif


    #ifdef ENABLE_CAS6
    m_circleNoLabel=new QLabel("盘位");
    m_circleNoEdit=new QSpinBox();
    m_circleNoEdit->setRange(1,36);
    m_blankBox=new QCheckBox("空白");
    m_batchNoLabel=new QLabel("批号");
    m_batchNoEdit=new QLineEdit();
    m_sampleNoLabel=new QLabel("样品编号");
    m_sampleNoEdit=new QLineEdit();
    m_weightLabel=new QLabel("重量");
    m_getWeightBtn=new QPushButton(QString::fromStdString(loadTranslation(m_lang,"GetWeight")));
    m_weightEdit=new QLineEdit();
    m_weightUnitLabel=new QLabel("g");
    m_statusLabel=new QLabel("状态");
    m_statusReadyRadio=new QRadioButton("待测");
    m_statusStopDetectRadio=new QRadioButton("停测");

    m_whichBalanceLabel = new QLabel("序列样品之间：");
    m_detectOverBalanceRadio = new QRadioButton("在测完瓶位平衡");
    m_backToZeroBalanceRadio = new QRadioButton("回0位平衡");
    m_directNextRadio = new QRadioButton("直接下一个");
    m_detectOverBalanceRadio->setChecked(true);  // 默认选中“”
    
    // 创建按钮组
    QButtonGroup *group = new QButtonGroup(this);

    // 加入组中（可选指定 id）
    group->addButton(m_detectOverBalanceRadio, 0);
    group->addButton(m_backToZeroBalanceRadio, 1);
    group->addButton(m_directNextRadio, 2);


    m_layout->addWidget(m_sampleNameLabel,0,0);
    m_layout->addWidget(m_sampleNameEdit,0,1);
    m_layout->addWidget(m_blankBox,0,2);
    m_layout->addWidget(m_batchNoLabel,1,0);
    m_layout->addWidget(m_batchNoEdit,1,1);
    m_layout->addWidget(m_sampleNoLabel,2,0);
    m_layout->addWidget(m_sampleNoEdit,2,1);
    m_layout->addWidget(m_weightLabel,3,0);
    m_layout->addWidget(m_weightEdit,3,1);
    m_layout->addWidget(m_weightUnitLabel,3,2);
    m_layout->addWidget(m_getWeightBtn,3,3);
    m_layout->addWidget(m_statusLabel,4,0);
    m_layout->addWidget(m_statusReadyRadio,4,1);
    m_layout->addWidget(m_statusStopDetectRadio,4,2);
    m_layout->addWidget(m_testFlowLabel,5,0);
    m_layout->addWidget(m_testFlowCombo,5,1);
    m_layout->addWidget(m_testChannelLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_testChannelEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_circleNoLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_circleNoEdit,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_whichBalanceLabel,m_layout->rowCount(),0);
    m_layout->addWidget(m_detectOverBalanceRadio,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_backToZeroBalanceRadio,m_layout->rowCount()-1,2);
    m_layout->addWidget(m_directNextRadio,m_layout->rowCount()-1,3);
    m_layout->addWidget(m_rectifyBtn,m_layout->rowCount(),0);
    m_layout->addWidget(m_saveBtn,m_layout->rowCount()-1,1);
    m_layout->addWidget(m_createBtn,m_layout->rowCount()-1,2);
    #else
    m_sampleInputLabel=new QLabel("样品来源");
    m_getSampleMethodLabel=new QLabel("取样方式");

    // m_actionLabel=new QLabel("反应池");
    
    m_getSamplePumpLabel=new QLabel("取样管路");
    // m_testMethodLabel=new QLabel("测试方法");
    m_workModeLabel=new QLabel("工作模式");
    m_intervalLabel=new QLabel("间隔时间");

    m_sampleInputEdit=new QLineEdit();
    m_getSampleMethodCombo=new QComboBox();
    m_getSampleMethodCombo->addItems({"在线取样"});
    m_getSamplePumpCombo=new QComboBox();
    m_getSamplePumpCombo->addItems({""});

    m_targetSampleGroup=new QGroupBox("");
    m_targetSampleLayout=new QGridLayout();
    m_scrollArea=new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_targetSampleGroup);
    m_targetSampleGroup->setLayout(m_targetSampleLayout);
    m_addFactorWidgets.push_back(new HGAddFactorWidget(m_lang,int(m_addFactorWidgets.size())));
    m_targetSampleLayout->addWidget(m_addFactorWidgets[m_addFactorWidgets.size()-1],0,0);

    
    // m_testMethodCombo=new QComboBox();
    // names.clear();
    // names=getMethodNames();
    // for (int i=0;i<int(names.size());i++){
    //     m_testMethodCombo->addItem(QString::fromStdString(names[i]));
    // }
        
    m_workModeCombo=new QComboBox();
    // for (auto mode:taskWorkingMode){
    //     m_workModeCombo->addItem(QString::fromStdString(mode.second));
    // }
    m_intervalCombo=new QComboBox();
    connect(m_workModeCombo,&QComboBox::currentTextChanged,[=](QString str){
        if (str=="周期性测定") {
            m_intervalLabel->setText("间隔时间");
            // for (auto inter : taskInterval)
            // {
            //     m_intervalCombo->addItem(QString::fromStdString(inter.second));
            // }
        } else if (str=="循环测试") {
            m_intervalLabel->setText("循环次数");
        } else if (str=="定时测试"){
            m_intervalLabel->setText("测试时间");
        }
    });
    


    m_horline=new QFrame;
    m_horline->setFrameShape(QFrame::HLine);
    m_horline->setFrameShadow(QFrame::Sunken);
    m_horline1=new QFrame;
    m_horline1->setFrameShape(QFrame::HLine);
    m_horline1->setFrameShadow(QFrame::Sunken);
    m_horline2=new QFrame;
    m_horline2->setFrameShape(QFrame::HLine);
    m_horline2->setFrameShadow(QFrame::Sunken);

    
    m_addFactorLabel=new LabelWithImg(IMGRIGHT,12,getPath("/resources/V1/@1xze-add-o 1.png"),
        loadTranslation(m_lang,"AddElement"));//"添加目标成分");
    connect(m_addFactorLabel,SIGNAL(clickImgLabel()),this,SLOT(clickAddFactor()));

    m_layout->addWidget(m_sampleInputLabel,0,0,1,2);
    m_layout->addWidget(m_sampleInputEdit,0,1,1,1);
    m_layout->addWidget(m_sampleNameLabel,1,0,1,2);
    m_layout->addWidget(m_sampleNameEdit,1,1,1,1);
    m_layout->addWidget(m_getSampleMethodLabel,2,0,1,2);
    m_layout->addWidget(m_getSampleMethodCombo,2,1,1,1);

    m_layout->addWidget(m_scrollArea,3,0,1,4);
    m_layout->addWidget(m_addFactorLabel,4,0);
    m_layout->addWidget(m_horline,5,0,1,3);
    m_layout->addWidget(m_testChannelLabel,6,0);
    m_layout->addWidget(m_testChannelEdit,6,1);
    m_layout->addWidget(m_getSamplePumpLabel,7,0);
    m_layout->addWidget(m_getSamplePumpCombo,7,1);

    m_layout->addWidget(m_testFlowLabel,8,0);
    m_layout->addWidget(m_testFlowCombo,8,1);
    // m_layout->addWidget(m_testMethodLabel,14,0);
    // m_layout->addWidget(m_testMethodCombo,14,1);
    m_layout->addWidget(m_horline1,9,0,1,3);
    m_layout->addWidget(m_workModeLabel,10,0);
    m_layout->addWidget(m_workModeCombo,10,1);
    m_layout->addWidget(m_intervalLabel,11,0);
    m_layout->addWidget(m_intervalCombo,11,1);
    m_layout->addWidget(m_horline2,12,0,1,3);
    m_layout->addWidget(m_createBtn,13,0);
    m_layout->addWidget(m_rectifyBtn,13,1);
    m_layout->addWidget(m_saveBtn,13,2);
    
    #endif
    fnSetReadOnly(true);
}
bool HGTaskInfoWidget::closeWindow(){
    return true;
}
void HGTaskInfoWidget::fninitTaskData(){
    m_taskS.Clear();
    fnSetTaskData();
}
void HGTaskInfoWidget::clickAddFactor(){
    #ifdef ENABLE_CAS6
    #else
    int rowCount=m_targetSampleLayout->rowCount();
    m_addFactorWidgets.push_back(new HGAddFactorWidget(m_lang,int(m_addFactorWidgets.size())));
    m_targetSampleLayout->addWidget(m_addFactorWidgets[m_addFactorWidgets.size()-1],rowCount+0,0);
    #endif
}
void HGTaskInfoWidget::fnSetTaskData(){
    m_sampleNameEdit->setText(QString::fromStdString(m_taskS.sampleName));
    m_testFlowCombo->setCurrentText(QString::fromStdString(m_taskS.testFlow));
    m_testChannelEdit->setValue(m_taskS.testChannel);

    #ifdef ENABLE_CAS6
    m_batchNoEdit->setText(QString::fromStdString(m_taskS.batchNo));
    m_sampleNoEdit->setText(QString::fromStdString(m_taskS.serailNo));
    m_weightEdit->setText(QString::number(m_taskS.content));
    if (m_taskS.detectStatus=="ready") m_statusReadyRadio->setChecked(true);
    else if (m_taskS.detectStatus=="stop") m_statusStopDetectRadio->setChecked(true);
    m_circleNoEdit->setValue(m_taskS.circleNo);
    if (m_taskS.blank=="enable") m_blankBox->setChecked(true);
    else m_blankBox->setChecked(false);

    switch (m_taskS.sampleDetectStrategy) {
        case 0:
        m_detectOverBalanceRadio->setChecked(true);
        break;
        case 1:
        m_backToZeroBalanceRadio->setChecked(true);
        break;
        case 2:
        m_directNextRadio->setChecked(true);
        break;
        default: 
        m_detectOverBalanceRadio->setChecked(true);
        break;
    }

    #else
    m_sampleInputEdit->setText(QString::fromStdString(m_taskS.sampleInput));
    m_getSampleMethodCombo->setCurrentText(QString::fromStdString(m_taskS.getSampleMethod));
    
    // m_testMethodCombo->setCurrentText(QString::fromStdString(m_taskS.testMethod));
    m_workModeCombo->setCurrentIndex(m_taskS.workingMode);
    m_intervalCombo->setCurrentIndex(m_taskS.interval);
    m_getSamplePumpCombo->setCurrentText(QString::fromStdString(m_taskS.getSamplePump));
    for (int i=0;i<int(m_addFactorWidgets.size());i++){
        m_addFactorWidgets[i]->fnSetTaskData(m_taskS);
    }
    #endif
}
void HGTaskInfoWidget::clickCreateTask(){
    m_editType=TASK_CREATE;
    fnSetReadOnly(false);
    fninitTaskData();
    
}
void HGTaskInfoWidget::fnSetReadOnly(bool flag){
    m_sampleNameEdit->setReadOnly(flag);
    m_testFlowCombo->setEnabled(!flag);
    if (flag){
        m_testFlowCombo->setFocusPolicy(Qt::NoFocus);
    } else {
        m_testFlowCombo->setFocusPolicy(Qt::StrongFocus);
    }
    m_testChannelEdit->setReadOnly(flag);


    #ifdef ENABLE_CAS6
    m_batchNoEdit->setReadOnly(flag);
    m_sampleNoEdit->setReadOnly(flag);
    m_weightEdit->setReadOnly(flag);
    m_statusReadyRadio->setEnabled(!flag);
    m_statusStopDetectRadio->setEnabled(!flag);
    m_blankBox->setEnabled(!flag);
    m_circleNoEdit->setEnabled(!flag);
    #else
    m_sampleInputEdit->setReadOnly(flag);
    
    // m_testMethodCombo->setEnabled(!flag);
    m_workModeCombo->setEnabled(!flag);
    m_intervalCombo->setEnabled(!flag);
    for (int i=0;i<int(m_addFactorWidgets.size());i++){
        m_addFactorWidgets[i]->fnSetReadOnly(flag);
    }
    if (flag){
        m_getSampleMethodCombo->setFocusPolicy(Qt::NoFocus);
        // m_testMethodCombo->setFocusPolicy(Qt::NoFocus);
        m_workModeCombo->setFocusPolicy(Qt::NoFocus);
        m_intervalCombo->setFocusPolicy(Qt::NoFocus);
        m_getSamplePumpCombo->setFocusPolicy(Qt::NoFocus);
    } else {
        m_getSampleMethodCombo->setFocusPolicy(Qt::StrongFocus);
        // m_testMethodCombo->setFocusPolicy(Qt::StrongFocus);
        m_workModeCombo->setFocusPolicy(Qt::StrongFocus);
        m_intervalCombo->setFocusPolicy(Qt::StrongFocus);
        m_getSamplePumpCombo->setFocusPolicy(Qt::StrongFocus);
    }
    #endif
}
void HGTaskInfoWidget::clickRectifyTask(){
    fnSetReadOnly(false);
    m_editType=TASK_RECTIFY;
}
void HGTaskInfoWidget::clickSaveTask(){
    if (m_editType==TASK_NOEDIT) return;
    fnUpdateTaskInfo();
    fnSetReadOnly(true);
    // display
    fnDisplayDB();
    m_editType=TASK_NOEDIT;

}
void HGTaskInfoWidget::fnDisplayDB(){
    emit updateTaskInfo(m_editType,m_taskS);
}
void HGTaskInfoWidget::fnUpdateTaskInfo(){
    m_taskS.sampleName=m_sampleNameEdit->text().toStdString();
    m_taskS.testFlow=m_testFlowCombo->currentText().toStdString();
    m_taskS.testChannel=m_testChannelEdit->value();
    m_taskS.testMethod=RWDb::getMethodName(m_taskS.testFlow);


    #ifdef ENABLE_CAS6
    m_taskS.batchNo=m_batchNoEdit->text().toStdString();
    m_taskS.serailNo=m_sampleNoEdit->text().toStdString();
    m_taskS.content=m_weightEdit->text().toDouble();
    if (m_statusReadyRadio->isChecked()) m_taskS.detectStatus="ready";
    else if (m_statusStopDetectRadio->isChecked()) m_taskS.detectStatus="stop";
    m_taskS.circleNo=m_circleNoEdit->value();
    m_taskS.blank=m_blankBox->isChecked()?"enable":"disable";

    if (m_detectOverBalanceRadio->isChecked()) m_taskS.sampleDetectStrategy=0;
    if (m_backToZeroBalanceRadio->isChecked()) m_taskS.sampleDetectStrategy=1;
    if (m_directNextRadio->isChecked()) m_taskS.sampleDetectStrategy=2;
    
    #else
    m_taskS.sampleInput=m_sampleInputEdit->text().toStdString();
    m_taskS.getSampleMethod=m_getSampleMethodCombo->currentText().toStdString();
    
    // m_taskS.testMethod=m_testMethodCombo->currentText().toStdString();
    
    m_taskS.workingMode=m_workModeCombo->currentIndex();
    m_taskS.interval=m_intervalCombo->currentIndex();
    m_taskS.getSamplePump=m_getSamplePumpCombo->currentText().toStdString();
    for (int i=0;i<int(m_addFactorWidgets.size());i++){
        Task info;
        m_addFactorWidgets[i]->fnUpdateTaskInfo(info);
        m_taskS.targetElement=info.targetElement;
        m_taskS.content=info.content;
        m_taskS.unit=info.unit;
        m_taskS.standard1Condition=info.standard1Condition;
        m_taskS.standard1=info.standard1;
        m_taskS.standard2Condition=info.standard2Condition;
        m_taskS.standard2=info.standard2;
    }
    #endif
}
void HGTaskInfoWidget::displayTaskInfo(Task info){
    m_taskS=info;
    fnSetTaskData();
}