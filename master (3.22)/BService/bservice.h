#ifndef BSERVICE_H
#define BSERVICE_H

#include <QtCore/qglobal.h>
#if defined(BSERVICE_LIBRARY)
#  define BSERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define BSERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*OnDealEvent)(int nType, char* params, int nLen);

BSERVICESHARED_EXPORT int  TXType(const char* strPath/*project path*/);
BSERVICESHARED_EXPORT int  TXDestroyed();
BSERVICESHARED_EXPORT int  TXInitial(OnDealEvent event);
BSERVICESHARED_EXPORT bool TXStart(int nPanelID);
BSERVICESHARED_EXPORT bool TXStop();
BSERVICESHARED_EXPORT void TXShowTest(bool bWrite);

BSERVICESHARED_EXPORT void TXGetPacket(int nMsgType, char *pByteBuf/*out*/, int &nOutLen/*out*/);
BSERVICESHARED_EXPORT void TXParsePacket(int nMsgType, const char* data, int nLen);


#ifdef __cplusplus
}
#endif

#endif // BSERVICE_H
