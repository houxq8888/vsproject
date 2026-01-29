#include "CommDialog.h"
#include <QMessageBox>
#include <QIntValidator>

#define ADMIN_PASSWORD "admin"

CommDialog::CommDialog(const CommConfig& config, QWidget *parent)
    : QDialog(parent), m_config(config)
{
    setupUI();
    setWindowTitle("通讯配置");
    setModal(true);
}

CommDialog::~CommDialog()
{
}

void CommDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* mqttGroup = new QGroupBox("MQTT 配置", this);
    QFormLayout* mqttLayout = new QFormLayout(mqttGroup);

    m_mqttHostEdit = new QLineEdit(this);
    m_mqttHostEdit->setText(m_config.mqttHost);
    m_mqttHostEdit->setPlaceholderText("MQTT 服务器地址");

    m_mqttPortEdit = new QLineEdit(this);
    m_mqttPortEdit->setText(QString::number(m_config.mqttPort));
    m_mqttPortEdit->setValidator(new QIntValidator(1, 65535, this));
    m_mqttPortEdit->setPlaceholderText("MQTT 端口");

    m_mqttUsernameEdit = new QLineEdit(this);
    m_mqttUsernameEdit->setText(m_config.mqttUsername);
    m_mqttUsernameEdit->setPlaceholderText("MQTT 用户名");

    m_mqttPasswordEdit = new QLineEdit(this);
    m_mqttPasswordEdit->setText(m_config.mqttPassword);
    m_mqttPasswordEdit->setEchoMode(QLineEdit::Password);
    m_mqttPasswordEdit->setPlaceholderText("MQTT 密码");

    mqttLayout->addRow("服务器地址:", m_mqttHostEdit);
    mqttLayout->addRow("端口:", m_mqttPortEdit);
    mqttLayout->addRow("用户名:", m_mqttUsernameEdit);
    mqttLayout->addRow("密码:", m_mqttPasswordEdit);

    mainLayout->addWidget(mqttGroup);

    QGroupBox* socketGroup = new QGroupBox("Socket 配置", this);
    QFormLayout* socketLayout = new QFormLayout(socketGroup);

    m_tuzhuangIPEdit = new QLineEdit(this);
    m_tuzhuangIPEdit->setText(m_config.tuzhuangIP);
    m_tuzhuangIPEdit->setPlaceholderText("涂装 IP 地址");

    m_tuzhuangPortEdit = new QLineEdit(this);
    m_tuzhuangPortEdit->setText(QString::number(m_config.tuzhuangPort));
    m_tuzhuangPortEdit->setValidator(new QIntValidator(1, 65535, this));
    m_tuzhuangPortEdit->setPlaceholderText("涂装 端口");

    m_didingIPEdit = new QLineEdit(this);
    m_didingIPEdit->setText(m_config.didingIP);
    m_didingIPEdit->setPlaceholderText("滴定 IP 地址");

    m_didingPortEdit = new QLineEdit(this);
    m_didingPortEdit->setText(QString::number(m_config.didingPort));
    m_didingPortEdit->setValidator(new QIntValidator(1, 65535, this));
    m_didingPortEdit->setPlaceholderText("滴定 端口");

    m_shuifenIPEdit = new QLineEdit(this);
    m_shuifenIPEdit->setText(m_config.shuifenIP);
    m_shuifenIPEdit->setPlaceholderText("水分 IP 地址");

    m_shuifenPortEdit = new QLineEdit(this);
    m_shuifenPortEdit->setText(QString::number(m_config.shuifenPort));
    m_shuifenPortEdit->setValidator(new QIntValidator(1, 65535, this));
    m_shuifenPortEdit->setPlaceholderText("水分 端口");

    socketLayout->addRow("涂装 IP:", m_tuzhuangIPEdit);
    socketLayout->addRow("涂装 端口:", m_tuzhuangPortEdit);
    socketLayout->addRow("滴定 IP:", m_didingIPEdit);
    socketLayout->addRow("滴定 端口:", m_didingPortEdit);
    socketLayout->addRow("水分 IP:", m_shuifenIPEdit);
    socketLayout->addRow("水分 端口:", m_shuifenPortEdit);

    mainLayout->addWidget(socketGroup);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_okButton = new QPushButton("确定", this);
    m_cancelButton = new QPushButton("取消", this);

    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &CommDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &CommDialog::onCancelClicked);

    setLayout(mainLayout);
}

bool CommDialog::validateConfig()
{
    if (m_mqttHostEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "MQTT 服务器地址不能为空！");
        return false;
    }

    bool ok;
    int port = m_mqttPortEdit->text().toInt(&ok);
    if (!ok || port < 1 || port > 65535) {
        QMessageBox::warning(this, "输入错误", "MQTT 端口必须是 1-65535 之间的数字！");
        return false;
    }

    port = m_tuzhuangPortEdit->text().toInt(&ok);
    if (!ok || port < 1 || port > 65535) {
        QMessageBox::warning(this, "输入错误", "涂装端口必须是 1-65535 之间的数字！");
        return false;
    }

    port = m_didingPortEdit->text().toInt(&ok);
    if (!ok || port < 1 || port > 65535) {
        QMessageBox::warning(this, "输入错误", "滴定端口必须是 1-65535 之间的数字！");
        return false;
    }

    port = m_shuifenPortEdit->text().toInt(&ok);
    if (!ok || port < 1 || port > 65535) {
        QMessageBox::warning(this, "输入错误", "水分端口必须是 1-65535 之间的数字！");
        return false;
    }

    return true;
}

void CommDialog::onOkClicked()
{
    if (validateConfig()) {
        m_config.mqttHost = m_mqttHostEdit->text().trimmed();
        m_config.mqttPort = m_mqttPortEdit->text().toInt();
        m_config.mqttUsername = m_mqttUsernameEdit->text().trimmed();
        m_config.mqttPassword = m_mqttPasswordEdit->text();
        m_config.tuzhuangIP = m_tuzhuangIPEdit->text().trimmed();
        m_config.tuzhuangPort = m_tuzhuangPortEdit->text().toInt();
        m_config.didingIP = m_didingIPEdit->text().trimmed();
        m_config.didingPort = m_didingPortEdit->text().toInt();
        m_config.shuifenIP = m_shuifenIPEdit->text().trimmed();
        m_config.shuifenPort = m_shuifenPortEdit->text().toInt();
        accept();
    }
}

void CommDialog::onCancelClicked()
{
    reject();
}

CommConfig CommDialog::getConfig() const
{
    return m_config;
}
