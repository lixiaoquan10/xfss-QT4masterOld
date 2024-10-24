#ifndef PACKAGEANALYZER_H
#define PACKAGEANALYZER_H

#include "packagetemplate.h"
/*------------------------------------
  数据包分析器，实现从流二进制数据中检索出完
  整的数据包，如果二进制流出现问题，可以对出
  问题的部分就行剔除，保证后续的正常数据的分析
-------------------------------------*/

class PackageAnalyzer
{
public:
    PackageAnalyzer();
    ~PackageAnalyzer(){}
    PackageTemplate* doAnalysis(QByteArray &bytes);

private:
    PackageTemplate m_package;
    QByteArray m_startSymbol;

};

#endif // PACKAGEANALYZER_H
