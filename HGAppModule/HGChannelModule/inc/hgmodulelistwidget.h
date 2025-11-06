#ifndef HGMODULELISTWIDGET_H
#define HGMODULELISTWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <map>
#include <vector>
#include <unordered_map>
#include "hgqlabel.h"
#include "labelwithimg.h"
#include <QScrollArea>

typedef struct {
    std::vector<LabelWithImg*> labels;
    QGroupBox* groupBox;
    QVBoxLayout* layout;
} HGModules;

class HGModuleListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HGModuleListWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();

signals:
    void createModule(std::string);
private slots:
    void slotCreateModule(std::string name);
    void slotLeftClickLabel(std::string);

private:
    void setAllFalse();
    
private:
    QVBoxLayout* m_layout, *m_mainLayout;
    QWidget *m_contentWidget;
    QScrollArea* m_scrollArea;
    std::unordered_map<std::string,HGModules> m_modules;
    std::string m_lang;
};

#endif // HGMODULELISTWIDGET_H
