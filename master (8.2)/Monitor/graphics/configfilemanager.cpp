#include "cglobal.h"

ConfigFileManager::ConfigFileManager()
{
    m_tRoot.m_sVersion = "1.00";
    m_tRoot.m_sTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
}
bool ConfigFileManager::ParseConfigFile()//读取loginInfo.xml文件并获取根节点
{
    qDebug() << "ConfigFileManager::ParseConfigFile" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QString sConfigFilePath;

    sConfigFilePath = qApp->applicationDirPath() + "/logfile/loginInfo.xml" ;//in linux

    QFile file(sConfigFilePath);
    if (!file.open(QFile::ReadOnly))
    {
        CGlobal::instance()->m_isxmlNull = true;
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument qDomConfigFile;
    if (!qDomConfigFile.setContent(&file, false, &errorStr, &errorLine,&errorColumn))
    {
        CGlobal::instance()->m_isxmlNull = true;
        return false;
    }
    QDomElement root = qDomConfigFile.documentElement();
    QDomNode child = root.firstChild();
    if (!child.isNull())
    {
        if(child.toElement().tagName() == "ROOT")
        {
            CGlobal::instance()->m_isxmlNull = false;
            ParseROOT(&child);
        }
    }
    file.close();
    return true;
}

bool ConfigFileManager::SaveConfigFile()
{
    qDebug() << "ConfigFileManager::SaveConfigFile" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement root = m_domDocSaveConfig.documentElement();
    if(root.isNull())
    {
        root = m_domDocSaveConfig.createElement("xml");
    }
    SaveROOT(&root);
    m_domDocSaveConfig.appendChild(root);

    QString sFilePath = qApp->applicationDirPath() + "/logfile/loginInfo.xml";

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

void ConfigFileManager::ParseROOT(QDomNode *node)//解析根节点
{
    qDebug() << "ConfigFileManager::ParseROOT" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    m_tRoot.m_sVersion = node->toElement().attribute("version");
    m_tRoot.m_sTime = node->toElement().attribute("time");
    ParseBRANCH_LEVEL1(node);
}

void ConfigFileManager::ParseBRANCH_LEVEL1(QDomNode *node)//解析一级分支
{
    qDebug() << "ConfigFileManager::ParseBRANCH_LEVEL1" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomNode childnode = node->toElement().firstChild();
    CController* controller = new CController(1);
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "Canport")
        {
            CCanport* canport = new CCanport(63);
            if(!canport)
                return;
            canport->setCanportValue(CANPORT_VALUE_NAME,childnode.toElement().attribute("Name"));
            canport->setCanportValue(CANPORT_VALUE_PORT,childnode.toElement().attribute("CanID").toInt());
            canport->setCanportValue(CANPORT_VALUE_ID,childnode.toElement().attribute("CanID").toInt());
            canport->setCanportValue(CANPORT_VALUE_ADDRESS,childnode.toElement().attribute("CanID").toInt()+2);
            canport = ParseBRANCH_LEVEL2(&childnode,canport);
            controller->addCanport(canport);
        }
        childnode = childnode.nextSibling();
    }
    CGlobal::instance()->ClientBusiness()->setLoginController(controller);
}

CCanport* ConfigFileManager::ParseBRANCH_LEVEL2(QDomNode *node, CCanport* canport)//解析二级分支
{
    qDebug() << "ConfigFileManager::ParseBRANCH_LEVEL2" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "Distribution")
        {
            int address = childnode.toElement().attribute("Address").toInt();
            CDistribution* distribution = new CDistribution(address);
            if(distribution)
            {
                distribution->setDistributionValue(DISTRIBUTION_VALUE_ADDR,address);
                distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,childnode.toElement().attribute("Name"));
                distribution->setDistributionValue(DISTRIBUTION_VALUE_TYPEID,childnode.toElement().attribute("TypeId"));
                distribution->setDistributionValue(DISTRIBUTION_VALUE_DESCRIPTION,childnode.toElement().attribute("ProductID"));
                distribution = ParseBRANCH_LEVEL3(&childnode,distribution);
                canport->addDistribution(distribution);
            }
        }
        childnode = childnode.nextSibling();
    }
    return canport;
}
CDistribution* ConfigFileManager::ParseBRANCH_LEVEL3(QDomNode *node, CDistribution* distribution)//解析三级分支
{
    qDebug() << "ConfigFileManager::ParseBRANCH_LEVEL3" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "Loop")
        {
            int address = childnode.toElement().attribute("Address").toInt();
            CLoop* loop = new CLoop(address);
            if(!loop)
                continue;
            loop->setloopValue(LOOP_VALUE_ADDR,address);
            loop->setloopValue(LOOP_VALUE_ID,address);
            loop->setloopValue(LOOP_VALUE_NAME,childnode.toElement().attribute("Name"));
            loop = ParseBRANCH_LEVEL4(&childnode,loop);
            distribution->addLoop(loop);
        }
        childnode = childnode.nextSibling();
    }
    return distribution;
}

CLoop* ConfigFileManager::ParseBRANCH_LEVEL4(QDomNode *node, CLoop* loop)//解析四级分支
{
    qDebug() << "ConfigFileManager::ParseBRANCH_LEVEL4" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    CDevice* device = new CDevice;
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        static int DeviceCount;
        if(childnode.toElement().tagName() == "Device")
        {
            DeviceCount = loop->deviceCount();
            loop->initDevices();
            device = loop->device(DeviceCount);
            if(!device)
                continue;
            device->setDeviceValue(DEVICE_VALUE_DESCRIPTION,childnode.toElement().attribute("ProductID"));
            device->setDeviceValue(DEVICE_VALUE_ADDR,childnode.toElement().attribute("Address"));
            device->setDeviceValue(DEVICE_VALUE_LOGINTYPE_ID,childnode.toElement().attribute("TypeID"));
            loop->addDevice(device);
        }
        childnode = childnode.nextSibling();
    }
    return loop;
}

void ConfigFileManager::SaveROOT(QDomNode *node)  //保存根节点
{
    qDebug() << "ConfigFileManager::SaveROOT" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement eldRoot = m_domDocSaveConfig.createElement("ROOT");
    eldRoot.setAttribute("version",m_tRoot.m_sVersion);
    eldRoot.setAttribute("time",m_tRoot.m_sTime);
    for(int i=3; i<=CGlobal::instance()->m_nCanNumber+2; i++)
    {
        CCanport* canport = CGlobal::instance()->controller()->canportByAddress(i);
        if(!canport)
            continue;
        SaveBranchLevel1(&eldRoot,canport);
    }
    node->appendChild(eldRoot);
}


void ConfigFileManager::SaveBranchLevel1(QDomElement *parentNode, CCanport* canport)   //保存一级分支
{
    qDebug() << "ConfigFileManager::SaveBranchLevel1" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement eleBranchLV1 = m_domDocSaveConfig.createElement("Canport");
    eleBranchLV1.setAttribute("Name",canport->codeStr());
    eleBranchLV1.setAttribute("CanID",canport->canportValue(CANPORT_VALUE_PORT).toInt());
    if(canport){
        for(int i=0; i<62; i++){
            CDistribution* distribution = canport->distributionByAddress(i+1);
            if(distribution && distribution->isDistributionLogin())
                SaveBranchLevel2(&eleBranchLV1,distribution);
        }
    }
    parentNode->appendChild(eleBranchLV1);
}

void ConfigFileManager::SaveBranchLevel2(QDomElement *parentNode, CDistribution* distribution)   //保存二级分支
{
    qDebug() << "ConfigFileManager::SaveBranchLevel2" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");

    QDomElement eleBranchLV2 = m_domDocSaveConfig.createElement("Distribution");
    eleBranchLV2.setAttribute("Name",distribution->typeStr());
    eleBranchLV2.setAttribute("Address",distribution->distributionAddress());
    eleBranchLV2.setAttribute("TypeId",distribution->distributionTypeId());
    eleBranchLV2.setAttribute("ProductID",distribution->descriptionStr());
    if(distribution){
        if(distribution->distributionTypeId() == 0 || distribution->descriptionStr() == "")
            return;
        for(int i=0; i<8; i++){
            CLoop* loop = distribution->loopByAdd(i+1);
            if(loop)
                SaveBranchLevel3(&eleBranchLV2,loop);
        }
    }
  parentNode->appendChild(eleBranchLV2);
}

void ConfigFileManager::SaveBranchLevel3(QDomElement *parentNode, CLoop* loop)   //保存三级分支
{
    qDebug() << "ConfigFileManager::SaveBranchLevel3" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement eleBranchLV3 = m_domDocSaveConfig.createElement("Loop");
    eleBranchLV3.setAttribute("Name",loop->typeStr());
    eleBranchLV3.setAttribute("Address",loop->loopAdd());
    if(loop){
        for(int i=0; i<255; i++){
            CDevice* device = loop->deviceByAdd(i+1);
//            if(device && device->deviceValue(DEVICE_VALUE_DATABASE) == 0)
            //仅注册或者配置且注册的灯具  才保存到注册表
            if(device && device->isDeviceLogin())
                SaveBranchLevel4(&eleBranchLV3,device);
        }
    }
    parentNode->appendChild(eleBranchLV3);
}

void ConfigFileManager::SaveBranchLevel4(QDomElement *parentNode, CDevice* device)  //保存四级分支
{
    qDebug() << "ConfigFileManager::SaveBranchLevel4" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QDomElement BranchLevel4 = m_domDocSaveConfig.createElement("Device");
    BranchLevel4.setAttribute("Address",device->deviceAdd());
    BranchLevel4.setAttribute("ProductID",device->descriptionStr());
    BranchLevel4.setAttribute("TypeID",device->deviceTypeId());
    parentNode->appendChild(BranchLevel4);
}
QVector<int> ConfigFileManager::SplitXmlStringInt(QString xmlText)//将QString按逗号分割，并转为int类型，存放到QVector<int>
{
    qDebug() << "ConfigFileManager::SplitXmlStringInt" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QVector<int> vParam;
    QString value;
    QStringList list = xmlText.split(',');
    for (int i = 0; i < list.size(); ++i)
    {
        value = list.at(i);
        vParam.push_back(value.toInt());
    }
    return vParam;
}
QVector<float> ConfigFileManager::SplitXmlStringFloat(QString xmlText)//将QString按逗号分割，并转为float类型，存放到QVector<float>
{
    qDebug() << "ConfigFileManager::SplitXmlStringFloat" << QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
    QVector<float> vParam;
    QString value;
    QStringList list = xmlText.split(',');
    for (int i = 0; i < list.size(); ++i)
    {
        value = list.at(i);
        vParam.push_back(value.toFloat());
    }
    return vParam;
}
