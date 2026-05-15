#ifndef COMMDIALOG_H
#define COMMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QString>

struct CommConfig {
    QString mqttHost;
    int mqttPort;
    QString mqttUsername;
    QString mqttPassword;
    QString tuzhuangIP;
    int tuzhuangPort;
    QString didingIP;
    int didingPort;
    QString shuifenIP;
    int shuifenPort;
};

class CommDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommDialog(const CommConfig& config, QWidget *parent = nullptr);
    virtual ~CommDialog();

    CommConfig getConfig() const;

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    bool validateConfig();

private:
    CommConfig m_config;

    QLineEdit* m_mqttHostEdit;
    QLineEdit* m_mqttPortEdit;
    QLineEdit* m_mqttUsernameEdit;
    QLineEdit* m_mqttPasswordEdit;
    QLineEdit* m_tuzhuangIPEdit;
    QLineEdit* m_tuzhuangPortEdit;
    QLineEdit* m_didingIPEdit;
    QLineEdit* m_didingPortEdit;
    QLineEdit* m_shuifenIPEdit;
    QLineEdit* m_shuifenPortEdit;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // COMMDIALOG_H
