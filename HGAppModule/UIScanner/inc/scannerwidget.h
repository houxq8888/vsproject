#ifndef SCANNERWIDGET_H
#define SCANNERWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include "scannerinputlineedit.h"
#include <QLabel>
#include <QTableWidget>

class ScannerWidget : public QWidget
{ 
    Q_OBJECT
public:
    ScannerWidget(std::string,QWidget *parent = nullptr);
    bool closeWindow();
    ~ScannerWidget();
    /**
     * @brief 解析输入字符串
     * 
     * @param inputStr 输入字符串
     * @param parseList 解析结果列表Sname,Sbatch,Sserial,Samount
     * @return int 解析成功返回0，失败返回-1,-2
     */
    int decodeInput(const std::string &inputStr,std::map<std::string,std::string> &parseList);

private slots:
    void updatePreview();
    void saveSettings();
    void resetSettings();
    void onTextChanged(const QString &newText);
    void parseData(const QStringList &dataList);

private:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void sendMsg2Manager(int);

private:
    std::string m_lang;
    QGridLayout *m_layout;
    QGridLayout* m_contentLayout,*m_separatorLayout,*m_realDisplayLayout;
    QGroupBox* m_contentGroupBox,*m_separatorGroupBox,*m_realDisplayGroupBox;
    QPushButton* m_saveBtn,*m_resetBtn;

    QLineEdit* m_separatorLineEdit;
    QLabel* m_realDisplayRegionLabel;
    QCheckBox* m_sampleNameCheckBox, *m_batchCheckBox, *m_serialCheckBox,*m_weightCheckBox, *m_dateCheckBox, *m_timeCheckBox, *m_locationCheckBox;

    QGroupBox* m_testGroupBox;
    QGridLayout* m_testLayout;
    ScanInputLineEdit* m_testLineEdit;
    QTableWidget* m_testTable;

    QLabel* m_testStatusLabel;

    std::map<std::string,std::string> scannerInfo;

};

#endif // SCANNERWIDGET_H