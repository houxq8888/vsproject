#ifndef COMMON_H
#define COMMON_H

#include <QRect>
#include <QTableWidget>
#include <QListWidget>
#include <QGridLayout>
#include <sstream>
#include <QToolBox>
#include "labelwithimg.h"
#include "rwDb.h"
#include "HGExactTime.h"
#include "hgcommonutility.h"
#include "hgxml.h"
#include "hglog4cplus.h"
#include "HGMacroData.h"
#include "hgprinter.h"
#include "config.h"
#include "hgtxt.h"
#include "hgcsv.h"
#include "hgpdf.h"
#include "hgserial.h"
#include "hgsecurity.h"
#include "HGMacroData.h"
// #include "version.h"
// #include "HGOnlinePlatformInterface.h"
#include "globalsingleton.h"

using namespace HGMACHINE;

// template function
template <typename T>
QList<T*> findWidgetsByType(QLayout* layout){
    QList<T*> widgets;
    std::function<void(QLayout*)> traverseLayout = [&](QLayout* currentLayout){
        if (!currentLayout) return;
        for (int i=0;i<currentLayout->count();i++){
            QLayoutItem* item=currentLayout->itemAt(i);
            if (!item) continue;
            if (QWidget* widget=item->widget()){
                if (T* targetWidget=qobject_cast<T*>(widget)){
                    widgets.append(targetWidget);
                }
                if (widget->layout()){
                    traverseLayout(widget->layout());
                }
            }
            else if (QLayout* childLayout=item->layout()){
                traverseLayout(childLayout);
            }
        }
    };
    traverseLayout(layout);
    return widgets;
}
QRect getScreenGeometry();
int getSelectedRow(QTableWidget *tableWidget);
void setFocusByName(QListWidget* listWidget,const std::string& name);
int getColumnIndexByName(QTableWidget* table,const QString &columnName);
int getColumnIndexByToolTip(QTableWidget* table,const QString &toolTip);
void setTableWColNoEdit(QTableWidget* table,int column);
bool findWidgetPosition(QGridLayout* layout,const std::string& name,int &row,int &column,bool &stable);
void setWidgetTextAtPosition(QGridLayout* layout,int row,int column,const std::string&text);
void setControlText(QGridLayout* layout,const std::string& param);
bool IsValidNumber(QString &qstrSrc);
void setUniformSizePolicy(QLayout *layout,QSizePolicy::Policy hPolicy,
    QSizePolicy::Policy vPolicy);
std::string getPath(std::string inPath);
void removeWidgetsFromLayout(QGridLayout *layout) ;
void selectCellsUsingSelectionModel(QTableWidget* tableWidget, const QString& targetText);
std::vector<std::pair<size_t,size_t>> matchStartEndPairs(
    const std::vector<size_t>& starts,
    const std::vector<size_t>& ends,
    size_t maxLength
);
enum{
    DRIFT_AUTO,
    DRIFT_MANUAL,
    DRIFT_OFF
};
enum{
    MANIPULATE_DOWN,
    MANIPULATE_UP,
    MANIPULATE_CLOSE,
    MANIPULATE_ALL
};
enum {
    TASK_CREATE,
    TASK_RECTIFY,
    TASK_NOEDIT
};

#define HG_DEVICE_NAME "AKF-CAS6自动进样水分仪" //"工业过程在线分析仪"
#define HG_DEVICE_MODEL "AKF-CAS6" //"ALT"
// #define HG_SOFTWARE_VERSION PROJECT_VERSION
#define HG_MAX_SEARCH_RANGE 7


#endif // COMMON_H