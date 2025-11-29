#include "languagewidget.h"
#include <QMessageBox>
#include <QTranslator>
#include <QApplication>
#include "common.h"


enum {
    LANGUAGE_CHINESE,
    LANGUAGE_ENGLISH
};

LanguageWidget::LanguageWidget(QWidget *parent) : BaseWidget(parent)
{ 
    m_layout=new QGridLayout();
    this->setLayout(m_layout);

    m_languageLabel=new QLabel(("显示语言"));
    m_languageComboBox=new QComboBox();
    m_languageComboBox->addItem("简体中文","zh");
    m_languageComboBox->addItem("English","en");
    connect(m_languageComboBox, &QComboBox::currentIndexChanged, this, 
        &LanguageWidget::switchLanguage);

    m_layout->addWidget(m_languageLabel,0,0);
    m_layout->addWidget(m_languageComboBox,0,1);
    fnReadDB();
}

LanguageWidget::~LanguageWidget(){
    closeWindow();
}
bool LanguageWidget::closeWindow()
{
    fnWriteDB();
    return true;
}
void LanguageWidget::fnReadDB()
{
    if (GlobalSingleton::instance().getSystemInfo("显示语言")!=""){
        m_languageComboBox->setCurrentText(QString::fromStdString(GlobalSingleton::instance().getSystemInfo("显示语言")));
    } else m_languageComboBox->setCurrentIndex(-1);
}
void LanguageWidget::fnWriteDB()
{
    GlobalSingleton::instance().setSystemInfo("显示语言", m_languageComboBox->currentText().toStdString());
}
void LanguageWidget::switchLanguage(int index)
{
    QString lang = qobject_cast<QComboBox *>(sender())->itemData(index).toString(); // 获取选择的语言
    switch (index){
        case LANGUAGE_CHINESE:
        m_languageLabel->setText("显示语言");
        RWDb::writeAuditTrailLog("切换语言为简体中文");
        emit updateLanguage("zh");
        break;
        case LANGUAGE_ENGLISH:
        m_languageLabel->setText("Language");
        RWDb::writeAuditTrailLog("切换语言为English");
        emit updateLanguage("en");
        break;
        default: break;
    }
}
void LanguageWidget::retranslateUi()
{
    // 更新所有需要翻译的文本
    findChild<QLabel *>("显示语言")->setText(tr("显示语言"));
}