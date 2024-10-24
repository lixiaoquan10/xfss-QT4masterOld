#include "ctrace.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include <QMutex>
#include <QLibrary>
#include <QDebug>

#ifdef Q_OS_WIN
#ifndef WINVER
    #define WINVER 0x0500
#endif
    #include <windows.h>
    #include <wincon.h>
#endif

typedef void (*__trace_setMainProcessCallback_fun)(__trace_addCallback_fun addFun, __trace_delCallback_fun delFun, __trace_getCallback_fun getFun);

namespace trace {

bool __trace_isInitialization = false;
std::vector<const char*> __trace_functionNames(1000);
int m_currentFunctionIndex = 0;
std::string __trace_name = "Trace";
QMutex* __trace_mutex = new QMutex();

void initialization();
void __trace_show();
void __trace_add(const char *name);
void __trace_del();
int __trace_get(char* data, int len);

__trace_addCallback_fun __trace_addCallback = __trace_add;
__trace_delCallback_fun __trace_delCallback = __trace_del;
__trace_getCallback_fun __trace_getCallback = __trace_get;

#ifdef Q_OS_WIN
LONG WINAPI ExitWithExceptionCode(_EXCEPTION_POINTERS* exception_pointers) {
    Q_UNUSED(exception_pointers);
    __trace_show();
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

#ifdef CTRACE_CLOSE
    const bool __trace_traceIsActive = false;
#else
    const bool __trace_traceIsActive = true;
#endif

CTrace::CTrace(const char *name)
{
    if(__trace_traceIsActive)
    {
        if(!__trace_isInitialization)
            initialization();
        __trace_addCallback(name);
    }
}

CTrace::~CTrace()
{
    if(__trace_traceIsActive)
    {
        __trace_delCallback();
    }
}

void CTrace::traceName(const char *name)
{
    __trace_name = name;
}

void CTrace::addDll(const char *file)
{
    QLibrary myLib(file);
    __trace_setMainProcessCallback_fun fun = (__trace_setMainProcessCallback_fun)myLib.resolve("__trace_setMainProcessCallback");
    if(fun)
        fun(__trace_add, __trace_del, __trace_get);
}

void CTrace::printTrace()
{
    char data[10240];
    memset(data, 0, 10240);
    __trace_getCallback(data, 10240);
    qDebug()<<"====Trace debug:\n"<<data;
}

void initialization()
{
    __trace_isInitialization = true;
#ifdef Q_OS_WIN
    if(__trace_addCallback == __trace_add && __trace_delCallback == __trace_del)
    {
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        SetUnhandledExceptionFilter(ExitWithExceptionCode);
    }
#endif
}

void __trace_show()
{
    if(__trace_traceIsActive)
    {
        using namespace std;
        ofstream of;
        string fileName = __trace_name;
        fileName.append("_Trace.txt");
        of.open(fileName.c_str(), ios_base::app | ios_base::out);
        if(!of.is_open())
            of.open("Trace_Trace.txt", ios_base::app | ios_base::out);
        of<<"========================================================================================="<<endl;
#ifdef Q_OS_WIN
        SYSTEMTIME sys;
        GetLocalTime(&sys);
        of<<sys.wYear<<"-"<<sys.wMonth<<"-"<<sys.wDay<<" "<<sys.wHour<<":"<<sys.wMinute<<":"<<sys.wSecond<<endl;
#endif
        of<<"========================================================================================="<<endl;
        wstringstream wss;
        wss<<"Trace information:"<<endl;
        for(int i = 0; i < m_currentFunctionIndex; ++i)
        {
            wss<<__trace_functionNames[i]<<endl;
            of<<__trace_functionNames[i]<<endl;
        }
        of<<"========================================================================================="<<endl;
        of<<endl<<endl;
        of.close();
#ifdef Q_OS_WIN
        MessageBox(NULL, wss.str().c_str(), L"Program error!", MB_OK);
#endif
    }
}

void __trace_add(const char *name)
{
    __trace_mutex->lock();
    __trace_functionNames[m_currentFunctionIndex++] = name;
    __trace_mutex->unlock();
}

void __trace_del()
{
    __trace_mutex->lock();
    m_currentFunctionIndex--;
    __trace_mutex->unlock();
}

int __trace_get(char *data, int len)
{
    int n = 0;
    int strLen = 0;
    for(int i = 0; i < m_currentFunctionIndex; ++i)
    {
        strLen = strlen(__trace_functionNames[i]);
        if(n + strLen + 1 <= len)
        {
            memcpy(data + n, __trace_functionNames[i], strLen);
            n += strLen;
            data[n] = '\n';
            n += 1;
        }
    }
    return n;
}

}   // namespace trace

#ifdef CTRACE_ISDLL
void __trace_setMainProcessCallback(__trace_addCallback_fun addFun, __trace_delCallback_fun delFun, __trace_getCallback_fun getFun)
{
    trace::__trace_addCallback = addFun;
    trace::__trace_delCallback = delFun;
    trace::__trace_getCallback = getFun;
}
#endif
