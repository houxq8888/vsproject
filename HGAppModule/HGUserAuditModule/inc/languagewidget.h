#ifndef LANGUAGEWIDGET_H
#define LANGUAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include "labelwithimg.h"
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "basewidget.h"

class LanguageWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit LanguageWidget(QWidget *parent = nullptr);
    ~LanguageWidget();
    bool closeWindow();

signals:

private slots:
    void switchLanguage(int);

private:
    void fnReadDB();
    void fnWriteDB();
    void retranslateUi();
    
private:
    QLabel* m_languageLabel;
    QComboBox* m_languageComboBox;
    QGridLayout* m_layout;
};

#endif // LANGUAGEWIDGET_H
