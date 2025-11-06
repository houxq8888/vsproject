#include "hgflowinfowidget.h"
#include "common.h"

HGFlowInfoWidget::HGFlowInfoWidget(std::string lang,QWidget *parent) : QWidget(parent),
m_lang(lang)
{
    m_steps.clear();
    m_toolBox=new QToolBox();
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
    connect(m_toolBox,SIGNAL(currentChanged(int)),this,SLOT(togglePage(int)));
    m_toolBox->installEventFilter(this);

    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_layout->addWidget(m_toolBox,0,0,1,3);

    m_lastIndex=-1;
}

bool HGFlowInfoWidget::closeWindow()
{
    for (auto &widget : m_blankW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_detectW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_getsampleW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_holdSampleW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_injectReagentW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_washW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_waitW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_addMedicationW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_loopW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_judgeW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_balanceW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget : m_injectSampleW)
    {
        if (widget.second)
        {
            if (widget.second->closeWindow())
            {
                SAFE_DELETE(widget.second);
            }
        }
    }
    for (auto &widget:m_swirlW){
        if (widget.second){
            if (widget.second->closeWindow()){
                SAFE_DELETE(widget.second);
            }
        }
    }
    return true;
}
#include <QDebug>
bool HGFlowInfoWidget::eventFilter(QObject *obj, QEvent *event) 
{
    if (obj == m_toolBox)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                // 获取当前点击的项
                // int index = m_toolBox->indexAt(mouseEvent->pos());
                int index=m_toolBox->currentIndex();
                if (index != -1)
                {
                    qDebug() << "Item clicked, index:" << index;

                    // 你可以根据 index 做其他操作
                    if (m_toolBox->currentIndex() == index)
                    {
                        qDebug() << "This item is already expanded, you clicked again.";
                    }
                    else
                    {
                        qDebug() << "This item was collapsed before.";
                    }
                }
            }
            return true; // 事件已处理
        }
    }

    return QWidget::eventFilter(obj, event); // 其他事件交给基类处理
}
void HGFlowInfoWidget::togglePage(int index){
    int cnt=m_toolBox->count();
    for (int i=0;i<cnt;i++){
        if (i==index) m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow-down 1.png"))));
        else m_toolBox->setItemIcon(i,QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))));
    }
    // if (m_lastIndex == index)
    // {
    //     m_toolBox->setCurrentIndex(-1); // 设置为 -1 来收起当前项
    //     m_lastIndex = -1;               // 重置当前展开项
    // }
    // else
    // {
    //     m_lastIndex = index; // 更新当前展开项
    // }
}
void HGFlowInfoWidget::showFlow(FlowOfTask info){
    while (m_toolBox->count() > 0){
        m_toolBox->removeItem(0);
    }
    m_steps.clear();
    std::vector<StepOfFlow> steps=info.steps;
    for (const auto &step:steps){
        createModule(step.name,step.param);
    }
}
std::vector<StepOfFlow> HGFlowInfoWidget::getStepOfFlow() {
    for (int i = 0; i < int(m_steps.size()); i++)
    {
        std::string name = m_steps[i].name;
        int index=m_steps[i].indexOfSameStep;
        if ("打空白" == name)
        {
            m_steps[i].param = m_blankW[index]->getParamStr();
        }
        else if ("测定" == name)
        {
            m_steps[i].param = m_detectW[index]->getParamStr();
        }
        else if ("取样" == name)
        {
            m_steps[i].param = m_getsampleW[index]->getParamStr();
        }
        else if ("留样" == name)
        {
            m_steps[i].param = m_holdSampleW[index]->getParamStr();
        }
        else if ("注入试剂" == name)
        {
            m_steps[i].param = m_injectReagentW[index]->getParamStr();
        }
        else if ("清洗" == name)
        {
            m_steps[i].param = m_washW[index]->getParamStr();
        }
        else if ("循环" == name)
        {
            m_steps[i].param = m_loopW[index]->getParamStr();
        }
        else if ("判定" == name)
        {
            m_steps[i].param = m_judgeW[index]->getParamStr();
        }
        else if ("等待" == name)
        {
            m_steps[i].param = m_waitW[index]->getParamStr();
        }
        else if ("加药" == name)
        {
            m_steps[i].param = m_addMedicationW[index]->getParamStr();
        } else if ("平衡" == name){
            m_steps[i].param=m_balanceW[index]->getParamStr();
        } else if ("进样" == name){
            m_steps[i].param=m_injectSampleW[index]->getParamStr();
        } else if ("搅拌" == name){
            m_steps[i].param=m_swirlW[index]->getParamStr();
        }
    }
    return m_steps;
}
void HGFlowInfoWidget::createModule(std::string name,std::string param){
    StepOfFlow step;
    step.name=name;
    step.index=m_steps.size()+1;
    if ("打空白"==name){
        step.nameOfSameStep=name+std::to_string(m_blankW.size());
        step.indexOfSameStep=int(m_blankW.size());
        m_blankW[step.indexOfSameStep]=new HGblankWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_blankW[step.indexOfSameStep],
            QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),
            QString::fromStdString(name));
        m_blankW[step.indexOfSameStep]->setParamStr(param);
        connect(m_blankW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));

    } else if ("测定"==name){
        step.nameOfSameStep=name+std::to_string(m_detectW.size());
        step.indexOfSameStep=int(m_detectW.size());
        m_detectW[step.indexOfSameStep]=new HGDetectWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_detectW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_detectW[step.indexOfSameStep]->setParamStr(param);
        connect(m_detectW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("取样"==name){
        step.nameOfSameStep=name+std::to_string(m_getsampleW.size());
        step.indexOfSameStep=int(m_getsampleW.size());
        m_getsampleW[step.indexOfSameStep]=new HGGetSampleWidget(m_lang,param,step.nameOfSameStep);
        m_toolBox->addItem(m_getsampleW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_getsampleW[step.indexOfSameStep]->setParamStr(param);
        connect(m_getsampleW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("留样"==name){
        step.nameOfSameStep=name+std::to_string(m_holdSampleW.size());
        step.indexOfSameStep=int(m_holdSampleW.size());
        m_holdSampleW[step.indexOfSameStep]=new HGHoldSampleWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_holdSampleW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_holdSampleW[step.indexOfSameStep]->setParamStr(param);
        connect(m_holdSampleW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("注入试剂"==name){
        step.nameOfSameStep=name+std::to_string(m_injectReagentW.size());
        step.indexOfSameStep=int(m_injectReagentW.size());
        m_injectReagentW[step.indexOfSameStep]=new HGinjectReagentWidget(m_lang,param,step.nameOfSameStep);
        m_toolBox->addItem(m_injectReagentW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_injectReagentW[step.indexOfSameStep]->setParamStr(param);
        connect(m_injectReagentW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("清洗"==name){
        step.nameOfSameStep=name+std::to_string(m_washW.size());
        step.indexOfSameStep=int(m_washW.size());
        m_washW[step.indexOfSameStep]=new HGWashWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_washW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_washW[step.indexOfSameStep]->setParamStr(param);
        connect(m_washW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("循环"==name){
        step.nameOfSameStep=name+std::to_string(m_loopW.size());
        step.indexOfSameStep=int(m_loopW.size());
        m_loopW[step.indexOfSameStep]=new HGLoopWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_loopW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_loopW[step.indexOfSameStep]->setParamStr(param);
        connect(m_loopW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("判定"==name){
        step.nameOfSameStep=name+std::to_string(m_judgeW.size());
        step.indexOfSameStep=int(m_judgeW.size());
        m_judgeW[step.indexOfSameStep]=new HGJudgeWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_judgeW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_judgeW[step.indexOfSameStep]->setParamStr(param);
        connect(m_judgeW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    }  else if ("等待"==name){
        step.nameOfSameStep=name+std::to_string(m_waitW.size());
        step.indexOfSameStep=int(m_waitW.size());
        m_waitW[step.indexOfSameStep]=new HGWaitWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_waitW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_waitW[step.indexOfSameStep]->setParamStr(param);
        connect(m_waitW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    }  else if ("加药"==name){
        step.nameOfSameStep=name+std::to_string(m_addMedicationW.size());
        step.indexOfSameStep=int(m_addMedicationW.size());
        m_addMedicationW[step.indexOfSameStep]=new HGAddMedicationWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_addMedicationW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_addMedicationW[step.indexOfSameStep]->setParamStr(param);
        connect(m_addMedicationW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    } else if ("进样"==name){
        step.nameOfSameStep=name+std::to_string(m_injectSampleW.size());
        step.indexOfSameStep=int(m_injectSampleW.size());
        m_injectSampleW[step.indexOfSameStep]=new HGInjectSampleWidget(m_lang,param,step.nameOfSameStep);
        m_toolBox->addItem(m_injectSampleW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_injectSampleW[step.indexOfSameStep]->setParamStr(param);
        connect(m_injectSampleW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));

    } else if ("平衡"==name){
        step.nameOfSameStep=name+std::to_string(m_balanceW.size());
        step.indexOfSameStep=int(m_balanceW.size());
        m_balanceW[step.indexOfSameStep]=new HGBalanceWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_balanceW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_balanceW[step.indexOfSameStep]->setParamStr(param);
        connect(m_balanceW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));

    } else if ("搅拌"==name){
        step.nameOfSameStep=name+std::to_string(m_swirlW.size());
        step.indexOfSameStep=int(m_swirlW.size());
        m_swirlW[step.indexOfSameStep]=new HGSwirlStepWidget(m_lang,step.nameOfSameStep);
        m_toolBox->addItem(m_swirlW[step.indexOfSameStep],QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))),QString::fromStdString(name));
        m_swirlW[step.indexOfSameStep]->setParamStr(param);
        connect(m_swirlW[step.indexOfSameStep],SIGNAL(mainpuateObject(std::string,int)),
            this,SLOT(slotManipulateObject(std::string,int)));
    }

    m_steps.push_back(step);

    // 添加自定义控件到项标题

    // m_manipulateW=new HGManipulateWidget(MANIPULATE_ALL,step.nameOfSameStep);
    // connect(m_manipulateW,&HGManipulateWidget::manipulateObject,
    //     [=](std::string name,int type){
    //         // emit mainpuateObject(name,type);
    // });
    // m_toolBox->setItemWidget(m_toolBox->count()-1, m_manipulateW); // 为第一个项的标题设置控件
    
}
template<typename V>
void swapKeysSafely(std::map<int, V*>& myMap, int key1, int key2) {
    if (myMap.find(key1) == myMap.end() || myMap.find(key2) == myMap.end()) {
        printf("One or both keys not found in the map.\n");
        return;
    }

    V *value1 = myMap[key1];
    V *value2 = myMap[key2];

    // 使用临时键
    int tempKey = -1;
    while (myMap.find(tempKey) != myMap.end()) {
        --tempKey; // 确保临时键不与现有键冲突
    }

    myMap[tempKey] = value1;
    myMap.erase(key1);
    myMap[key1] = value2;
    myMap.erase(key2);
    myMap[key2] = myMap[tempKey];
    myMap.erase(tempKey);
}
void HGFlowInfoWidget::swapItem(int preIndex,int curIndex)
{
    for (int i=0;i<int(m_steps.size());i++){
        std::string typeName=m_steps[i].name;
        if ("打空白" == typeName)
        {
            m_steps[i].param=m_blankW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("测定" == typeName)
        {
            m_steps[i].param=m_detectW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("取样" == typeName)
        {
            m_steps[i].param=m_getsampleW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("留样" == typeName)
        {
            m_steps[i].param=m_holdSampleW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("注入试剂" == typeName)
        {
            m_steps[i].param=m_injectReagentW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("清洗" == typeName)
        {
            m_steps[i].param=m_washW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("循环" == typeName)
        {
            m_steps[i].param=m_loopW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("判定" == typeName)
        {
            m_steps[i].param=m_judgeW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("等待" == typeName)
        {
            m_steps[i].param=m_waitW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("加药" == typeName)
        {
            m_steps[i].param = m_addMedicationW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("进样" == typeName)
        {
            m_steps[i].param = m_injectSampleW[m_steps[i].indexOfSameStep]->getParamStr();
        }
        else if ("平衡" == typeName)
        {
            m_steps[i].param = m_balanceW[m_steps[i].indexOfSameStep]->getParamStr();
        } else if ("搅拌"==typeName){
            m_steps[i].param=m_swirlW[m_steps[i].indexOfSameStep]->getParamStr();
        }
    }
    closeWindow();
    for (int i = m_toolBox->count() - 1; i >= 0; --i)
    {
        m_toolBox->removeItem(i);
    }
    // 1 name name0 0
    // 2 type type0 0
    // 1 name name0 0
    // 2 name name1 1
    if (m_steps[preIndex].name == m_steps[curIndex].name)
    {
        std::string tempname=m_steps[preIndex].param;
        m_steps[preIndex].param = m_steps[curIndex].param;
        m_steps[preIndex + 1].param = tempname;
    }
    else
    {
        int tempIndex = m_steps[preIndex].indexOfSameStep;
        m_steps[preIndex].indexOfSameStep = m_steps[curIndex].indexOfSameStep;
        m_steps[preIndex + 1].indexOfSameStep = tempIndex;
        std::string tempname=m_steps[preIndex].name;
        m_steps[preIndex].name = m_steps[curIndex].name;
        m_steps[preIndex + 1].name = tempname;
        tempname=m_steps[preIndex].nameOfSameStep;
        m_steps[preIndex].nameOfSameStep = m_steps[curIndex].nameOfSameStep;
        m_steps[preIndex + 1].nameOfSameStep = tempname;
        tempname=m_steps[preIndex].param;
        m_steps[preIndex].param = m_steps[curIndex].param;
        m_steps[preIndex + 1].param = tempname;

    }
    for (int i=0;i<int(m_steps.size());i++){
        std::string name=m_steps[i].name;
        int indexOfSameStep = m_steps[i].indexOfSameStep;
        std::string nameOfSameStep= m_steps[i].nameOfSameStep;
        if ("打空白" == name)
        {
            m_blankW[indexOfSameStep]=new HGblankWidget(m_lang,nameOfSameStep);
            m_blankW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_blankW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_blankW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("测定" == name)
        {
            m_detectW[indexOfSameStep]=new HGDetectWidget(m_lang,nameOfSameStep);
            m_detectW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_detectW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_detectW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("取样" == name)
        {
            m_getsampleW[indexOfSameStep]=new HGGetSampleWidget(m_lang,m_steps[i].param,nameOfSameStep);
            m_getsampleW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_getsampleW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_getsampleW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("留样" == name)
        {
            m_holdSampleW[indexOfSameStep]=new HGHoldSampleWidget(m_lang,nameOfSameStep);
            m_holdSampleW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_holdSampleW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_holdSampleW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("注入试剂" == name)
        {
            m_injectReagentW[indexOfSameStep]=new HGinjectReagentWidget(m_lang,m_steps[i].param,nameOfSameStep);
            m_injectReagentW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_injectReagentW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_injectReagentW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("清洗" == name)
        {
            m_washW[indexOfSameStep]=new HGWashWidget(m_lang,nameOfSameStep);
            m_washW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_washW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_washW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("循环" == name)
        {
            m_loopW[indexOfSameStep]=new HGLoopWidget(m_lang,nameOfSameStep);
            m_loopW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_loopW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_loopW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("判定" == name)
        {
            m_judgeW[indexOfSameStep]=new HGJudgeWidget(m_lang,nameOfSameStep);
            m_judgeW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_judgeW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_judgeW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("等待" == name)
        {
            m_waitW[indexOfSameStep]=new HGWaitWidget(m_lang,nameOfSameStep);
            m_waitW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_waitW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_waitW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("加药" == name)
        {
            m_addMedicationW[indexOfSameStep]=new HGAddMedicationWidget(m_lang,nameOfSameStep);
            m_addMedicationW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_addMedicationW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_addMedicationW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("进样" == name)
        {
            m_injectSampleW[indexOfSameStep]=new HGInjectSampleWidget(m_lang,m_steps[i].param,nameOfSameStep);
            m_injectSampleW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_injectSampleW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_injectSampleW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
        else if ("平衡" == name)
        {
            m_balanceW[indexOfSameStep]=new HGBalanceWidget(m_lang,nameOfSameStep);
            m_balanceW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_balanceW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_balanceW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        } else if ("搅拌"==name){
            m_swirlW[indexOfSameStep]=new HGSwirlStepWidget(m_lang,nameOfSameStep);
            m_swirlW[indexOfSameStep]->setParamStr(m_steps[i].param);
            m_toolBox->addItem(m_swirlW[indexOfSameStep], QIcon(QString::fromStdString(getPath("/resources/V1/@1xze-arrow 1.png"))), QString::fromStdString(name));
            connect(m_swirlW[indexOfSameStep], SIGNAL(mainpuateObject(std::string, int)),
                    this, SLOT(slotManipulateObject(std::string, int)));
        }
    }
}
void HGFlowInfoWidget::slotManipulateObject(std::string name,int type){
    int currentIndex=m_toolBox->currentIndex();
    
    switch (type){
        case MANIPULATE_DOWN:
        {
            if (currentIndex < 0 || currentIndex>=m_toolBox->count()-1){
                return;
            }
            swapItem(currentIndex,currentIndex+1);
            break;
        }
        case MANIPULATE_UP:
        {
            if (currentIndex <=0 || currentIndex>=m_toolBox->count()){
                return;
            }
            swapItem(currentIndex-1,currentIndex);
            break;
        }
        case MANIPULATE_CLOSE:{
            int indexOfSameStep=-1,index=-1;
            std::string typeName="";
            m_toolBox->removeItem(currentIndex);
            for (int i=0;i<int(m_steps.size());i++){
                if (m_steps[i].nameOfSameStep==name){
                    indexOfSameStep=m_steps[i].indexOfSameStep;
                    index=m_steps[i].index;
                    typeName=m_steps[i].name;
                    m_steps.erase(m_steps.begin()+i);
                    break;
                }
            }
            
            if (indexOfSameStep==-1) return;
            for (int i=0;i<int(m_steps.size());i++){
                if (name.find(m_steps[i].name)!=std::string::npos){
                    if (m_steps[i].indexOfSameStep > indexOfSameStep)
                    {
                        m_steps[i].indexOfSameStep-=1;
                        m_steps[i].nameOfSameStep=m_steps[i].name+std::to_string(m_steps[i].indexOfSameStep);
                    } 
                }
                if (m_steps[i].index > index){
                    m_steps[i].index-=1;
                }
            }
            // delete map
            if ("打空白"==typeName){
                auto it=m_blankW.find(indexOfSameStep);
                if (it!=m_blankW.end()){
                    m_blankW.erase(it);
                }
                for (auto w:m_blankW){
                    if (w.first > indexOfSameStep){
                        HGblankWidget *blankW=w.second;
                        auto it=m_blankW.find(w.first);
                        if (it!=m_blankW.end()){
                            m_blankW.erase(it);
                            m_blankW[w.first-1]=blankW;
                        }
                    }
                }
            }else if ("测定"==typeName){
                auto it=m_detectW.find(indexOfSameStep);
                if (it!=m_detectW.end()){
                    m_detectW.erase(it);
                }
                for (auto w:m_detectW){
                    if (w.first > indexOfSameStep){
                        HGDetectWidget *detectW=w.second;
                        auto it=m_detectW.find(w.first);
                        if (it!=m_detectW.end()){
                            m_detectW.erase(it);
                            m_detectW[w.first-1]=detectW;
                        }
                    }
                }
            
            } else if ("取样"==typeName){
                auto it=m_getsampleW.find(indexOfSameStep);
                if (it!=m_getsampleW.end()){
                    m_getsampleW.erase(it);
                }
                for (auto w:m_getsampleW){
                    if (w.first > indexOfSameStep){
                        HGGetSampleWidget *getsampleW=w.second;
                        auto it=m_getsampleW.find(w.first);
                        if (it!=m_getsampleW.end()){
                            m_getsampleW.erase(it);
                            m_getsampleW[w.first-1]=getsampleW;
                        }
                    }
                }
            
            } else if ("留样"==typeName){
                auto it=m_holdSampleW.find(indexOfSameStep);
                if (it!=m_holdSampleW.end()){
                    m_holdSampleW.erase(it);
                }
                for (auto w:m_holdSampleW){
                    if (w.first > indexOfSameStep){
                        HGHoldSampleWidget *holdsampleW=w.second;
                        auto it=m_holdSampleW.find(w.first);
                        if (it!=m_holdSampleW.end()){
                            m_holdSampleW.erase(it);
                            m_holdSampleW[w.first-1]=holdsampleW;
                        }
                    }
                }
            
            } else if ("注入试剂"==typeName){
                auto it=m_injectReagentW.find(indexOfSameStep);
                if (it!=m_injectReagentW.end()){
                    m_injectReagentW.erase(it);
                }
                for (auto w:m_injectReagentW){
                    if (w.first > indexOfSameStep){
                        HGinjectReagentWidget *injectReagentW=w.second;
                        auto it=m_injectReagentW.find(w.first);
                        if (it!=m_injectReagentW.end()){
                            m_injectReagentW.erase(it);
                            m_injectReagentW[w.first-1]=injectReagentW;
                        }
                    }
                }
            
            } else if ("清洗"==typeName){
                auto it=m_washW.find(indexOfSameStep);
                if (it!=m_washW.end()){
                    m_washW.erase(it);
                }
                for (auto w:m_washW){
                    if (w.first > indexOfSameStep){
                        HGWashWidget *washW=w.second;
                        auto it=m_washW.find(w.first);
                        if (it!=m_washW.end()){
                            m_washW.erase(it);
                            m_washW[w.first-1]=washW;
                        }
                    }
                }
            
            } else if ("循环"==typeName){
                auto it=m_loopW.find(indexOfSameStep);
                if (it!=m_loopW.end()){
                    m_loopW.erase(it);
                }
                for (auto w:m_loopW){
                    if (w.first > indexOfSameStep){
                        HGLoopWidget *loopW=w.second;
                        auto it=m_loopW.find(w.first);
                        if (it!=m_loopW.end()){
                            m_loopW.erase(it);
                            m_loopW[w.first-1]=loopW;
                        }
                    }
                }
            
            } else if ("判定"==typeName){
                auto it=m_judgeW.find(indexOfSameStep);
                if (it!=m_judgeW.end()){
                    m_judgeW.erase(it);
                }
                for (auto w:m_judgeW){
                    if (w.first > indexOfSameStep){
                        HGJudgeWidget *judgeW=w.second;
                        auto it=m_judgeW.find(w.first);
                        if (it!=m_judgeW.end()){
                            m_judgeW.erase(it);
                            m_judgeW[w.first-1]=judgeW;
                        }
                    }
                }
            
            }  else if ("等待"==typeName){
                auto it=m_waitW.find(indexOfSameStep);
                if (it!=m_waitW.end()){
                    m_waitW.erase(it);
                }
                for (auto w:m_waitW){
                    if (w.first > indexOfSameStep){
                        HGWaitWidget *waitW=w.second;
                        auto it=m_waitW.find(w.first);
                        if (it!=m_waitW.end()){
                            m_waitW.erase(it);
                            m_waitW[w.first-1]=waitW;
                        }
                    }
                }
            
            }  else if ("加药"==typeName){
                auto it=m_addMedicationW.find(indexOfSameStep);
                if (it!=m_addMedicationW.end()){
                    m_addMedicationW.erase(it);
                }
                for (auto w:m_addMedicationW){
                    if (w.first > indexOfSameStep){
                        HGAddMedicationWidget *addMedicationW=w.second;
                        auto it=m_addMedicationW.find(w.first);
                        if (it!=m_addMedicationW.end()){
                            m_addMedicationW.erase(it);
                            m_addMedicationW[w.first-1]=addMedicationW;
                        }
                    }
                }
            }
            else if ("进样" == name)
            {
                auto it=m_injectSampleW.find(indexOfSameStep);
                if (it!=m_injectSampleW.end()){
                    m_injectSampleW.erase(it);
                }
                for (auto w:m_injectSampleW){
                    if (w.first > indexOfSameStep){
                        HGInjectSampleWidget *injectSampleW=w.second;
                        auto it=m_injectSampleW.find(w.first);
                        if (it!=m_injectSampleW.end()){
                            m_injectSampleW.erase(it);
                            m_injectSampleW[w.first-1]=injectSampleW;
                        }
                    }
                }
            }
            else if ("平衡" == name)
            {
                auto it=m_balanceW.find(indexOfSameStep);
                if (it!=m_balanceW.end()){
                    m_balanceW.erase(it);
                }
                for (auto w:m_balanceW){
                    if (w.first > indexOfSameStep){
                        HGBalanceWidget *balanceW=w.second;
                        auto it=m_balanceW.find(w.first);
                        if (it!=m_balanceW.end()){
                            m_balanceW.erase(it);
                            m_balanceW[w.first-1]=balanceW;
                        }
                    }
                }
            } else if ("搅拌"==name){
                auto it=m_swirlW.find(indexOfSameStep);
                if (it!=m_swirlW.end()){
                    m_swirlW.erase(it);
                }
                for (auto w:m_swirlW){
                    if (w.first > indexOfSameStep){
                        HGSwirlStepWidget *swirlW=w.second;
                        auto it=m_swirlW.find(w.first);
                        if (it!=m_swirlW.end()){
                            m_swirlW.erase(it);
                            m_swirlW[w.first-1]=swirlW;
                        }
                    }
                }
            }

            break;
        }
    }
}