#include "MainWindow.h"
#include "ui_MainWindow.h"

#define MAX_KEY_LENGTH 255

#include <QtDebug>
#include <Windows.h>
#include <WtsApi32.h>
#include <strsafe.h>

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "User32.lib")


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->ClearPushButton, SIGNAL(pressed()), this, SLOT(ClearRegistry()));
    QObject::connect(ui->AutoClearPushButton, SIGNAL(pressed()), this, SLOT(ClearRegistry2()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClearRegistry2()
{
    qDebug() << "11";

    HKEY hKey;

    DWORD value;
    DWORD BufferSize = sizeof(DWORD);
    
    TCHAR lptstrSearchScope[MAX_KEY_LENGTH] = TEXT("Software\\OU_01_504ac2ff4962402ea1a02e7bf85fdfaa");

    LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, lptstrSearchScope, 0, KEY_READ | KEY_WRITE, &hKey);
    if (res == ERROR_SUCCESS)
    {
        qDebug() << "22";
        
        res = RegGetValue(hKey,
            NULL,
            TEXT("ts"),
            RRF_RT_ANY,
            NULL,
            (PVOID)&value,
            &BufferSize
            );
        
        qDebug() << res;
        qDebug() << value;
    }
}


void MainWindow::ClearRegistry()
{
    qDebug() << "1";

    HKEY hKey;

    TCHAR lptstrKeyPath[MAX_KEY_LENGTH];
    TCHAR lptstrSearchScope[MAX_KEY_LENGTH] = TEXT("Software");

    LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, lptstrSearchScope, 0, KEY_READ | KEY_WRITE, &hKey);
    if (res == ERROR_SUCCESS)
    {
        qDebug() << "2";

        TCHAR HKUClass[MAX_PATH] = TEXT("");
        DWORD dwcchClass = MAX_PATH;
        DWORD dwcSubKeys = 0;

        res = RegQueryInfoKey (hKey,
            HKUClass,
            &dwcchClass,
            NULL,
            &dwcSubKeys,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        );

        if(res == ERROR_SUCCESS) {
            
            QStringList NNRusMutexsStringList;

            for (int i=0; i<dwcSubKeys; i++) {

                DWORD dwcbName = MAX_KEY_LENGTH;
                res = RegEnumKeyEx(hKey, i,
                    lptstrKeyPath,
                    &dwcbName,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                );

                if(res == ERROR_SUCCESS) {
                    
                    TCHAR lptstrNNRusMutexPath[MAX_KEY_LENGTH];
                    StringCchPrintf(lptstrNNRusMutexPath, MAX_KEY_LENGTH, TEXT("%s\\%s"), lptstrSearchScope, lptstrKeyPath);
                    
                    HKEY hKeyNNRusMutex;
                    LONG res = RegOpenKeyEx(HKEY_CURRENT_USER,
                        lptstrNNRusMutexPath,
                        0,
                        KEY_READ | KEY_WRITE,
                        &hKeyNNRusMutex
                    );

                    if (res == ERROR_SUCCESS) {
                        DWORD ts = 0;
                        DWORD dwSize = sizeof(DWORD);
                        
                        res = RegGetValue(hKeyNNRusMutex,
                            NULL,
                            TEXT("ts"),
                            RRF_RT_ANY,
                            NULL,
                            (PVOID)&ts,
                            &dwSize
                        );
                        
                        if(res == ERROR_SUCCESS) {
                            
                            NNRusMutexsStringList.append(QString::fromWCharArray(lptstrNNRusMutexPath));
                            
                        }

                    }
                    
                    RegCloseKey(hKeyNNRusMutex);
                    
                    
                    
                }
            
            }
            
            foreach (const QString &str, NNRusMutexsStringList) {
                qDebug() << "+++";
                qDebug() << str;
                qDebug() << "-------------------";
                
                /*
                res = RegDeleteTree(
                    HKEY_CURRENT_USER,
                    lptstrFullPath
                );
                
                qDebug() << res;
                */
                
            }
            
        }

    }
}
