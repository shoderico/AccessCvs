#include "getcomobject.h"

#include <QDebug>

#include <windows.h>
#include <tlhelp32.h>
#include <oleacc.h>


#include <ActiveQt/QAxObject>

#include "pch.hpp"

GetComObject::GetComObject(const QString &targetExeName, const QString &targetWindowClassName)
    : m_targetExeName( targetExeName )
    , m_targetWndClassName( targetWindowClassName )
{
}

QString GetComObject::tartgetExeName() const
{
    return m_targetExeName;
}

void GetComObject::setTargetExeName(const QString &targetExeName)
{
    m_targetExeName = targetExeName;
}

QString GetComObject::targetWindowClassName() const
{
    return m_targetWndClassName;
}

void GetComObject::setTargetWindowClassName(const QString &targetWindowClassName)
{
    m_targetWndClassName = targetWindowClassName;
}





int GetProcessIdList(QList<DWORD> &processIdList, const QString &targetExeName)
{
    PROCESSENTRY32 pe;
    HANDLE hSnap;
    BOOL bResult;

    processIdList.clear();

    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe.dwSize = sizeof( PROCESSENTRY32 );

    bResult = Process32First( hSnap, &pe );

    while ( bResult )
    {
        QString exeFile =  QString::fromWCharArray( pe.szExeFile );

        if ( exeFile.toUpper() == targetExeName.toUpper() )
        {
            processIdList.append( pe.th32ProcessID );
        }

        bResult = Process32Next(hSnap, &pe);
    }

    CloseHandle(hSnap);

    return processIdList.size();
}


BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam)
{
     QList<HWND> *pList = ( QList<HWND> *)lParam;
     pList->append( hWnd );
     return TRUE;
}


int GetProcessHWndList(const QList<DWORD> &processIdList, const QString &targetWndClassName, QList<HWND> &hWndList)
{
    QList<HWND> allhWndList;
    EnumWindows(EnumWindowsProc, (LPARAM)&allhWndList);

    hWndList.clear();
    foreach( DWORD processId, processIdList)
    {
        foreach( HWND hWnd, allhWndList)
        {
            DWORD processIdForHWnd = 0;
            if ( GetWindowThreadProcessId( hWnd, &processIdForHWnd ) != 0)
            {
                if ( processIdForHWnd == processId )
                {
                    TCHAR classNameApi[MAX_PATH];
                    GetClassName( hWnd, classNameApi, _countof(classNameApi) );

                    QString className = QString::fromWCharArray( classNameApi );
                    if ( className == targetWndClassName)
                    {
                        hWndList.append( hWnd );
                    }
                }
            }
        }
    }

    return hWndList.size();
}



QAxObject *GetComObjectFromHWnd(HWND hWnd)
{

    // MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
    // IUnknown *pIUnknown;

    // MIDL_INTERFACE("00020400-0000-0000-c000-000000000046")
    IDispatch *pIDispatch;

    HRESULT hr;
    hr = AccessibleObjectFromWindow( hWnd, OBJID_NATIVEOM, IID_IDispatch, (void**)&pIDispatch );
    if ( hr != S_OK )
        return NULL;

    QAxObject *pComObject = new QAxObject( pIDispatch );
    pIDispatch->Release();

    return pComObject;
}





QList<QAxObject *> GetComObject::getComObjects()
{
    QList<DWORD> processIdList;
    QList<HWND> hWndList;

    QList<QAxObject *> list;

    if ( GetProcessIdList( processIdList, m_targetExeName ) )
    {
        if ( GetProcessHWndList( processIdList, m_targetWndClassName, hWndList ))
        {
            foreach( HWND hWnd, hWndList)
            {
                QAxObject *pComObject = GetComObjectFromHWnd( hWnd );
                if ( pComObject )
                    list.append( pComObject );
            }
        }
    }

    return list;
}

