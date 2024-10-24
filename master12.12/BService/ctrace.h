#ifndef CTRACE_H
#define CTRACE_H

#include <qglobal.h>

#ifdef __GNUC__
    #define CTRACE_FUNCTION __PRETTY_FUNCTION__
#else
    #define CTRACE_FUNCTION __FUNCSIG__
#endif

#ifdef CTRACE_CLOSE
    #define CTRACE_LOG
#else
    #define CTRACE_LOG trace::CTrace __trace_tmpvalue(CTRACE_FUNCTION); Q_UNUSED(__trace_tmpvalue)
#endif

#ifndef CTRACE_ISDLL
    #define CTRACE_ADDDLL(file) trace::CTrace::addDll((file))
#endif

#define CTRACE_LOGFILE(file) trace::CTrace::traceName((file))
#define CTRACE_PRINT() trace::CTrace::printTrace()

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*__trace_addCallback_fun)(const char *name);
typedef void (*__trace_delCallback_fun)();
typedef int (*__trace_getCallback_fun)(char* data, int len);

#ifdef CTRACE_ISDLL
Q_DECL_EXPORT void __trace_setMainProcessCallback(__trace_addCallback_fun addFun, __trace_delCallback_fun delFun, __trace_getCallback_fun getFun);
#endif

#ifdef __cplusplus
}
#endif

namespace trace {

class CTrace
{
public:
    CTrace(const char* name);
    ~CTrace();
    static void traceName(const char* name);
    static void addDll(const char* file);
    static void printTrace();
};

}
#endif // CTRACE_H
