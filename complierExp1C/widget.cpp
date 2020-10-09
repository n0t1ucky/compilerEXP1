#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->init();
    this->initFunction();
}

void Widget::init()
{
    //初始化
    headerItem=new QStringList;
    *headerItem<<"Lexis"<<"Type";
    this->opened=false;
    this->setWindowTitle("编译原理 实验1");
    ui->resultView->setColumnCount(2);
    //ui->resultView->setRowCount(1000);
    ui->resultView->setHorizontalHeaderLabels(*headerItem);
    ui->saveBtn->setDisabled(true);
    ui->checkBox->setDisabled(true);
    ui->codeView->setDisabled(true);
    ui->scanBtn->setDisabled(true);
    ui->resultView->setDisabled(true);
    ui->svrsBtn->setDisabled(true);
    savedMsgBox.setWindowTitle("保存成功");
    aboutMsgBox.setWindowTitle("关于");
    aboutMsgBox.setText("编译原理 实验1\n作者:李龙燊(20182131021)");
    //链接信号和槽
    connect(ui->openFileBtn,&QPushButton::released,this,&Widget::unlockBtn);
    connect(ui->openFileBtn,&QPushButton::clicked,this,&Widget::selectSourseFile);
    connect(ui->scanBtn,&QPushButton::clicked,this,&Widget::scanCode);
    connect(ui->checkBox,&QCheckBox::toggled,this,&Widget::showClearCode);
    connect(ui->saveBtn,&QPushButton::clicked,this,&Widget::saveSourseFile);
    connect(ui->svrsBtn,&QPushButton::clicked,this,&Widget::saveResultFile);
    connect(ui->AboutBtn,&QPushButton::clicked,this,&Widget::showAboutMsgBox);
}
bool Widget::selectSourseFile()
{
    dlgTitle="选择文件...";
    filter="C++源文件(*.c *.cpp *.h);;所有文件(*,*)";
    fileName=QFileDialog::getOpenFileName(this,dlgTitle,QDir::currentPath(),filter);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QDialog(this);
    }
    QTextStream fileInput(&file);
    if(!fileName.isEmpty())
    {
        this->opened=true;
        ui->fileStatus->setText(fileName);
        codeCPP=fileInput.readAll();
        rawCode=codeCPP;
        cleanCode();
        ui->codeView->setText(rawCode);
    }
    ui->resultView->clear();
    ui->svrsBtn->setDisabled(true);
    return true;
}

bool Widget::saveSourseFile()
{
    dlgTitle="保存文件...";
    filter="C++源文件(*.cpp);;C++头文件(*.h);;文本文件(*.txt);;所有文件(*,*)";
    fileName=QFileDialog::getSaveFileName(this,dlgTitle,QDir::currentPath(),filter);
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
    {
        QDialog(this);
    }
    else
    {
        file.write(ui->codeView->toPlainText().toUtf8());
        file.close();
        savedMsgBox.setText("文件已保存到"+fileName);
        savedMsgBox.exec();
    }
}

bool Widget::saveResultFile()
{
    dlgTitle="保存文件...";
    filter="文本文件(*.txt);;所有文件(*,*)";
    fileName=QFileDialog::getSaveFileName(this,dlgTitle,QDir::currentPath(),filter);
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
    {
        QDialog(this);
    }
    else
    {
        file.write(resultText.toUtf8());
        file.close();
        savedMsgBox.setText("文件已保存到"+fileName);
        savedMsgBox.exec();
    }
}
void Widget::unlockBtn()
{
    ui->saveBtn->setDisabled(false);
    ui->checkBox->setDisabled(false);
    ui->codeView->setDisabled(false);
    ui->scanBtn->setDisabled(false);
    ui->resultView->setDisabled(false);
    //ui->svrsBtn->setDisabled(false);
}

void Widget::tableAddItem(int row,QString A,QString B)
{
    ui->resultView->setRowCount(row+1);
    ui->resultView->insertRow(row);
    ui->resultView->setItem(row,0,new QTableWidgetItem(A));
    ui->resultView->setItem(row,1,new QTableWidgetItem(B));
    resultText+=A+"     "+B+"\n";
}

void Widget::showClearCode(bool toggled)
{
    if(toggled)
        ui->codeView->setText(codeCPP);
    else
        ui->codeView->setText(rawCode);
}

void Widget::showAboutMsgBox()
{
    aboutMsgBox.exec();
}

void Widget::initFunction()
{
    rsv="asm else new this\
auto enum operator throw\
bool explicit private true\
break export protected try\
case extern public typedef\
catch false register typeid\
char float reinterpret_cast typename\
class for return union\
const friend short unsigned\
const_cast goto signed using\
continue if sizeof virtual\
default inline static void\
delete int static_cast volatile\
do long struct wchar_t\
double mutable switch while\
dynamic_cast namespace template\
cin cout include iostream";
    opr="+ - * / % ++ -- == != > < >= <= && || ! = += -= *= /= %= &= ^= |= -> >> << [ ] []";
    dlm="# { } ( ) ; < > ' \" ";
    ending="\n \0";
    letter="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    letter+=letter.toLower();
    letter+='_';
    number="0123456789.";
    Rsvd=new QStringList;
    Oprt=new QStringList;
    Dlmt=new QStringList;
    End=new QStringList;
    *Rsvd=rsv.split(" ");
    *Oprt=opr.split(" ");
    *Dlmt=dlm.split(" ");
    *End=ending.split(" ");
    //QStringList codeType={"保留字","标识符","数字","字符串","特殊符号"};
    qDebug()<<*Rsvd;
    qDebug()<<*Oprt;
    qDebug()<<*Dlmt;
    qDebug()<<*End;
}
void Widget::cleanCode()
{
    //整理代码
    //移除注释
    int posSlash=codeCPP.indexOf('/');
    int len=0;
    while(posSlash>=0)
    {
        qDebug()<<"posSlash="<<posSlash<<endl;
        if(codeCPP.at(posSlash+1)=='/')
        {
            while(codeCPP.at(posSlash+1+len)!='\n')
            {
                ++len;
            }
        }
        else if(codeCPP.at(posSlash+1)=='*')
        {
            len=codeCPP.indexOf("*/")-posSlash+1;
            qDebug()<<"len="<<len<<endl;
        }
        else
        {
            posSlash=codeCPP.indexOf('/',posSlash+1);
            continue;
        }
        if(len>0)
        {
            codeCPP.remove(posSlash,len+2);
            posSlash=codeCPP.indexOf("/");
            len=0;
        }
    }
    codeCPP.remove("\t");
}

bool Widget::isLetter(QChar c)
{
    return letter.contains(c);
}

bool Widget::isNumber(QChar c)
{
    return number.contains(c);
}

bool Widget::isRsvd(QString s)
{
    return Rsvd->contains(s);
}
int Widget::charType(QChar curChar)
{
    if(curChar==' ')
    {
        return 99;
    }
    if(isLetter(curChar))
    {
        return 1;
    }
    if(isNumber(curChar))
    {
        if(curChar=='.')
            return 6;
        return 2;
    }
    if(Oprt->contains(curChar))
    {
        return 4;
    }
    if(Dlmt->contains(curChar))
    {
        if(curChar=='"')
            return 5;
        return 3;
    }
    else return 100;
}
void Widget::scanCode()
{
    Result=new QStringList;
    ui->scanStatus->setText("已分析");
    resultText="";
    ui->svrsBtn->setDisabled(false);

    int curPos=0,countWord=0,floatFlag=0,typeFlag=0;
    QChar curChar=' ';
    QString token="";
    //typeFlag=100(错误)0(初始)1(字母与下划线)2(数字)3(分隔符)4(运算符)5(串)6(小数)
    while(curChar!='\0'&&curPos<codeCPP.length())
    {
        curChar=codeCPP.at(curPos++);
        token+=curChar;
        typeFlag=charType(curChar);
        switch(typeFlag)
        {
        case 1:
            curChar=codeCPP.at(curPos);
            while(charType(curChar)<3)
            {
                token+=curChar;
                curChar=codeCPP.at(++curPos);
            }
            if(isRsvd(token))
            {
                tableAddItem(countWord,token,"保留字");
                ++countWord;
                qDebug()<<token<<",reserved";
            }
            else
            {
                tableAddItem(countWord,token,"标识符");
                ++countWord;
                qDebug()<<token<<",identifier";
            }
        break;
        case 2:
        case 6:
            if(typeFlag==6)
                floatFlag=1;
            while(charType(curChar)==2||charType(curChar)==6)
            {
                curChar=codeCPP.at(curPos);
                if(floatFlag==0&&charType(curChar)==6)
                    floatFlag=1;
                else if(floatFlag==1&&charType(curChar)==6)
                {
                    floatFlag=-1;
                    tableAddItem(countWord,token,"未定义");
                    qDebug()<<token<<",error";
                    break;
                }
                typeFlag=charType(curChar);
                if(charType(curChar)==2||charType(curChar)==6)
                    token+=curChar;
                ++curPos;
            }
            if(floatFlag>-1)
            {
                tableAddItem(countWord,token,"数");
                ++countWord;
                qDebug()<<token<<",number";
            }
        break;
        case 3:
            tableAddItem(countWord,token,"特殊符号");
            ++countWord;
            qDebug()<<token<<",delimiter";
        break;
        case 4:
            curChar=codeCPP.at(curPos);
            if(charType(curChar)!=4)
            {
                tableAddItem(countWord,token,"特殊符号");
                ++countWord;
                qDebug()<<token<<",operator";
            }
            else if(Oprt->contains(QString(token+curChar)))
            {
                //qDebug()<<"OPERATORS";
                token+=curChar;
                tableAddItem(countWord,token,"特殊符号");
                ++countWord;
                ++curPos;
                qDebug()<<token<<",operator";
            }
            else
            {
                qDebug()<<token<<",error";
            }
        break;
        case 5:
            while(true)
            {
                curChar=codeCPP.at(curPos++);
                if(End->contains(curChar)&&curPos>codeCPP.length())
                {
                    tableAddItem(countWord,token,"未定义");
                    qDebug()<<token<<",error";
                    break;
                }
                else if(charType(curChar)!=5)
                {
                    token+=curChar;
                }
                else
                {
                    token+=curChar;
                    tableAddItem(countWord,token,"串");
                    ++countWord;
                    qDebug()<<token<<",string";
                    break;
                }
            }
        break;
        default:
            break;
        }
        floatFlag=0;
        token="";
    }
}
Widget::~Widget()
{
    delete ui;
}

