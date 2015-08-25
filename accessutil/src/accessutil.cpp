#include "accessutil.h"

#include "util/officeutil.h"
#include "util/getcomobject.h"

#include <QProcess>
#include <QAxObject>

#include <windows.h>
#include <tlhelp32.h>
#include <winuser.h>


// http://www.codeproject.com/Questions/78801/How-to-get-the-main-thread-ID-of-a-process-known-b
#ifndef MAKEULONGLONG
#define MAKEULONGLONG(ldw, hdw) ((ULONGLONG(hdw) << 32) | ((ldw) & 0xFFFFFFFF))
#endif

#ifndef MAXULONGLONG
#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#endif

DWORD GetThreadIdFromProcessId(DWORD dwProcID)
{
    DWORD dwMainThreadID = 0;
    ULONGLONG ullMinCreateTime = MAXULONGLONG;

    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 th32;
        th32.dwSize = sizeof(THREADENTRY32);
        BOOL bOK = TRUE;
        for (bOK = Thread32First(hThreadSnap, &th32); bOK; bOK = Thread32Next(hThreadSnap, &th32))
        {
            if (th32.th32OwnerProcessID == dwProcID)
            {
                HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, TRUE, th32.th32ThreadID);
                if (hThread)
                {
                    FILETIME afTimes[4] = {0};
                    if (GetThreadTimes(hThread, &afTimes[0], &afTimes[1], &afTimes[2], &afTimes[3]))
                    {
                        ULONGLONG ullTest = MAKEULONGLONG(afTimes[0].dwLowDateTime, afTimes[0].dwHighDateTime);
                        if (ullTest && ullTest < ullMinCreateTime)
                        {
                            ullMinCreateTime = ullTest;
                            dwMainThreadID = th32.th32ThreadID;
                        }
                    }
                    CloseHandle(hThread);
                }
            }
        }
        CloseHandle(hThreadSnap);
    }

    return dwMainThreadID;
}


// http://d.hatena.ne.jp/yu-hr/20100323/1269355469
HWND GetWindowHandle(const DWORD TargetID)
{
    HWND hWnd = GetTopWindow(NULL);
    do {
        if (GetWindowLong( hWnd, GWL_HWNDPARENT ) != 0 || !IsWindowVisible( hWnd ) )
            continue;
        DWORD ProcessID;
        GetWindowThreadProcessId( hWnd, &ProcessID );
        if(TargetID == ProcessID)
            return hWnd;
    } while ( (hWnd = GetNextWindow( hWnd, GW_HWNDNEXT)) != NULL);

    return NULL;
}

bool AccessUtil::decompile(const QString &fileName)
{
    QString exePath = OfficeUtil::getExePath("Access.Application");
    QStringList arguments;
    arguments << "/decompile" << QString("%1").arg(fileName);

    QProcess process;
    process.start(exePath, arguments);

    if (process.waitForStarted())
    {
        quint64 targetProcessId = process.processId();

        DWORD targetThreadId = GetThreadIdFromProcessId( (DWORD)targetProcessId );
        DWORD currentThreadId = GetCurrentThreadId();


        if (targetThreadId) {

            // attach input
            BOOL ret = 0;
            while (ret == 0)
                ret = AttachThreadInput(currentThreadId, targetThreadId, TRUE);

            // find hWNd
            HWND hwnd = NULL;
            while (hwnd == NULL)
                hwnd = GetWindowHandle(targetProcessId);

            // set focus
            SetForegroundWindow(hwnd);
            SetFocus(hwnd);


            QAxObject *application = NULL;

            BYTE keyStateSrc[256];
            BYTE keyStateDst[256];
            if ( GetKeyboardState(keyStateSrc) && GetKeyboardState(keyStateDst) )
            {
                // simulate Shift key pressed
                keyStateDst[VK_SHIFT] = 0x80;
                SetKeyboardState(keyStateDst);

                // wait for boot complete

                // This is ugly.
                //Sleep(1000);

                // This is too short.
                /*
                HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
                WaitForInputIdle( processHandle, 5000 );
                CloseHandle(processHandle);
                */

                // Get com object and compare its' file path. it works.
                GetComObject gco("MSACCESS.EXE", "OMain");
                bool found = false;
                while (!found)
                {
                    QList<QAxObject*> objects = gco.getComObjects();
                    //qDebug() << "objects.count()" << objects.count();
                    if (objects.count() == 2) found = true;
                    foreach (QAxObject *object, objects)
                    {
                        QAxObject *currentProject = object->querySubObject("CurrentProject()");
                        if (currentProject)
                        {
                            QString fullName = currentProject->property("FullName").toString();

                            if (fullName.toUpper() == QString(fileName).toUpper())
                            {
                                found = true;
                                application = object;
                            }
                            delete currentProject;
                        }
                        if (object != application)
                            delete object;
                    }
                }

                // restore keyboard state
                SetKeyboardState(keyStateSrc);
            }

            // detach input
            AttachThreadInput(currentThreadId, targetThreadId, FALSE);

            // quit
            if (application)
            {
                application->dynamicCall("Quit()");
                delete application;
            }
        }
    }

    if (process.state() == QProcess::Running)
        process.terminate();

    return true;
}

bool AccessUtil::compactRepair(const QString &fileName)
{
    Q_UNUSED(fileName)
    return true;
}
