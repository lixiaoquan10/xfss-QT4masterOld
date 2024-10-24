#include "cglobal.h"

deviceForbidFault::deviceForbidFault()
{
    m_tRoot.m_sVersion = "1.00";
    m_tRoot.m_sTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
}
bool deviceForbidFault::deviceForbidFile()//deviceForbidFault.xml文件并获取根节点
{
    qDebug() << "deviceForbidFault::deviceForbidFile" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString sConfigFilePath;

    sConfigFilePath = qApp->applicationDirPath() + "/logfile/deviceForbidFault.xml" ;//in linux

    QFile file(sConfigFilePath);
    if (!file.open(QFile::ReadOnly))
    {
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument qDomConfigFile;
    if (!qDomConfigFile.setContent(&file, false, &errorStr, &errorLine,&errorColumn))
    {
        return false;
    }
    QDomElement root = qDomConfigFile.documentElement();
    QDomNode child = root.firstChild();
    if (!child.isNull())
    {
        if(child.toElement().tagName() == "ROOT")
        {
            ParseROOT(&child);
        }
    }
    file.close();
    return true;
}

bool deviceForbidFault::saveDeviceForbidFile()
{
    qDebug() << "deviceForbidFault::saveDeviceForbidFile" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement root = m_domDocSaveConfig.documentElement();
    if(root.isNull())
    {
        root = m_domDocSaveConfig.createElement("xml");
    }
    SaveROOT(&root);
    m_domDocSaveConfig.appendChild(root);

    QString sFilePath = qApp->applicationDirPath() + "/logfile/deviceForbidFault.xml";

    QFile file(sFilePath);
    if (!file.open(QFile::WriteOnly|QFile::Truncate | QFile::Text))//1.QFile::WriteOnly如果文件不存在，则创建；2.QFile::Truncate打开的同时清空
    {
        return false;
    }
    QTextStream stream( &file );
    stream.setCodec("utf-8");
    m_domDocSaveConfig.save(stream,4,QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}

void deviceForbidFault::ParseROOT(QDomNode *node)//解析根节点
{
    qDebug() << "deviceForbidFault::ParseROOT" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_tRoot.m_sVersion = node->toElement().attribute("version");
    m_tRoot.m_sTime = node->toElement().attribute("time");
    ParseBRANCH_LEVEL1(node);
}

void deviceForbidFault::ParseBRANCH_LEVEL1(QDomNode *node)//解析一级分支
{
    qDebug() << "deviceForbidFault::ParseBRANCH_LEVEL1" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "ForbidDevice")
        {
            int canportAddress = childnode.toElement().attribute("CanID").toInt() + 2;
            int distributionAddress = childnode.toElement().attribute("DistributionAddress").toInt();
            int loopAddress = childnode.toElement().attribute("LoopAddress").toInt();
            int deviceAddress = childnode.toElement().attribute("DeviceAddress").toInt();
            CCanport* canport = CGlobal::instance()->controller()->canportByAddress(canportAddress);
            if(!canport)
                return;
            CDistribution* distribution = canport->distributionByAddress(distributionAddress);
            if(distribution)
            {
                CLoop* loop = distribution->loopByAdd(loopAddress);
                if(loop)
                {
                    CDevice* device = loop->deviceByAdd(deviceAddress);
                    if(device)
                    {
                        device->setDeviceForbid(true);
                        CGlobal::instance()->dlgDeviceForbidFault()->addDeviceForbidFaultInfo(device);
                    }
                }
            }
        }
        childnode = childnode.nextSibling();
    }
}


void deviceForbidFault::SaveROOT(QDomNode *node)  //保存根节点
{
    qDebug() << "deviceForbidFault::SaveROOT" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement eldRoot = m_domDocSaveConfig.createElement("ROOT");
    eldRoot.setAttribute("version",m_tRoot.m_sVersion);
    eldRoot.setAttribute("time",m_tRoot.m_sTime);
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        QList<CDistribution*> distributions = canport->distributions();
        for(int i=0; i<distributions.count(); i++){
            CDistribution* distribution = distributions.at(i);
            if(distribution){
                QList<CLoop*> loops = distribution->loops();
                for(int i=0; i<loops.count(); i++){
                    CLoop* loop = loops.at(i);
                    if(loop){
                        QList<CDevice*> devices = loop->devices();
                        for(int m=0; m<devices.count(); m++){
                            CDevice* device = devices.at(m);
                            if(device)
                            {
                                if(device->isDeviceForbid())
                                    SaveBranchLevel1(&eldRoot,device);
                            }
                        }
                        devices.clear();
                    }
                }
                loops.clear();
            }
        }
        distributions.clear();
    }
    node->appendChild(eldRoot);
}


void deviceForbidFault::SaveBranchLevel1(QDomElement *parentNode, CDevice* device)   //保存一级分支
{
    qDebug() << "deviceForbidFault::SaveBranchLevel1" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement eleBranchLV1 = m_domDocSaveConfig.createElement("ForbidDevice");
    eleBranchLV1.setAttribute("CanID",device->canportAdd()-2);
    eleBranchLV1.setAttribute("DistributionAddress",device->distributionAdd());
    eleBranchLV1.setAttribute("LoopAddress",device->loopAdd());
    eleBranchLV1.setAttribute("DeviceAddress",device->deviceAdd());
    parentNode->appendChild(eleBranchLV1);
}

