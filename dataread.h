#ifndef DATAREAD_H
#define DATAREAD_H

#include<iostream>
#include<fstream>
#include<time.h>
#include<algorithm>
#include<sys/stat.h>
#include<QDebug>
#include<QString>
#include<windows.h>
#define MAXSUBSTRSIZE 1024*1024
#define MAXSIZE 1024*1024*1024
class DataRead
{
public:
    long long fileSize;
    QString fileDir;
    QString fileName;
    QString fileType;
    double genTime;
    double readTime;
    int modelSelected;

    struct RoadPoint{
        double x;
        double y;
    };

    struct RoadAttribute{
        char ID[MAXSUBSTRSIZE];
        char Kind_num[5];
        int width;
        char PathName[MAXSUBSTRSIZE];
        char SnodeID[MAXSUBSTRSIZE];
        char EnodeID[MAXSUBSTRSIZE];
        char length[10];
        int RPointNum;
    };

    struct Road{
        struct RoadAttribute RoadAttr;
        RoadPoint coordArr[MAXSIZE];
    };

public:
    DataRead();
    DataRead(QString dir);
    void genData();
    void readData();
    void readData2();
    void readData3();
    void getBegin();

};

#endif


