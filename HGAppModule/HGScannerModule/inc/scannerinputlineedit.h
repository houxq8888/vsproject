#ifndef SCANINPUTLINEEDIT_H
#define SCANINPUTLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>

class ScanInputLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    ScanInputLineEdit(QWidget *parent = nullptr) ;
    bool isScannerInput() const;
    bool isScannerOver() const {return isScanOver;};

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onTimeout();

private:
    QTime lastInputTime;
    QTimer *inputTimer;
    bool isScanInput;
    bool isScanOver;
};


#endif