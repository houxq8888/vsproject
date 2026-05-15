#include "RangeConfigDialog.h"
#include <QMessageBox>
#include <QDoubleValidator>

#define ADMIN_PASSWORD "admin"

RangeConfigDialog::RangeConfigDialog(const QString& itemName, const Range& currentRange, QWidget *parent)
    : QDialog(parent), m_itemName(itemName), m_originalName(itemName), m_range(currentRange)
{
    setupUI();
    setWindowTitle("配置范围 - " + itemName);
    setModal(true);
}

RangeConfigDialog::~RangeConfigDialog()
{
}

void RangeConfigDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* groupBox = new QGroupBox("设置参数配置", this);
    QFormLayout* formLayout = new QFormLayout(groupBox);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setText(m_itemName);
    m_nameEdit->setPlaceholderText("参数名称");
    connect(m_nameEdit, &QLineEdit::textChanged, this, &RangeConfigDialog::onNameChanged);

    m_minEdit = new QLineEdit(this);
    m_minEdit->setText(QString::number(m_range.min));
    m_minEdit->setValidator(new QDoubleValidator(this));
    m_minEdit->setPlaceholderText("最小值");

    m_maxEdit = new QLineEdit(this);
    m_maxEdit->setText(QString::number(m_range.max));
    m_maxEdit->setValidator(new QDoubleValidator(this));
    m_maxEdit->setPlaceholderText("最大值");

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("修改参数名称需要输入密码");
    m_passwordEdit->setVisible(false);

    m_passwordLabel = new QLabel("管理员密码:", this);
    m_passwordLabel->setVisible(false);

    formLayout->addRow("参数名称:", m_nameEdit);
    formLayout->addRow("最小值:", m_minEdit);
    formLayout->addRow("最大值:", m_maxEdit);
    formLayout->addRow(m_passwordLabel, m_passwordEdit);

    mainLayout->addWidget(groupBox);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_okButton = new QPushButton("确定", this);
    m_cancelButton = new QPushButton("取消", this);

    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &RangeConfigDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &RangeConfigDialog::onCancelClicked);

    setLayout(mainLayout);
}

bool RangeConfigDialog::validateRange()
{
    QString newName = m_nameEdit->text().trimmed();
    if (newName.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "参数名称不能为空！");
        return false;
    }
    
    bool ok1, ok2;
    float min = m_minEdit->text().toFloat(&ok1);
    float max = m_maxEdit->text().toFloat(&ok2);

    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "输入错误", "请输入有效的数字！");
        return false;
    }

    if (min >= max) {
        QMessageBox::warning(this, "输入错误", "最小值必须小于最大值！");
        return false;
    }

    if (newName != m_originalName) {
        if (!validatePassword()) {
            return false;
        }
    }
    
    m_itemName = newName;
    m_range.min = min;
    m_range.max = max;
    return true;
}

bool RangeConfigDialog::validatePassword()
{
    QString password = m_passwordEdit->text();
    if (password != ADMIN_PASSWORD) {
        QMessageBox::warning(this, "密码错误", "修改参数名称需要管理员密码！");
        return false;
    }
    return true;
}

void RangeConfigDialog::onNameChanged(const QString& text)
{
    if (text != m_originalName) {
        m_passwordEdit->setVisible(true);
        m_passwordLabel->setVisible(true);
    } else {
        m_passwordEdit->setVisible(false);
        m_passwordLabel->setVisible(false);
    }
}

void RangeConfigDialog::onOkClicked()
{
    if (validateRange()) {
        accept();
    }
}

void RangeConfigDialog::onCancelClicked()
{
    reject();
}

Range RangeConfigDialog::getRange() const
{
    return m_range;
}

QString RangeConfigDialog::getItemName() const
{
    return m_itemName;
}

bool RangeConfigDialog::isNameModified() const
{
    return m_itemName != m_originalName;
}
