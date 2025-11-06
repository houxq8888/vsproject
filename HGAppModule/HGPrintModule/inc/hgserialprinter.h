#ifndef HGSERIALPRINTER_H
#define HGSERIALPRINTER_H 

#include <QWidget>

class SerialPrinter:public QWidget{
    Q_OBJECT
    public:
        SerialPrinter(QWidget *parent = nullptr);
        ~SerialPrinter();

};
#endif