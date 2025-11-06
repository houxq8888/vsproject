/***************************************************************************
 * File Name  : main.cpp
 * Description:
 * Version    : V1.0.0.0
 * Author     : xiaoqin.hou
 * Date       : 2024-10
 * History    :
 *
***************************************************************************/

#include "HGCupDetWidget.h"
#include <QApplication>
#include <stdio.h>
#include <QProcess>


int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);
    //QApplication::setOverrideCursor(Qt::BlankCursor);//隐藏鼠标
    QFont font=app.font();
    font.setPointSize(/*0.012*width*/12);
    font.setFamily("Arial");
    app.setFont(font);

    HGCupDetWidget mainWidget;

    int result=app.exec();
    return result;
}
