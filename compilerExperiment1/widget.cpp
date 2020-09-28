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
    //链接信号和槽
    connect(ui->openFileBtn,&QPushButton::clicked,this,&Widget::unlockBtn);
}
bool Widget::selectSourseFile()
{
    return true;
}
void Widget::unlockBtn()
{
    ui->saveBtn->setDisabled(false);
    ui->checkBox->setDisabled(false);
    ui->codeView->setDisabled(false);
}

Widget::~Widget()
{
    delete ui;
}

