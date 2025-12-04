// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "pluginmanager.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QGridLayout>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(const std::string& lang,QWidget *parent = nullptr);
    ~MainWindow();

signals:

private slots:
    void onItemClicked(QTreeWidgetItem *item, int column);

private:
    void loadPlugin(const QString& name);
    
private:
    QWidget* m_pluginArea;
    QGridLayout* m_pluginlayout;
    PluginManager* m_pluginManager;
    QTreeWidgetItem *__qtreewidgetitem;
    QTreeWidget *__qtreewidget;
    std::string m_lang;
};

#endif // MAINWINDOW_H
