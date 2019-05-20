#include "MainWindow.h"
#include "ui_MainWindow.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#include <QtDebug>
#include <QDateTime>

#include "SimpleTimer.h"

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
    QObject::connect(ui->AutoClearPushButton, SIGNAL(pressed()), this, SLOT(AutoClearStartStop()));
    bAutoClear = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AutoClearStartStop()
{
    bAutoClear = (bAutoClear == true) ? false : true;

    if(bAutoClear) {
        ui->AutoClearPushButton->setText(QObject::tr("Auto Clear (Stop)"));
        stimer = new SimpleTimer(this);
        stimer->setTimeout(ui->IntervalSpinBox->value());
        QObject::connect(stimer, SIGNAL(finished()), this, SLOT(TimeoutExceeded()));
        stimer->start();
    } else {
        ui->AutoClearPushButton->setText(QObject::tr("Auto Clear (Start)"));
        QObject::disconnect(stimer, SIGNAL(finished()), this, SLOT(TimeoutExceeded()));
        stimer->terminate();
        stimer->deleteLater();
    }
}

void MainWindow::TimeoutExceeded()
{
    ClearRegistry();
    if(bAutoClear) {
        if(stimer != NULL) {
            QObject::disconnect(stimer, SIGNAL(finished()), this, SLOT(TimeoutExceeded()));
            delete stimer;
        }
        stimer = new SimpleTimer(this);
        stimer->setTimeout(ui->IntervalSpinBox->value());
        QObject::connect(stimer, SIGNAL(finished()), this, SLOT(TimeoutExceeded()));
        stimer->start();
    }
}


void MainWindow::ClearRegistry()
{

    HKEY hKey;

    TCHAR lptstrKeyPath[MAX_KEY_LENGTH];
    TCHAR lptstrSearchScope[MAX_KEY_LENGTH] = TEXT("Software");

    DWORD lpcdwSubKeys2 = 0;
    DWORD lpcdwSubValues2 = 0;

    TCHAR lptstrValueName[MAX_VALUE_NAME];
    DWORD dwcchValueName = MAX_VALUE_NAME;

    LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, lptstrSearchScope, 0, KEY_READ | KEY_WRITE, &hKey);
    if (res == ERROR_SUCCESS)
    {

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

            for (DWORD i=0; i<dwcSubKeys; i++) {

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

                        /** +++++ Registry key must have only one value */

                        res = RegQueryInfoKey (hKeyNNRusMutex,
                            NULL,
                            NULL,
                            NULL,
                            &lpcdwSubKeys2,
                            NULL,
                            NULL,
                            &lpcdwSubValues2,
                            NULL,
                            NULL,
                            NULL,
                            NULL
                        );

                        if( (lpcdwSubKeys2 == 0) && (lpcdwSubValues2 == 1) ) {

                            /** Name must have 2 chars */

                            dwcchValueName = MAX_VALUE_NAME;
                            ZeroMemory(lptstrValueName, MAX_VALUE_NAME);

                            res = RegEnumValue(hKeyNNRusMutex,
                                0,
                                lptstrValueName,
                                &dwcchValueName,
                                NULL,
                                NULL,
                                NULL,
                                NULL
                            );

                            if(res == ERROR_SUCCESS) {
                                if(dwcchValueName == 2) {
                                    NNRusMutexsStringList.append(QString::fromWCharArray(lptstrNNRusMutexPath));
                                }
                            }

                        }

                    }

                    RegCloseKey(hKeyNNRusMutex);

                }

            }

            foreach (const QString &str, NNRusMutexsStringList) {
                
                TCHAR *lptstrNNRusMutexKeyPath;
                DWORD dwcchNNRusMutexKeyPath = str.length() + 1;
                
                lptstrNNRusMutexKeyPath = new TCHAR[dwcchNNRusMutexKeyPath];
                ZeroMemory(lptstrNNRusMutexKeyPath, dwcchNNRusMutexKeyPath * sizeof(TCHAR));
                
                str.toWCharArray(lptstrNNRusMutexKeyPath);
                
                res = RegDeleteTree(
                    HKEY_CURRENT_USER,
                    lptstrNNRusMutexKeyPath
                );
                
                if(res == ERROR_SUCCESS) {
                    QDateTime currentDateTime = QDateTime::currentDateTime();
                    QString dateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
                    QString logString("%1 %2 was removed successfully");
                    logString = logString.arg(dateTimeString);
                    logString = logString.arg(str);
                    ui->LogTextEdit->append(logString);
                }
                
                delete lptstrNNRusMutexKeyPath;
                
            }
            
        }

    }
}
