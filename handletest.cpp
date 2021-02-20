#include "handletest.h"
#include<QString>
#include<QDebug>
#include<iostream>
#include<windows.h>
#include<winbase.h>
#include <stdio.h>
#include <wchar.h>
#include<string>


#define BUFFER_SIZE 10
//#define FILE_MAP_START 0

using namespace std;

HandleTest::HandleTest(){}



void HandleTest::handle(){
    HANDLE hFile;
    HANDLE hMapFile;

    DWORD dwSysGran;
    DWORD dwFileSize;
    DWORD dwFileMapSize;
    DWORD dwMapViewSize;
    DWORD dwFileMapStart;

    SYSTEM_INFO sysInfo;
    LPVOID lpMapAddress;
    BOOL bFlag;
    int iViewDelta;
    int iData;


    WCHAR wcFileName[256];
    WCHAR wcShareName[256];

    string strFileDir = this->fileDir.toStdString();
    const char *temp1 = (char *)strFileDir.c_str();
    MultiByteToWideChar(CP_ACP,
                        0,
                        temp1,
                        strlen(temp1) + 1,
                        wcFileName,
                        sizeof(wcFileName)/sizeof(wcFileName[0]));
    string stemp = "Resource";

    const char * temp2 =  (char*)stemp.c_str();
    MultiByteToWideChar(CP_ACP,
                        0,
                        temp2,
                        strlen(temp2)+1,
                        wcShareName,
                        sizeof(wcShareName)/sizeof(wcShareName[0]));

    GetSystemInfo(&sysInfo);
    dwSysGran = sysInfo.dwAllocationGranularity;

    hFile = CreateFile(wcFileName,
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_ALWAYS,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);
    if(hFile == NULL){
        qDebug()<<"CreateFile failed : "<<GetLastError();
    }

    dwFileSize = GetFileSize(hFile, NULL);

    dwFileMapStart =((dwFileSize - 10) / dwSysGran)  * dwSysGran;
    dwMapViewSize = ((dwFileSize - 10) % dwSysGran) + BUFFER_SIZE;

    dwFileMapSize = (dwFileSize - 10) + BUFFER_SIZE;
    iViewDelta = dwFileMapSize - (dwFileSize - 10);


//    dwFileMapStart =(FILE_MAP_START / dwSysGran)  * dwSysGran;
//    dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFER_SIZE;

//    dwFileMapSize = FILE_MAP_START + BUFFER_SIZE;
//    iViewDelta = dwFileMapSize - FILE_MAP_START;



//    dwFileSize = GetFileSize(hFile, NULL);
    hMapFile = CreateFileMapping(hFile,
                                 NULL,
                                 PAGE_READWRITE,
                                 0,
                                 dwFileMapSize,
                                 NULL);
    if(hMapFile == NULL) {
        qDebug()<<"CreateFileMapping failed : " << GetLastError();
    }

    lpMapAddress = MapViewOfFile(hMapFile,
                                 FILE_MAP_ALL_ACCESS,
                                 0,
                                 dwFileMapStart,
                                 dwMapViewSize);
    if(lpMapAddress == NULL){
        qDebug()<<"MapViewOfFile Failed : " << GetLastError();
    }

    char read_chars[128] = "";
    memcpy(read_chars, (char *)lpMapAddress + iViewDelta, (size_t)128);

    qDebug()<<"Read Chars : "<< read_chars;
//    char * pData;
//    pData = (char *)lpMapAddress;
//    pData = (char *)lpMapAddress + iViewDelta;
//    char charTemp[BUFFER_SIZE] = "";


    bFlag = UnmapViewOfFile(lpMapAddress);
    if(bFlag == NULL) {
        qDebug()<<"UnMapViewOfFile failed.  "<<GetLastError();
    }
    bFlag = CloseHandle(hMapFile);
    if(bFlag == NULL) {
        qDebug()<<"UnMapViewOfFile failed.  "<<GetLastError();
    }
    bFlag = CloseHandle(hFile);
    if(bFlag == NULL) {
        qDebug()<<"UnMapViewOfFile failed.  "<<GetLastError();
    }
    qDebug()<<"Headle Test Succeess";
}



/*#include "handletest.h"
#include<QString>
#include<QDebug>
#include<iostream>
#include<assert.h>

#include<Windows.h>
#include<winBase.h>

#define BAD_POS 0xFFFFFFFF
#define SUCCESS 0

typedef DWORD mmf_share_mode;
typedef DWORD mmf_access_mode;
typedef DWORD mmf_flags;

using namespace std;

HandleTest::HandleTest()
{
}

void HandleTest::handle(){

//    WCHAR wcFileName[256];
//    WCHAR wcResource[256];

    char *szStr = (char *)this->fileDir.toStdString().c_str();
    WCHAR wszClassName[256];
    memset(wszClassName, 0, sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName, sizeof(wszClassName)/sizeof(wszClassName[0]));

    char *szStr2 = (char *)"Resource";
    WCHAR wszClassName2[256];
    memset(wszClassName2, 0, sizeof(wszClassName2));
    MultiByteToWideChar(CP_ACP, 0, szStr2, strlen(szStr2)+1, wszClassName2, sizeof(wszClassName2)/sizeof(wszClassName2[0]));

    const WCHAR * file_name = wszClassName;
    const WCHAR * shared_name = wszClassName2;

    const DWORD mmf_size = 512 * 1024;
//    const DWORD mmf_size = 1 * 1024;

    //存取
    mmf_share_mode share_mode = (FILE_SHARE_READ | FILE_SHARE_WRITE);
    mmf_access_mode access_mode = (GENERIC_READ | GENERIC_WRITE);
    mmf_flags flags = FILE_FLAG_SEQUENTIAL_SCAN ;
    DWORD error_code;



    //创建文件句柄
    HANDLE mmHandle = CreateFile(file_name, access_mode, share_mode, NULL, OPEN_ALWAYS, flags, NULL);
    if(mmHandle == INVALID_HANDLE_VALUE){
        error_code = GetLastError();
        qDebug()<<"创建 mmf 失败: "<< error_code;
    }else{
        DWORD high_size;
        DWORD file_size = GetFileSize(mmHandle, &high_size);
        if(file_size == BAD_POS && (error_code = GetLastError()) != SUCCESS){
            CloseHandle(mmHandle);
            qDebug()<<"error: "<<error_code;
        }
        DWORD size_high = 0;
//        HANDLE mmfm = CreateFileMapping(mmHandle, NULL, PAGE_READWRITE, size_high, mmf_size, shared_name);
        HANDLE mmfm = CreateFileMapping(mmHandle, NULL, PAGE_READWRITE, size_high, mmf_size + 10 , shared_name);
        qDebug()<<"mmf_size : "<<mmf_size;

        error_code = GetLastError();
        if(SUCCESS != error_code){
            qDebug()<<"Create File Mapping Error" << error_code;
        }else{
            if(mmfm == NULL){
                if(mmHandle != INVALID_HANDLE_VALUE){
                    CloseHandle(mmHandle);
                }
            }else{
                size_t  view_size = 1024 * 256;
                DWORD view_access = FILE_MAP_ALL_ACCESS;

//                char * mmfm_base_address = (char *)MapViewOfFile(mmfm, view_access, 0, 0, view_size);
                PVOID mmfm_base_address = MapViewOfFile(mmfm, view_access, 0, 0 ,mmf_size);
                if(mmfm_base_address == NULL){
                    error_code = GetLastError();
                    if(error_code != SUCCESS){
                        qDebug()<<"error code : "<< error_code;
                    }
                }else{
                    char * write_chars = (char *)("ABCDEFG");
                    const size_t write_chars_size = sizeof(write_chars);
//                    CopyMemory(mmfm_base_address, write_chars, write_chars_size);
                    memcpy((char *)mmfm_base_address, write_chars, write_chars_size);

                    FlushViewOfFile(mmfm_base_address, write_chars_size);
                    qDebug()<<"mmfm_base_address : "<<mmfm_base_address;

                    size_t  position = 0;
                    char read_chars[write_chars_size] = "";
                    memcpy(read_chars, mmfm_base_address, write_chars_size);

                    qDebug()<<"Read Chars : "<< read_chars;
                    UnmapViewOfFile(mmfm_base_address);
                    CloseHandle(mmfm);
                    CloseHandle(mmHandle);
                }
            }
        }

    }


}
*/

/*#include "handletest.h"
#include <iostream>
#include <QString>

#include <QDebug>
#include <assert.h>
#include <windows.h>
#include <winBase.h>
#include <stdio.h>
#include <wchar.h>

HandleTest::HandleTest() {}

using namespace std;
#define BUFFSIZE 1024
#define FILE_MAP_START 138240  //文件起始位置

TCHAR * lpcTheFile = TEXT("fmtest.txt");


void HandleTest::handle(){
    HANDLE hMapFile;
    HANDLE hFile;
    BOOL bFlag;
    DWORD dwBytesWritten;
    DWORD dwFileSize;
    DWORD dwFileMapSize;
    DWORD dwMapViewSize;
    DWORD dwFileMapStart;
    DWORD dwSysGran;
    SYSTEM_INFO SysInfo;
    LPVOID lpMapAddress;

    char * pData;
    int i;
    int iData;
    int iViewDelta;

    hFile = CreateFile(lpcTheFile,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       CREATE_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);

    if(hFile == INVALID_HANDLE_VALUE){
        qDebug()<<"hFile is NULL \n";
        qDebug()<<"Targe file is "<< lpcTheFile;
//        return ;
    }

    GetSystemInfo(&SysInfo);
    dwSysGran = SysInfo.dwAllocationGranularity;

    dwFileMapStart = (FILE_MAP_START / dwSysGran) * dwSysGran;
    qDebug() << "The file map view of start at " << dwFileMapStart <<" bytes into the file. ";

    dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFSIZE;
    qDebug() << "The file map view is " << dwMapViewSize << " bytes large. ";

    dwFileMapSize = FILE_MAP_START + BUFFSIZE;
    qDebug() << "The file mapping object is "<< dwFileMapSize <<"bytes large.";

    iViewDelta = FILE_MAP_START - dwFileMapStart;
    qDebug() <<"The data is "<< iViewDelta << " bytes into the view. ";

    for(i = 0; i < (int)dwSysGran; ++i){
        WriteFile(hFile, &i, sizeof(i), &dwBytesWritten, NULL);
    }

    dwFileSize = GetFileSize(hFile, NULL);
    qDebug()<<"hFile size " <<dwFileSize;

    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwFileMapSize, NULL);
    if(hMapFile == NULL){
        qDebug()<<"hMapFile is NULL : last error "<<hMapFile;
    }

    lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCaESS, 0, dwFileMapStart, dwMapViewSize);
    if(lpMapAddress == NULL)
    {
        qDebug()<<"lpMapAddres is NULL : last error: "<< GetLastError();
    }

    pData = (char *) lpMapAddress + iViewDelta;
    iData = *(int *) pData;


    qDebug()<<"The value at the pointer is "<<iData <<" which "<<iData * 4 << "FILE_START_MAP: " << FILE_MAP_START;

    bFlag = UnmapViewOfFile(lpMapAddress);
    bFlag = CloseHandle(hMapFile);

    if(!bFlag) qDebug()<<"Error "<<GetLastError()<<" occurred closing the mapping object";
    bFlag = CloseHandle(hFile);
    if(!bFlag) qDebug()<<"Error" << GetLastError()<<" occurred closing the file";
}

*/

/*#include <iostream>
#include <assert.h>

#include <Windows.h>
#include <WinBase.h>

#define BAD_POS 0xFFFFFFFF // returned by SetFilePointer and GetFileSize
#define SUCCESS 0
using namespace std;


typedef DWORD mmf_share_mode;
typedef DWORD mmf_access_mode;
typedef DWORD mmf_flags;


void HandleTest::handle() {

//    char szStr[] = "D:\\QT\\build-BigDataSetRead-Desktop_Qt_5_14_2_MinGW_64_bit-Debug\\100MB.txt";
    char *szStr = (char *)this->fileDir.toStdString().c_str();
    WCHAR  wszClassName[256];
    memset(wszClassName, 0, sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));

    char szstr2[] = "Resource";
    WCHAR  wszClassName2[256];
    memset(wszClassName2, 0, sizeof(wszClassName2));
    MultiByteToWideChar(CP_ACP, 0, szstr2, strlen(szstr2) + 1, wszClassName2, sizeof(wszClassName2) / sizeof(wszClassName2[0]));


//    cout << "create memorymapfile..." << endl;
//    const char* shared_name = "testMmf";
//    const char* file_name = "d:\\testMmf.mmf";
    WCHAR* shared_name = wszClassName2;
    WCHAR* file_name = wszClassName;

    const DWORD mmf_size = 512 * 1024;
    //存取模式
    mmf_access_mode access_mode = (GENERIC_READ | GENERIC_WRITE);
    //共享模式
    mmf_share_mode share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    //文件属性
    mmf_flags flags = FILE_FLAG_SEQUENTIAL_SCAN;//|FILE_FLAG_WRITE_THROUGH|FILE_FLAG_NO_BUFFERING;
    DWORD error_code;

    //创建文件
    HANDLE mmHandle =
        CreateFile(file_name,
            access_mode,
            share_mode,
            NULL,
            OPEN_ALWAYS,
            flags,
            NULL);

    if (mmHandle == INVALID_HANDLE_VALUE) {
        error_code = GetLastError();
        cout << "创建mmf失败:" << error_code << endl;
    }
    else {
        DWORD high_size;
        DWORD file_size = GetFileSize(mmHandle, &high_size);
        if (file_size == BAD_POS && (error_code = GetLastError()) != SUCCESS) {
            CloseHandle(mmHandle);
            cout << "error：" << error_code << endl;
        }
//        cout << "create mmf sucessfully" << endl;

        //assert(file_size == 0);

        DWORD size_high = 0;
        //创建文件映射，如果要创建内存页面文件的映射，第一个参数设置为INVALID_HANDLE_VALUE
        HANDLE mmfm = CreateFileMapping(mmHandle,
            NULL,
            PAGE_READWRITE,
            size_high,file_size,
//            mmf_size,
            shared_name);

        error_code = GetLastError();
        if (SUCCESS != error_code) {
            cout << "createFileMapping error" << error_code << endl;
        }
        else {
            if (mmfm == NULL) {
                if (mmHandle != INVALID_HANDLE_VALUE) {
                    CloseHandle(mmHandle);
                }
            }
            else {
                //char write_chars[] = "hello chars";
                //size_t position = 0;
                //DWORD written = 0;
                //const size_t write_chars_size = sizeof(write_chars);
                //WriteFile(mmHandle,write_chars,write_chars_size,&written,NULL);
                size_t view_size = 1024 * 256;
                DWORD view_access = FILE_MAP_ALL_ACCESS;

                //获得映射视图
                char* mmfm_base_address = (char*)MapViewOfFile(mmfm, view_access, 0, 0, view_size);
                if (mmfm_base_address == NULL) {
                    error_code = GetLastError();
                    if (error_code != SUCCESS) {
                        cout << "error code " << error_code << endl;
                    }
                }
                else {
                    char write_chars[] = "hello chars";
                    const size_t write_chars_size = sizeof(write_chars);
                    //向内存映射视图中写数据
                    CopyMemory((PVOID)mmfm_base_address, write_chars, write_chars_size);

                    CopyMemory((PVOID)(mmfm_base_address + size_high-write_chars_size), write_chars, write_chars_size);
                    //memcpy(mmfm_base_address,write_chars,write_chars_size);

                    size_t position = 0;
                    char read_chars[write_chars_size];

                    //读数据
                    memcpy(read_chars, mmfm_base_address, write_chars_size);
//                    cout << "read chars " << read_chars << endl;

                    //卸载映射
                    UnmapViewOfFile(mmfm_base_address);
                    //关闭内存映射文件
                    CloseHandle(mmfm);
                    //关闭文件
                    CloseHandle(mmHandle);
                }
            }
        }
    }

//    system("pause");
//    exit(0);
    //return EXIT_SUCCESS;
}*/
