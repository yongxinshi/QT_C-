#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataread.h"
#include<QDebug>
#include<QVector>
#include<QPair>
#include<QDialog>
#include<QStringList>
#include<QButtonGroup>
#include<direct.h>
//#include<iostream>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //iniPushButton();
    //iniDataSetSizeSelect();

//    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(print_s()));
     iniRadioButton();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(getBegin()));
}

void MainWindow::getBegin(){

    //    QStringList strList = {"B","KB","MB","GB"};
    //    QStringList strList2 = {"方法1","方法2","方法3"};

    QStringList strList = {"MB","GB"};
    QStringList strList2 = {"使用ofstream读取","使用Handle读取"};

    int index = ui->comboBox->currentIndex();
    long long  num = ui->lineEdit->text().toLongLong();
    int module =  this->BG.checkedId();

    this->dataSetSize = (long long)(1 << ( index * 10)) * (long long)num;

    //获取当前工作路径
    char strModule[256];
    getcwd(strModule, 256);


//    ui->radioButton;
    //获取需要操作的文件名
    string fileName = std::to_string(num) + strList[index].toStdString();

    string dir = strModule;
    DataRead dr(QString::fromStdString(dir));
    dr.fileName = QString::fromStdString(fileName);
    dr.fileSize = this->dataSetSize;
    dr.modelSelected = module;
    qDebug()<<"fileDir: "<<dr.fileDir +"\\"+ dr.fileName + dr.fileType;
    dr.getBegin();

}

void MainWindow:: iniPushButton(){

    QPushButton * btn = new QPushButton("开始",this);
    btn->move(200,200);

}

void MainWindow::iniDataSetSizeSelect(){



//    QComboBox * cbBox = new QComboBox(this);
//    QDialog * inputDialog = new QDialog(this);
//    qDebug()<<"ParentWidget:"<<cbBox->parentWidget();

//    cbBox->move(300,100);

//    QVector<QPair<QString, long long>> dataSetSize;
//    dataSetSize.push_back({"B", 1<<0});
//    dataSetSize.push_back({"KB", 1<20});
//    dataSetSize.push_back({"MB", 1<<30});
//    dataSetSize.push_back({"GB", (long long )1<<40});

//    cbBox->clear();
//    for(auto & [str, capity]:dataSetSize){
//        cbBox->addItem(str, capity);
//        qDebug()<<"capity: "<< capity;
//    }
//    cbBox->setEditable(true);
//    int index = cbBox->currentData().toInt();
//    qDebug()<<"index: "<<index;

}

void MainWindow::iniRadioButton(){
    this->BG.setParent(this);

    this->BG.addButton(ui->radioButton);
    this->BG.addButton(ui->radioButton_2);
//    this->BG.addButton(ui->radioButton_3);

    this->BG.setId(ui->radioButton, 0);
    this->BG.setId(ui->radioButton_2, 1);
//    this->BG.setId(ui->radioButton_3, 2);
    ui->radioButton->setChecked(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

