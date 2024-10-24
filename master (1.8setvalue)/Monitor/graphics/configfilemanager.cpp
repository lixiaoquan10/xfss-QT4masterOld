#include "configfilemanager.h"
#include "dm/cproject.h"
#include "dm/ccontroller.h"
#include "dm/cdistribution.h"
#include "dm/cloop.h"
#include "dm/cdevice.h"
#include "cglobal.h"
#include "QDateTime"
ConfigFileManager::ConfigFileManager()
{
    m_tRoot.m_sVersion = "1.00";
    m_tRoot.m_sTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
}
bool ConfigFileManager::ParseConfigFile()//读取loginInfo.xml文件并获取根节点
{
    QString sConfigFilePath;

    sConfigFilePath = qApp->applicationDirPath() + "/../root/loginInfo.xml" ;//in linux

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
    QDomElement root = m_domDocSaveConfig.documentElement();
    if(root.isNull())
    {
        root = m_domDocSaveConfig.createElement("xml");
    }
    SaveROOT(&root);
    m_domDocSaveConfig.appendChild(root);

    QString sFilePath = qApp->applicationDirPath() + "/../root/loginInfo.xml";

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
    m_tRoot.m_sVersion = node->toElement().attribute("version");
    m_tRoot.m_sTime = node->toElement().attribute("time");
    ParseBRANCH_LEVEL1(node);
}

void ConfigFileManager::ParseBRANCH_LEVEL1(QDomNode *node)//解析一级分支
{
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "Controller")
        {
            CController* controller = new CController(63);
            if(!controller)
                return;
            controller->setValue(CONTROLLER_VALUE_NAME,childnode.toElement().attribute("name"));
            controller->setValue(CONTROLLER_VALUE_ADDRESS,childnode.toElement().attribute("address").toInt());
            controller = ParseBRANCH_LEVEL2(&childnode,controller);
            CGlobal::instance()->ClientBusiness()->setController(controller);
        }
        childnode = childnode.nextSibling();
    }
}

CController* ConfigFileManager::ParseBRANCH_LEVEL2(QDomNode *node, CController* controller)//解析二级分支
{
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "Distribution")
        {
            int address = childnode.toElement().attribute("address").toInt();
            CDistribution* distribution = new CDistribution(address);
            if(distribution)
            {
                distribution->setDistributionValue(DISTRIBUTION_VALUE_ADDR,address);
                distribution->setDistributionValue(DISTRIBUTION_VALUE_NAME,childnode.toElement().attribute("name"));
                distribution->setDistributionValue(DISTRIBUTION_VALUE_TYPEID,childnode.toElement().attribute("TypeId"));
                distribution->setDistributionValue(DISTRIBUTION_VALUE_DESCRIPTION,childnode.toElement().attribute("productID"));
                distribution = ParseBRANCH_LEVEL3(&childnode,distribution);
                controller->addDistribution(distribution);
            }
        }
        childnode = childnode.nextSibling();
    }
    return controller;
}
CDistribution* ConfigFileManager::ParseBRANCH_LEVEL3(QDomNode *node, CDistribution* distribution)//解析三级分支
{
    QDomNode childnode = node->toElement().firstChild();
    while(!childnode.isNull())
    {
        if(childnode.toElement().tagName() == "Loop")
        {
            int address = childnode.toElement().attribute("address").toInt();
            CLoop* loop = new CLoop(address);
            loop->setloopValue(LOOP_VALUE_ADDR,address);
            loop->setloopValue(LOOP_VALUE_ID,address);
            loop->setloopValue(LOOP_VALUE_NAME,childnode.toElement().attribute("name"));
            loop = ParseBRANCH_LEVEL4(&childnode,loop);
            distribution->addLoop(loop);
        }
        childnode = childnode.nextSibling();
    }
    return distribution;
}

CLoop* ConfigFileManager::ParseBRANCH_LEVEL4(QDomNode *node, CLoop* loop)//解析四级分支
{
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
            device->setDeviceValue(DEVICE_VALUE_DESCRIPTION,childnode.toElement().attribute("productID"));
            device->setDeviceValue(DEVICE_VALUE_ADDR,childnode.toElement().attribute("address"));
            device->setDeviceValue(DEVICE_VALUE_TYPE_ID,childnode.toElement().attribute("typeID"));
            loop->addDevice(device);
        }
        childnode = childnode.nextSibling();
    }
    return loop;
}

void ConfigFileManager::SaveROOT(QDomNode *node)  //保存根节点
{
    QDomElement eldRoot = m_domDocSaveConfig.createElement("ROOT");
    eldRoot.setAttribute("version",m_tRoot.m_sVersion);
    eldRoot.setAttribute("time",m_tRoot.m_sTime);
    CController* controller = CGlobal::instance()->project()->controllerById(CGlobal::instance()->panelAddress());
    SaveBranchLevel1(&eldRoot,controller);
    node->appendChild(eldRoot);
}


void ConfigFileManager::SaveBranchLevel1(QDomElement *parentNode, CController* controller)   //保存一级分支
{
    QDomElement eleBranchLV1 = m_domDocSaveConfig.createElement("Controller");
    eleBranchLV1.setAttribute("name",controller->codeStr());
    eleBranchLV1.setAttribute("address",controller->controllerId());
    if(controller){
        for(int i=0; i<62; i++){
            CDistribution* distribution = controller->distributionByAddress(i+1);
//            if(distribution && distribution->distributionValue(DISTRIBUTION_VALUE_DATABASE) == 0)
            if(distribution)
                SaveBranchLevel2(&eleBranchLV1,distribution);
        }
    }
    parentNode->appendChild(eleBranchLV1);
}

void ConfigFileManager::SaveBranchLevel2(QDomElement *parentNode, CDistribution* distribution)   //保存二级分支
{

    QDomElement eleBranchLV2 = m_domDocSaveConfig.createElement("Distribution");
    eleBranchLV2.setAttribute("name",distribution->typeStr());
    eleBranchLV2.setAttribute("address",distribution->distributionAddress());
    eleBranchLV2.setAttribute("TypeId",distribution->distributionTypeId());
    eleBranchLV2.setAttribute("productID",distribution->descriptionStr());
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
    QDomElement eleBranchLV3 = m_domDocSaveConfig.createElement("Loop");
    eleBranchLV3.setAttribute("name",loop->typeStr());
    eleBranchLV3.setAttribute("address",loop->loopAdd());
    if(loop){
        for(int i=0; i<255; i++){
            CDevice* device = loop->deviceByAdd(i+1);
//            if(device && device->deviceValue(DEVICE_VALUE_DATABASE) == 0)
            //仅注册或者配置且注册的灯具  才保存到注册表
            if(device && (device->deviceValue(DEVICE_VALUE_ISUSED) == 1 || device->deviceValue(DEVICE_VALUE_ISUSED) == 2))
                SaveBranchLevel4(&eleBranchLV3,device);
        }
    }
    parentNode->appendChild(eleBranchLV3);
}

void ConfigFileManager::SaveBranchLevel4(QDomElement *parentNode, CDevice* device)  //保存四级分支
{
    QDomElement BranchLevel4 = m_domDocSaveConfig.createElement("Device");
    BranchLevel4.setAttribute("address",device->deviceAdd());
    BranchLevel4.setAttribute("productID",device->descriptionStr());
    BranchLevel4.setAttribute("typeID",device->deviceTypeId());
    parentNode->appendChild(BranchLevel4);
}
QVector<int> ConfigFileManager::SplitXmlStringInt(QString xmlText)//将QString按逗号分割，并转为int类型，存放到QVector<int>
{
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
