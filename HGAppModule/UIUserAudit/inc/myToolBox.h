#ifndef MYTOOLBOX_H
#define MYTOOLBOX_H

#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include "basewidget.h"


class ToolPage : public QWidget
{
    Q_OBJECT

public:
    explicit ToolPage(QWidget *parent = nullptr);
    ~ToolPage();
    bool closeWindow();
    BaseWidget* getWidget();
    void setLanguage(std::string);

signals:
    void signalSoundVolumn(int);
    void updateLanguage(std::string);

public slots:
    void addWidget(const std::string& lang, const std::string &title, BaseWidget *widget);
    void expand();
    void collapse();

private slots:
    void onPushButtonFoldClicked();

private:
    QPushButton *m_pPushButtonFold;
    QVBoxLayout *m_pContentVBoxLayout;
    bool m_bIsExpanded;
    QLabel *m_pLabel;
    std::string m_title;

};

class MyToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit MyToolBox(QWidget *parent = nullptr);
    ~MyToolBox();
    bool closeWindow();
    void setLanguage(std::string);

    BaseWidget* getWidget(const std::string& title);
    void addWidget(const std::string& lang, const std::string &title, BaseWidget *widget);
    QVBoxLayout *getContentVBoxLayout() {return m_pContentVBoxLayout;};

signals:
    void signalSoundVolumn(int);
    void updateLanguage(std::string);

private:
    QScrollArea* m_scrollArea;
    QVBoxLayout *m_pContentVBoxLayout;
};

#endif // MYTOOLBOX_H
