#ifndef deviceControl_H
#define deviceControl_H
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
struct cROOT//根节点
{
    QString                 m_sVersion;
    QString                 m_sTime;
};
class deviceControl : public QObject
{
        Q_OBJECT
public:
    deviceControl();
    bool            restDeviceControl();
    bool            saveDeviceControlFile();
private:
    void            ParseROOT(QDomNode* node);
    void            ParseBRANCH_LEVEL1(QDomNode *node);
    void            SaveROOT(QDomNode* node);
    void            SaveBranchLevel1(QDomElement* parentNode, CDevice* device);

private:
    cROOT               m_tRoot;
    QDomElement         eldRoot;
    QDomDocument        m_domDocSaveConfig;
};
#endif // deviceControl_H
