#ifndef deviceForbidFault_H
#define deviceForbidFault_H
#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QVector>
class CCanport;
class CDistribution;
class CLoop;
class CDevice;
struct fROOT//根节点
{
    QString                 m_sVersion;
    QString                 m_sTime;
};
class deviceForbidFault : public QObject
{
        Q_OBJECT
public:
    deviceForbidFault();
    bool            deviceForbidFile();
    bool            saveDeviceForbidFile();
private:
    void            ParseROOT(QDomNode* node);
    void            ParseBRANCH_LEVEL1(QDomNode *node);
    void            SaveROOT(QDomNode* node);
    void            SaveBranchLevel1(QDomElement* parentNode, CDevice* device);

private:
    fROOT                m_tRoot;
    QDomElement         eldRoot;
    QDomDocument     m_domDocSaveConfig;
};
#endif // deviceForbidFault_H
