#include "RangeConfigDialog.h"
#include <QMessageBox>
#include <QDoubleValidator>

RangeConfigDialog::RangeConfigDialog(const QString& itemName, const Range& currentRange, QWidget *parent)
    : QDialog(parent), m_itemName(itemName), m_range(currentRange)
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

    QGroupBox* groupBox = new QGroupBox("设置 " + m_itemName + " 的合格范围", this);
    QFormLayout* formLayout = new QFormLayout(groupBox);

    m_minEdit = new QLineEdit(this);
    m_minEdit->setText(QString::number(m_range.min));
    m_minEdit->setValidator(new QDoubleValidator(this));
    m_minEdit->setPlaceholderText("最小值");

    m_maxEdit = new QLineEdit(this);
    m_maxEdit->setText(QString::number(m_range.max));
    m_maxEdit->setValidator(new QDoubleValidator(this));
    m_maxEdit->setPlaceholderText("最大值");

    formLayout->addRow("最小值:", m_minEdit);
    formLayout->addRow("最大值:", m_maxEdit);

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

    m_range.min = min;
    m_range.max = max;
    return true;
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
