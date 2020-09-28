#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->init();
}

void Widget::init()
{
    //初始化
    ui->saveBtn->setDisabled(true);
    ui->checkBox->setDisabled(true);
    ui->codeView->setDisabled(true);
}
bool Widget::selectSourseFile()
{
    return true;
}

Widget::~Widget()
{
    delete ui;
}

