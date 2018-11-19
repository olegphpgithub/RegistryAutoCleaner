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

}

MainWindow::~MainWindow()
{
    delete ui;
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
            qDebug() << dwcSubKeys;

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

                    TCHAR lptstrFullPath[MAX_KEY_LENGTH];
                    StringCchPrintf(lptstrFullPath, MAX_KEY_LENGTH, TEXT("%s\\%s"), lptstrSearchScope, lptstrKeyPath);
                    MessageBox(NULL, lptstrFullPath, TEXT("3"), MB_OK);

                    HKEY hKeyNNRusMutex;
                    LONG res = RegOpenKeyEx(HKEY_CURRENT_USER,
                        lptstrFullPath,
                        0,
                        KEY_READ | KEY_WRITE,
                        &hKeyNNRusMutex
                    );

                    if (res == ERROR_SUCCESS) {
                        //CHAR pvDataText[MAX_VALUE_NAME];
                        //DWORD pcbData = MAX_VALUE_NAME;
                        DWORD ts = 0;
                        DWORD dwSize = sizeof(DWORD);

                        res = RegQueryValueEx(hKeyNNRusMutex, TEXT("ts"), NULL, NULL, (LPBYTE)&ts, &dwSize);

                        if(res = ERROR_SUCCESS) {
                            MessageBox(NULL, TEXT("found"), TEXT("4"), MB_OK);
                        }

                    }


                }

            }
        }

    }
}
