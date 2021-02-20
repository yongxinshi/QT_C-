#include "dataread.h"
#include"handletest.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<windows.h>
#include<cstring>

using namespace std;

DataRead::DataRead(QString dir)
{
    this->fileDir = dir;
    this->fileType = ".txt";

}

void DataRead::genData()
{
    //生成大规模文件
    clock_t startTime, endTime;
    startTime = clock();

    long long readSize = this->fileSize / sizeof(int);
    string fileDir = this->fileDir.toStdString();
    string filePrefix = this->fileName.toStdString();
    string fileType = this->fileType.toStdString();

    ofstream ofs(fileDir + "\\"+filePrefix + fileType);
    qDebug()<<"readSize: "<<readSize;
    //设置随机数种子
    srand((unsigned int)(time(NULL)));


    long long bufferSize = 1  * 1024;
    string randInt(bufferSize, '\0');
    for (long long i = 0; i < bufferSize; ++i) {
        randInt[i] = rand() % 10 + '0';
    }
    for (long long i = 0; i < fileSize / bufferSize + 1; ++i) {
        ofs.write(&randInt[0], bufferSize);
    }

//    for(long long i = 0; i < readSize; ++i)
//    {
        ofs << rand()%10 + '0';
//        if(i != 0 && i % 128 == 0)
//        {
//            ofs << (int)rand() << endl;
//        }else{
//            ofs << (int)rand()  << ",";
//        }
//    }
    ofs.close();
    endTime = clock();
    this->genTime = ((double)endTime - startTime)/CLOCKS_PER_SEC;
    qDebug()<<"The Generate Data Time is :\t " << ((double)endTime - startTime)/CLOCKS_PER_SEC<<"s!"<<endl;
}

void DataRead::readData()
{
    qDebug()<<"使用iostream  \tread\twrite 进行读写";
    string fileDir = this->fileDir.toStdString();
    string filePrefix = this->fileName.toStdString();
    string fileType = this->fileType.toStdString();

    //若该文件名文件不存在则生成一份
    std::fstream fileCheck;
    fileCheck.open(fileDir + "\\" + filePrefix + fileType, ios::in);
    if(!fileCheck.is_open()){
        genData();
    }
    fileCheck.close();

//    genData(size);

    //记录开始时间
    clock_t startTime, endTime;
    startTime = clock();

    const int BUFFER_SIZE = 1024 * 10;


    std::ifstream ifs(fileDir + "\\" + filePrefix + fileType, ios::binary);
    std::ofstream ofs(fileDir + "\\" + filePrefix + "_readed" + fileType, ios::binary);

    if(ifs){
        ifs.seekg(0, ifs.end);
        int length = (int) ifs.tellg();
        ifs.seekg(0,ifs.beg);
        std::string buffer(BUFFER_SIZE, '\0');
        int offset = 0;
        int readSize = min(BUFFER_SIZE, length - offset);
        while(readSize > 0 && ifs.read(&buffer[0], readSize)){
            ofs.write(&buffer[0], readSize);
            offset += readSize;
            readSize = min(BUFFER_SIZE, length - offset);
    //		std::cout<<"offset:"<< offset << std::endl;
        }
    }
    struct stat info;

    stat((fileDir + "\\"+filePrefix + fileType).c_str(), &info);

    qDebug()<<"The File Size is:\t\t "<<(double)info.st_size<<endl;
    ifs.close();
    ofs.close();
    endTime = clock();
    this->readTime = ((double)endTime - startTime)/CLOCKS_PER_SEC;
    qDebug()<<"The Precessing Time Is :\t "<<((double)endTime - startTime)/CLOCKS_PER_SEC << "s!\n"<<endl;
}

void DataRead::readData2()
{
    qDebug()<<"使用HANDLE 句柄进行读写";
    clock_t startTime, endTime;
    startTime = clock();

    string fileDir = this->fileDir.toStdString();
    string filePrefix = this->fileName.toStdString();
    string fileType = this->fileType.toStdString();

    std::fstream fileCheck;
    fileCheck.open(fileDir + "\\" + filePrefix + fileType, ios::in);
    if(!fileCheck.is_open()){
        genData();
    }
    fileCheck.close();

    HandleTest ht;
    ht.fileDir = QString::fromStdString(fileDir + "\\"+filePrefix + fileType);

    ht.handle();

    struct stat info;
    stat((fileDir + "\\"+filePrefix + fileType).c_str(), &info);
    qDebug()<<"The File Size is:\t\t "<<(double)info.st_size<<endl;

    endTime = clock();
    this->readTime = ((double)endTime - startTime)/CLOCKS_PER_SEC;
    qDebug()<<"The Precessing Time Is :\t "<<((double)endTime - startTime)/CLOCKS_PER_SEC << "s!\n"<<endl;

    return;


//    // 声明并初始化变量
//    qDebug()<<"使用HANDLE 句柄进行读写";
//    string fileDir = this->fileDir.toStdString();
//    string filePrefix = this->fileName.toStdString();
//    string fileType = this->fileType.toStdString();

//    //若该文件名文件不存在则生成一份
//    std::fstream fileCheck;
//    fileCheck.open(fileDir + "\\" + filePrefix + fileType, ios::in);
//    if(!fileCheck.is_open()){
//        genData();
//    }
//    fileCheck.close();

//    //记录开始时间
//    clock_t startTime, endTime;
//    startTime = clock();

//    //将string转化为LPCWSTR;
//    char cFileDir[] = "";
//    strcpy(cFileDir, (fileDir + "\\" + filePrefix + fileType).c_str());

//    WCHAR *wcFileDir = new WCHAR[256];
//    MultiByteToWideChar(CP_ACP, 0, cFileDir, strlen(cFileDir) + 1, wcFileDir, 256);

////    HANDLE fileHandle = CreateFile(wszClassName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);


//    //创建文件映射
////    HANDLE fileHandle = CreateFile(wcFileDir, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//    HANDLE fileHandle = CreateFile(wcFileDir, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
//    if(fileHandle == INVALID_HANDLE_VALUE){
//        qDebug()<<"创建文件映射失败";
////        return;
//    }

//    //创建文件内存映射
//    HANDLE fileMapHandle = CreateFileMapping(fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
//    if(fileMapHandle == NULL){
//        qDebug()<<"创建文件内存映射失败";
////        return;
//    }

//    //映射文件数据到进程的地址空间中
//    HANDLE mapFile2Adr =  MapViewOfFile(fileMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
//    if(mapFile2Adr == NULL){
//        qDebug()<<"通过映射方法读取文件失败";
////        return;
//    }

//    //读取数据

//    char *buffer = (char*)mapFile2Adr;
//    double k;
//    int times = 300000*34;
//    for(int i = 0; i < times; ++i){
//        k = atof(buffer);
//        buffer = strstr(buffer+9, " ");
//    }

//    //关闭内存映射查询、关闭文件内存映射句柄、关闭文件映射句柄
//    UnmapViewOfFile(mapFile2Adr);
//    CloseHandle(fileMapHandle);
//    CloseHandle(fileHandle);


///*    char fileDirStr[] = "";
//    strcpy(fileDirStr, (fileDir + "\\" + filePrefix + fileType).c_str());
//    WCHAR wszClassName[256];
//    memset(wszClassName, 0, sizeof(wszClassName));
//    MultiByteToWideChar(CP_ACP, 0, fileDirStr, strlen(fileDirStr) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));

//    HANDLE fileHandle = CreateFile(wszClassName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
//    long long fileSize = GetFileSize(fileHandle, NULL);

//    int RoadCount = fileSize/sizeof(Road);

//    HANDLE handle = CreateFileMapping(fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
//    if(handle == NULL){
//        qDebug()<<"CreateFileMapping err \n";
//    }
//    LPVOID lp = MapViewOfFile(handle, FILE_MAP_READ, 0, 0, 0 );
//    if(lp == NULL){
//        DWORD err = GetLastError();
//        qDebug()<<"MapViewOfFile err = "<<err;
//    }
//    Road* tarDataP = new  Road [RoadCount];
//    if(tarDataP == NULL){
//        qDebug()<<"分配堆区内存失败";
//    }
//    delete []tarDataP;
////    memcpy_s(tarDataP, fileSize, lp, fileSize);
//    memcpy_s(tarDataP, RoadCount, lp, RoadCount);

//    UnmapViewOfFile(lp);
//    CloseHandle(handle);
//    CloseHandle(fileHandle);
//*/


//    //获取文件信息与结束时间
//    struct stat info;
//    stat((fileDir + "\\"+filePrefix + fileType).c_str(), &info);
//    qDebug()<<"The File Size is:\t\t "<<(double)info.st_size<<endl;

//    endTime = clock();
//    this->readTime = ((double)endTime - startTime)/CLOCKS_PER_SEC;
//    qDebug()<<"The Precessing Time Is :\t "<<((double)endTime - startTime)/CLOCKS_PER_SEC << "s!\n"<<endl;
}

void DataRead::readData3()
{
    string fileDir = this->fileDir.toStdString();
    string filePrefix = this->fileName.toStdString();
    string fileType = this->fileType.toStdString();

    //若该文件名文件不存在则生成一份
    std::fstream fileCheck;
    fileCheck.open(fileDir + "\\" + filePrefix + fileType, ios::in);
    if(!fileCheck.is_open()){
        genData();
    }
    fileCheck.close();

//    genData(size);

    //记录开始时间
    clock_t startTime, endTime;
    startTime = clock();

    const int BUFFER_SIZE = 1024 * 10;


    std::ifstream ifs(fileDir + "\\" + filePrefix + fileType, ios::binary);
    std::ofstream ofs(fileDir + "\\" + filePrefix + "_readed" + fileType, ios::binary);

    if(ifs){
        ifs.seekg(0, ifs.end);
        int length = (int) ifs.tellg();
        ifs.seekg(0,ifs.beg);
        std::string buffer(BUFFER_SIZE, '\0');
        int offset = 0;
        int readSize = min(BUFFER_SIZE, length - offset);
        while(readSize > 0 && ifs.read(&buffer[0], readSize)){
            ofs.write(&buffer[0], readSize);
            offset += readSize;
            readSize = min(BUFFER_SIZE, length - offset);
    //		std::cout<<"offset:"<< offset << std::endl;
        }
    }
    struct stat info;

    stat((fileDir + "\\"+filePrefix + fileType).c_str(), &info);

    qDebug()<<"The File Size is:\t\t "<<(double)info.st_size<<endl;
    ifs.close();
    ofs.close();
    endTime = clock();
    this->readTime = ((double)endTime - startTime)/CLOCKS_PER_SEC;
    qDebug()<<"The Precessing Time Is :\t "<<((double)endTime - startTime)/CLOCKS_PER_SEC << "s!\n"<<endl;
}

void DataRead::getBegin(){
    qDebug()<<"Start Precssion! "<<endl;
//    readData();
    switch (this->modelSelected) {
        case 0:
            readData();
            break;
        case 1:
            readData2();
            break;
        case 2:
            readData3();
            break;
        default:
            break;
    }
//    readData2();
}
