#include "packageanalyzer.h"
#include <QDebug>
/*
================
PackageAnalyzer::PackageAnalyzer
================
*/
PackageAnalyzer::PackageAnalyzer()
{
    m_startSymbol.append((char)0x55);
    m_startSymbol.append((char)0xaa);
    m_startSymbol.append((char)0xcc);
    m_startSymbol.append((char)0x33);
}

/*
================
PackageAnalyzer::doAnalysis
frame head
canid
recv_addr
tx_addr
cmd
frametype
varlen
var
================
0x55 AA CC 33 03 00
*/
PackageTemplate *PackageAnalyzer::doAnalysis(QByteArray &bytes)
{
    QByteArray tmpHead;
    tmpHead.append((char)0x55);
    tmpHead.append((char)0xaa);
    tmpHead.append((char)0xcc);
    tmpHead.append((char)0x33);
    int startIndex = bytes.indexOf(tmpHead);
    if(startIndex < 0 ) return NULL;

    if (bytes.size() < 14)
        return NULL;
    int result = m_package.fromByteArray(bytes.mid(startIndex));
    if(result==1)
    {
        bytes.remove(0,bytes.length());
        return &m_package;
    }

    bytes.remove(0,bytes.length());
    return NULL;
}

