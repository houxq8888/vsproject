#ifndef RANGECONFIGDIALOG_H
#define RANGECONFIGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMap>
#include <QString>

struct Range {
    float min;
    float max;
};

class RangeConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RangeConfigDialog(const QString& itemName, const Range& currentRange, QWidget *parent = nullptr);
    virtual ~RangeConfigDialog();

    Range getRange() const;
    QString getItemName() const;
    bool isNameModified() const;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onNameChanged(const QString& text);

private:
    void setupUI();
    bool validateRange();
    bool validatePassword();

private:
    QString m_itemName;
    QString m_originalName;
    Range m_range;

    QLineEdit* m_nameEdit;
    QLineEdit* m_minEdit;
    QLineEdit* m_maxEdit;
    QLineEdit* m_passwordEdit;
    QLabel* m_passwordLabel;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // RANGECONFIGDIALOG_H
