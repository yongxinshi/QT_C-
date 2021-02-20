#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QComboBox>
#include<QMap>
#include<QString>
#include<QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    long long dataSetSize;
    QButtonGroup BG;

public:
    MainWindow(QWidget *parent = nullptr);

    void iniPushButton();
    void iniDataSetSizeSelect();
    void iniRadioButton();
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void getBegin();

};
#endif // MAINWINDOW_H
