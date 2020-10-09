#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QAction>
#include <QMessageBox>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    //窗口初始化
    void init();
    //功能初始化
    void initFunction();
    //窗口处理
    void unlockBtn();
    void tableAddItem(int row,QString A,QString B);
    void tableClear();
    void showClearCode(bool toggled);
    void showAboutMsgBox();
    //文件处理
    bool selectSourseFile();
    bool saveSourseFile();
    void scanSourseFile();
    bool showCompressedFile();
    bool saveResultFile();
    //分析函数
    void cleanCode();
    bool isLetter(QChar c);
    bool isNumber(QChar c);
    bool isRsvd(QString s);
    bool isDlmt(QString s);
    bool isOprt(QString s);
    void scanCode();
    int charType(QChar curChar);
private:
    bool opened;
    Ui::Widget *ui;
    QString fileName;
    QString dlgTitle;
    QString filter;
    QStringList *Rsvd;//保留字
    QString codeCPP;
    QString rawCode;
    QStringList *Oprt;//运算符
    QStringList *Dlmt;//分隔符
    QStringList Type;
    QStringList *End;
    QString opr;
    QString dlm;
    QString rsv;
    QString ending;
    QString letter;
    QString number;
    QString resultText;
    QStringList *Result;
    QStringList *headerItem;
    QMessageBox savedMsgBox;
    QMessageBox aboutMsgBox;
};
#endif // WIDGET_H
