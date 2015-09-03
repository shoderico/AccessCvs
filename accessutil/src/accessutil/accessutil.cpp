#include "accessutil.h"

#include "util/officeutil.h"
#include "util/getcomobject.h"
#include "officelib/officelib.h"

#include <QDebug>
#include <QProcess>
#include <QAxObject>
#include <QFile>
#include <QTemporaryFile>

#include <windows.h>
#include <tlhelp32.h>
#include <winuser.h>
#include <oleacc.h>


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

bool AccessUtil::decompile(const QString &fileName, quint64 threadIdForAttachInput)
{
    if (!QFile(fileName).exists())
        return false;

    QString exePath = OfficeUtil::getExePath("Access.Application");
    QStringList arguments;
    arguments << "/decompile" << QString("%1").arg(fileName);

    QProcess process;
    process.start(exePath, arguments);
    bool quitted = false;

    if ( !process.waitForStarted() )
        qDebug() << "process.waitForStarted() timeout";
    else
    {

        // find target hWnd and ProcessId
        HWND targetHwnd = NULL;
        DWORD targetThreadId = 0;

        // find from processId
        {
            quint64 targetProcessId = process.processId();
            if ( !targetProcessId )
                qDebug() << "process.processId() failed";

            targetThreadId = GetThreadIdFromProcessId( (DWORD)targetProcessId );
            if ( !targetThreadId )
                qDebug() << "GetThreadIdFromProcessId( (DWORD)targetProcessId ) failed";

            while (targetHwnd == NULL)
            {
                targetHwnd = GetWindowHandle(targetProcessId);
                // this cause virus detection
                //if (targetHwnd == NULL)
                //    qDebug() << "GetWindowHandle(targetProcessId) failed. try again";
            }
        }
        // find from hWnd
        if (false)
        {
            DWORD targetProcessId = 0;
            targetThreadId = GetWindowThreadProcessId( targetHwnd, &targetProcessId);
        }

        // threadId for input attach
        DWORD currentThreadId = threadIdForAttachInput;
        if (!currentThreadId)
        {
            currentThreadId = GetCurrentThreadId();
        }


        if (targetThreadId)
        {

            // attach input
            if ( !AttachThreadInput(currentThreadId, targetThreadId, TRUE) )
                qDebug() << "AttachThreadInput(currentThreadId, targetThreadId, TRUE) failed";

            // set focus
            if ( !SetForegroundWindow(targetHwnd) )
                qDebug() << "SetForegroundWindow(targetHwnd) failed";
            SetFocus(targetHwnd);

            QAxObject *application = NULL;

            BYTE keyStateSrc[256];
            BYTE keyStateDst[256];
            if ( !( GetKeyboardState(keyStateSrc) && GetKeyboardState(keyStateDst) ) )
                qDebug() << "GetKeyboardState(keyStateSrc) && GetKeyboardState(keyStateDst) failed";
            else
            {
                // simulate Shift key pressed
                keyStateDst[VK_SHIFT] = 0x80;
                if ( !SetKeyboardState(keyStateDst) )
                    qDebug() << "SetKeyboardState(keyStateDst) failed";

                // wait for boot complete
                {
                    IDispatch *pIDispatch = NULL; // MIDL_INTERFACE("00020400-0000-0000-c000-000000000046")
                    HRESULT hr = AccessibleObjectFromWindow( targetHwnd, OBJID_NATIVEOM, IID_IDispatch, (void**)&pIDispatch );
                    if ( hr != S_OK )
                        qDebug() << "AccessibleObjectFromWindow(targetHwnd, ...) failed";
                    else
                    {
                        application = new QAxObject( pIDispatch );
                        pIDispatch->Release();

                        bool found = false;
                        while (!found)
                        {
                            QAxObject *currentProject = application->querySubObject("CurrentProject()");
                            if (currentProject)
                            {
                                QString fullName = currentProject->property("FullName").toString();
                                delete currentProject;

                                if (fullName.toUpper() == fileName.toUpper())
                                    found = true;
                            }
                        } // while (!found)
                    }
                }

                // restore keyboard state
                if ( !SetKeyboardState(keyStateSrc) )
                    qDebug() << "SetKeyboardState(keyStateSrc) failed";
            }

            // detach input
            if ( !AttachThreadInput(currentThreadId, targetThreadId, FALSE) )
                qDebug() << "AttachThreadInput(currentThreadId, targetThreadId, FALSE) failed";

            // quit
            if (application)
            {
                application->dynamicCall("Quit()");
                delete application;
                quitted = true;
            }
        }
    }

    if (!quitted && process.state() == QProcess::Running)
    {
        process.terminate();
    }
    process.waitForFinished();

    return true;
}

bool AccessUtil::compactRepair(Access::Application *application, const QString &fileName, const int repeatCount)
{
    if (!QFile(fileName).exists())
        return false;

//    Access::Application *application = new Access::Application();

    QString sourceFile = fileName;
    QString destinationFile = "";
    for (int i = 0 ; i < repeatCount ; ++i )
    {
        int temp = 0;
        do {
            destinationFile = QString(fileName + ".temp.%1").arg(++temp);
        }
        while ( QFile(destinationFile).exists() );

        // workaround for error 'Cannot open any more tables.'
        if (i > 0)
        {
            openCurrentDatabase( application, sourceFile );
            application->CloseCurrentDatabase();
        }

        application->CompactRepair( sourceFile, destinationFile );

        if (i > 0)
            QFile(sourceFile).remove();

        sourceFile = destinationFile;
    }

//    application->Quit();
//    delete application;

    QFile(fileName).remove();
    QFile::copy(destinationFile, fileName);
    QFile(destinationFile).remove();

    return true;
}

bool AccessUtil::openCurrentDatabase(Access::Application *application, const QString &fileName)
{
    HWND targetHwnd = (HWND)application->hWndAccessApp();
    DWORD targetProcessId = 0;
    DWORD targetThreadId = GetWindowThreadProcessId( targetHwnd, &targetProcessId);
//    DWORD currentThreadId = GetCurrentThreadId();


    if (targetThreadId)
    {

        // attach input
//        BOOL ret = 0;
//        while (ret == 0)
//            ret = AttachThreadInput(currentThreadId, targetThreadId, TRUE);

        // set focus
        SetForegroundWindow(targetHwnd);
        SetFocus(targetHwnd);

        BYTE keyStateSrc[256];
        BYTE keyStateDst[256];
        if ( GetKeyboardState(keyStateSrc) && GetKeyboardState(keyStateDst) )
        {
            // simulate Shift key pressed
            keyStateDst[VK_SHIFT] = 0x80;
            SetKeyboardState(keyStateDst);

            // open file
            if (fileName.toUpper().endsWith(".ADP"))
                application->OpenAccessProject(fileName);
            else
                application->OpenCurrentDatabase(fileName);

            // restore keyboard state
            SetKeyboardState(keyStateSrc);
        }

        // detach input
//        AttachThreadInput(currentThreadId, targetThreadId, FALSE);
    }

    return true;
}

quint64 AccessUtil::getAccessThreadId(Access::Application *application)
{
    HWND currentHwnd = (HWND)application->hWndAccessApp();
    DWORD currentProcessId = 0;
    DWORD currentThreadId = 0;
    currentThreadId = GetWindowThreadProcessId( currentHwnd, &currentProcessId );
    return currentThreadId;
}

Access::Application *AccessUtil::createAccessApplication(QObject *parent)
{
    Access::Application *application = new Access::Application(parent);
    return application;
}

void AccessUtil::quitAndDeleteApplication(Access::Application *application)
{
    application->Quit();
    delete application;
}
