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

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    bool validateRange();

private:
    QString m_itemName;
    Range m_range;

    QLineEdit* m_minEdit;
    QLineEdit* m_maxEdit;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // RANGECONFIGDIALOG_H
