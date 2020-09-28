#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QAction>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void init();
    void unlockBtn();
    bool selectSourseFile();
    bool saveSourseFile();
    void scanSourseFile();
    bool showCompressedFile();
    bool saveResultFile(bool isCompressed=false);
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
