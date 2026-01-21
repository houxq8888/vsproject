#include "scannerinputlineedit.h"
#include <QDebug>

ScanInputLineEdit::ScanInputLineEdit(QWidget *parent) : QLineEdit(parent)
    {
        lastInputTime = QTime::currentTime();
        inputTimer = new QTimer(this);
        inputTimer->setSingleShot(true);
        connect(inputTimer, &QTimer::timeout, this, &ScanInputLineEdit::onTimeout);

        isScanInput =false;
        isScanOver = false;
    }

    void ScanInputLineEdit::keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Backspace)
        {
            // 删除一个字符，确保不会清空全部内容
            if (!text().isEmpty()) {
                setText("");
                QLineEdit::keyPressEvent(event);  // 调用基类的处理函数，删除一个字符
                return;
            }
        }
        QLineEdit::keyPressEvent(event);

        // 判断输入的时间间隔
        int elapsed = lastInputTime.msecsTo(QTime::currentTime());
        lastInputTime = QTime::currentTime();

        isScanOver = false;
        if (elapsed < 50)  // 你可以根据实际情况调整这个时间
        {
            // 认为是扫码枪输入
            isScanInput = true;
            printf("扫码枪输入\n");
        }
        else
        {
            // 认为是手动输入
            isScanInput = false;
            printf("手动输入\n");
        }

        // 重置计时器，等待下一次输入
        inputTimer->start(200);  // 200ms时间间隔
    }

    void ScanInputLineEdit::onTimeout()
    {
        printf("输入结束，可能是手动输入\n");
        isScanOver = true;
    }

    bool ScanInputLineEdit::isScannerInput() const
    {
        return isScanInput;
    }