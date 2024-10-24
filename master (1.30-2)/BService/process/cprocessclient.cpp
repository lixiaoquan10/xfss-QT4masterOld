#include "cprocessclient.h"
#include "cprocesscontroller.h"
#include "cprocessdistribution.h"
#include "cprocesssocket.h"
#include "cprocessserver.h"
#include "log/clog.h"
#include "print/cprint.h"
#include "struct/cmsgnull.h"
#include "struct/cmsglog.h"
#include "struct/cmsgcheckset.h"
#include "struct/cmsgloginfo.h"
#include "struct/cmsglogin.h"
#include "struct/cmsgprintset.h"
#include "struct/cmsgfirepoint.h"
#include "struct/cmsgdeviceinfo.h"
#include "struct/cmsgdirectiontest.h"
#include "struct/cmsgevacuationgroup.h"
#include "struct/cmsgledinfo.h"
#include "struct/cmsgobjectstatus.h"
#include "cglobal.h"
#include "struct.h"
#include "ctrace.h"

#include <QTimer>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

CProcessClient::CProcessClient() :
    CProcessBase("Client")
{
}

void CProcessClient::procRecvEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray& byte)
{
    Q_UNUSED(infoType);
    Q_UNUSED(control);
    procRecvData(byte);
}

void CProcessClient::procSendData(const QByteArray &byte)
{
    QHash<QString, QVariant> control;
    procSendEvent(0, control, byte);
}

void CProcessClient::procSendEvent(const int &infoType, const QHash<QString, QVariant> &control, const QByteArray& byte)
{
    emit sendEvent(name(), infoType, control, byte);
}
/*CMD FROM GUI*/
void CProcessClient::procRecvData(const QByteArray &byte)
{
    QDataStream stream(const_cast<QByteArray*>(&byte), QIODevice::ReadOnly);
    CMsgStructBase* msgStruct = NULL;
    int type = NCT_None;
    stream>>type>>&msgStruct;
    qDebug()<< type << msgStruct << "===CProcessClient::procRecvData===" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz");
    if(!msgStruct) return;
    switch (type) {
        case NCT_AddLog:
        {
            CMsgLog* msgLog = static_cast<CMsgLog*>(msgStruct);
            CGlobal::instance()->processController()->addLog(msgLog);
            break;
        }
        case NCT_GetLog:
        {
            CMsgLogInfo* msgLogInfo = static_cast<CMsgLogInfo*>(msgStruct);
            QList<CMsgLog*> logs = CGlobal::instance()->log()->getLog(msgLogInfo->beginTime, msgLogInfo->endTime, msgLogInfo->type,
                                                                      CGlobal::instance()->m_recordPage,CGlobal::instance()->m_recordNumber);
            qDebug() << msgLogInfo->beginTime << msgLogInfo->endTime;
            if(logs.count() == 0)
            {
                CGlobal::instance()->setRecordPage(1);//已最后一页，再下一页回到第一页
                logs = CGlobal::instance()->log()->getLog(msgLogInfo->beginTime, msgLogInfo->endTime, msgLogInfo->type,
                                                                          CGlobal::instance()->m_recordPage,CGlobal::instance()->m_recordNumber);
                QByteArray data = m_msgNull->data(NCT_RecordPage);
                CGlobal::instance()->DealEvent(NCT_RecordPage, data);
            }
            for(int i = logs.size()-1; i >= 0; --i)
            {
                CMsgLog* log = logs.at(i);
                log->EventTypeID = msgLogInfo->requestId;
                QByteArray data = log->data(NCT_Log);
                CGlobal::instance()->DealEvent(NCT_Log, data);
                delete log;
            }
            logs.clear();
            break;
        }
        case NCT_ProjectInitComplete:
        {
            CProcessController* processController = CGlobal::instance()->processController();
            if(processController->isYearCheck())
                CGlobal::instance()->DealEvent(NCT_YearCheckBegin, m_msgNull->data(NCT_YearCheckBegin));
            else if(processController->isMonthCheck())
                CGlobal::instance()->DealEvent(NCT_MonthCheckBegin, m_msgNull->data(NCT_MonthCheckBegin));
            break;
        }
        case NCT_LedStatus:
        {
            CMsgLedInfo* msgLedInfo = static_cast<CMsgLedInfo*>(msgStruct);
            CGlobal::instance()->processController()->setLedStatus(msgLedInfo->nLedID, msgLedInfo->nLedStatus);
            break;
        }    
        case NCT_PrintSet:
        {
            CMsgPrintSet* msgPrintSet = static_cast<CMsgPrintSet*>(msgStruct);
            CPrint* print = CGlobal::instance()->print();
            if(msgPrintSet->printStart)
                print->addPrintType(CPrint::Start);
            else
                print->delPrintType(CPrint::Start);
            if(msgPrintSet->printFault)
                print->addPrintType(CPrint::Fault);
            else
                print->delPrintType(CPrint::Fault);
            if(msgPrintSet->printOther)
                print->addPrintType(CPrint::Other);
            else
                print->delPrintType(CPrint::Other);
            CGlobal::instance()->DealEvent(type, byte);
            CGlobal::instance()->processController()->addLog(CPrint::Other, tr("Print Setting"));
            break;
        }
        case NCT_CheckSet:
        {
            CMsgCheckSet* msgCheckSet = static_cast<CMsgCheckSet*>(msgStruct);
            CGlobal::instance()->processController()->setCheck(msgCheckSet->yearCycle, msgCheckSet->yearDuration, msgCheckSet->monthCycle, msgCheckSet->monthDuration);
            CGlobal::instance()->DealEvent(type, byte);
            CGlobal::instance()->processController()->addLog(CPrint::Start, tr("Check Setting"));
            break;
        }
        case NCT_DirectionTest:
        {
            CMsgDirectionTest* msgDirectionTest = static_cast<CMsgDirectionTest*>(msgStruct);
            CProcessDistribution::Directions direction = msgDirectionTest->left ? CProcessDistribution::LeftDirection : CProcessDistribution::NoneDirection;
            if(msgDirectionTest->right)
                direction |= CProcessDistribution::RightDirection;
            CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();
            if(processDistribution)
                processDistribution->directionTest(direction);
            CGlobal::instance()->processController()->addLog(CPrint::Other, tr("Device Direction Test"));
            break;
        }
        case NCT_ManualFire:
        {
            CMsgFirePoint* msgFirePoint = static_cast<CMsgFirePoint*>(msgStruct);
            CGlobal::instance()->processController()->manualFire(msgFirePoint->content);
            break;
        }
        case NCT_Relogin:
        {
            qDebug() << "login";
            CGlobal::instance()->processController()->reLogin();
            break;
        }
        case NCT_DuplicateCheck:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            int distributionAddress = msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0);
            int loopAddress = msgDeviceInfo->deviceInfo.value(DI_LoopId, 0);
            if(distributionAddress != 0 && loopAddress != 0)
            {
                CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();
                if(processDistribution)
                    processDistribution->readDuplicateDevice(loopAddress, distributionAddress);
            }
            CGlobal::instance()->processController()->addLog(CPrint::Other, tr("Duplicate Check"));
            break;
        }
        case NCT_ChangeDeviceAddress:
        {
            int ID[5];
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            int distributionAddress = msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0);
            int loopAddress = msgDeviceInfo->deviceInfo.value(DI_LoopId, 0);
            int oldaddress = msgDeviceInfo->deviceInfo.value(DI_oldDeviceID, 0);
            int newAddress = msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0);
            ID[0] = msgDeviceInfo->deviceInfo.value(DI_ID1, 0);
            ID[1] = msgDeviceInfo->deviceInfo.value(DI_ID2, 0);
            ID[2] = msgDeviceInfo->deviceInfo.value(DI_ID3, 0);
            ID[3] = msgDeviceInfo->deviceInfo.value(DI_ID4, 0);
            ID[4] = msgDeviceInfo->deviceInfo.value(DI_ID5, 0);
            if(distributionAddress != 0 && loopAddress != 0 && newAddress != 0)
            {
                CProcessDistribution* processDistribution = CGlobal::instance()->processDistribution();
                if(processDistribution)
                    processDistribution->writeDeviceAddress(loopAddress, oldaddress, newAddress, ID, distributionAddress);
            }
            CGlobal::instance()->processController()->addLog(CPrint::Other, tr("Change Duplicate Device Address"));
            break;
        }
        case NCT_StartEvacuationGroup:
        {
            break;
        }
        case NCT_StartFlowEvacuationGroup:
        {
            break;
        }
        case NCT_StopEvacuationGroup:
        {
            break;
        } 
        case NCT_ClearEvacuationFlow:
        {
            break;
        }
        case NCT_StartSpeedUp:
        {
            CGlobal::instance()->processController()->setSpeedUp(true);
            break;
        }
        case NCT_StopSpeedUp:
        {
            CGlobal::instance()->processController()->setSpeedUp(false);
            break;
        }
        case NCT_StartCharge:
        {
            CGlobal::instance()->processController()->setJZPowerStatus(true, true, true);
            break;
        }
        case NCT_StopCharge:
        {
            CGlobal::instance()->processController()->setJZPowerStatus(false, true, true);
            break;
        }
        case NCT_ObjectStatus:
        {
            break;
        }
        case NCT_ExitProgram:
        {
            break;
        }
        //复位
        case NCT_Reset:
        {
            CGlobal::instance()->processController()->reset();
            break;
        }
        //消音
        case NCT_Mute:
        {
            CGlobal::instance()->processController()->mute(true);
            break;
        }
        //取消消音
        case NCT_CancelMute:
        {
            CGlobal::instance()->processController()->mute(false);
            break;
        }
        //自动模式
        case NCT_AutomaticEvacuation:
        {
            CGlobal::instance()->processController()->setAutoEvacuation(true);
            break;
        }
        //手动模式
        case NCT_ManualEvacuation:
        {
            CGlobal::instance()->processController()->setAutoEvacuation(false);
            break;
        }
        //系统自检
        case NCT_SystemSelfCheck:
        {
            CGlobal::instance()->processController()->selfCheck();
            break;
        }
        //系统手动月检
        case NCT_SystemMonthCheck:
        {
            CGlobal::instance()->processController()->manualMonthCheck();
            break;
        }
        //系统手动年检
        case NCT_SystemYearCheck:
        {
            CGlobal::instance()->processController()->manualYearCheck();
            break;
        }
        //系统月检
        case NCT_MonthCheckEnd:
        {
            CGlobal::instance()->processController()->stopMonthCheck();
            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_MonthEmergency, 0, QDateTime::currentDateTime().toTime_t());
            break;
        }
        //系统年检
        case NCT_YearCheckEnd:
        {
            CGlobal::instance()->processController()->stopYearCheck();
            uploadStatus(1, CGlobal::instance()->panelAddress(), 0, 0, 0, OBJS_YearEmergency, 0, QDateTime::currentDateTime().toTime_t());
            break;
        }
        //开关量联动应急（输入）
        case NCT_InputEmergency:
        {
            CGlobal::instance()->processController()->emergency(OBJS_InputEmergency, true);
            CGlobal::instance()->processController()->addLog(CPrint::Start, tr("Input Emergency Start"));
            break;
        }
        //强制启动（按键）
        case NCT_ForceEmergency:
        {
            CGlobal::instance()->processController()->emergency(OBJS_ForceEmergency, true);
            CGlobal::instance()->processController()->addLog(CPrint::Start, tr("Force Emergency Start"));
            break;
        }
        //应急启动
        case NCT_EmergencyStart:
        {
            CGlobal::instance()->processController()->emergency(OBJS_Emergency, true);
            CGlobal::instance()->processController()->addLog(CPrint::Start, tr("Emergency Start"));
            break;
        }
        //应急停止
        case NCT_EmergencyStop:
        {
            CGlobal::instance()->processController()->emergency(OBJS_Emergency, false);
            CGlobal::instance()->processController()->addLog(CPrint::Start, tr("Emergency Stop"));
            break;
        }
        //发送应急指令
        case NCT_Emergency:
        {
            CMsgObjectStatus* msg = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->emergency(msg->nValue, msg->nDisID, msg->nLoopID,msg->nDeviceID);
            break;
        }
        //全部注册
        case NCT_LoginInfo:
        {
            qDebug() << "loginInfo";
            //led灯键板改为2s下发一次，等待注册结束
//            CGlobal::instance()->processController()->setLedupdateTime2s();
            //先读取集中电源信息。收到后再发注册，见0x77 == cmdType
            CGlobal::instance()->processDistribution()->readDeviceType();
            break;
        }
        //全部清除注册
        case NCT_Unlogin:
        {
            qDebug() << "unlogin";
            CGlobal::instance()->processDistribution()->unLogin();
            break;
        }
        //单个注册
        case NCT_LoginInfoLoop:
        {
            qDebug() << "loginInfoLoop";
            CMsgLoginData* msglogin = static_cast<CMsgLoginData*>(msgStruct);
            CGlobal::instance()->processDistribution()->login(msglogin->nDistributeID,msglogin->nLoopID);
            break;
        }
        //单个清除注册
        case NCT_UnLoginLoop:
        {
            qDebug() << "unloginLoop";
            CMsgLoginData* msglogin = static_cast<CMsgLoginData*>(msgStruct);
            CGlobal::instance()->processDistribution()->unLogin(msglogin->nDistributeID,msglogin->nLoopID);
            break;
        }
        //查询运行参数
        case NCT_PowerInfo:
        {
            qDebug() << "powerLnfo";
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->powerInfo(msgObjectStatus->nDisID,msgObjectStatus->nLoopID,msgObjectStatus->nDeviceID);
            break;
        }
        //读取保护参数
        case NCT_ReadProtectInfor:
        {
            qDebug() << "readProtectInfor";
            CMsgLoginData* msglogin = static_cast<CMsgLoginData*>(msgStruct);
            CGlobal::instance()->processDistribution()->readProtectInfor(msglogin->nDistributeID,0,0);
            break;
        }
        //写保护参数
        case NCT_WriteProtectInfor:
        {
            qDebug() << "writeProtectInfor";
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->writeProtectInfor(msgObjectStatus->nDisID,0,0,msgObjectStatus->ndata);
            break;
        }
        //设备应急开始
        case NCT_DeviceEmergencyStart:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            CGlobal::instance()->processDistribution()->lightControl(true, msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0),
                                                                     msgDeviceInfo->deviceInfo.value(DI_LoopId, 0), msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0));
            break;
        }
        //设备应急停止
        case NCT_DeviceEmergencyStop:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            CGlobal::instance()->processDistribution()->lightControl(false, msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0),
                                                                     msgDeviceInfo->deviceInfo.value(DI_LoopId, 0), msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0));
            break;
        }
        //修改灯具设备地址
        case NCT_DeviceAdd:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            int deviceAddress = msgObjectStatus->nDeviceID;
            int value = msgObjectStatus->nValue;
            if(value != deviceAddress)
            {
                CGlobal::instance()->processDistribution()->DeviceAdd(msgObjectStatus->nDisID, msgObjectStatus->nLoopID, deviceAddress,
                                                                      value, msgObjectStatus->strText);
            }

            break;
        }
        //修改灯具设备类型
        case NCT_DeviceType:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            int value = msgObjectStatus->nValue;
            int typeId = msgObjectStatus->nType;
            if(value != typeId)
            {
                CGlobal::instance()->processDistribution()->DeviceType(msgObjectStatus->nDisID, msgObjectStatus->nLoopID,
                                                                       msgObjectStatus->nDeviceID, value);
            }
            break;
        }
        //设备测试开始
        case NCT_DeviceTestStart:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            CGlobal::instance()->processDistribution()->lightTest(msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0),
                                                                  msgDeviceInfo->deviceInfo.value(DI_LoopId, 0), msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0));
            break;
        }
        //灯具默认点亮
        case NCT_DeviceDefaultOn:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            CGlobal::instance()->processDistribution()->lampdefaultlighten(true, msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0),
                                                                           msgDeviceInfo->deviceInfo.value(DI_LoopId, 0), msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0));
            break;
        }
        //灯具默认熄灭
        case NCT_DeviceDefaultOff:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            CGlobal::instance()->processDistribution()->lampdefaultlighten(false, msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0),
                                                                               msgDeviceInfo->deviceInfo.value(DI_LoopId, 0), msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0));
            break;
        }
        //标志灯默认方向
        case NCT_DeviceDefaultDrection:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->lampdefaultdirection(msgObjectStatus->nValue, msgObjectStatus->nDisID,
                                                                             msgObjectStatus->nLoopID, msgObjectStatus->nDeviceID);
            break;
        }
        //标志灯方向控制
        case NCT_DeviceDirection:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->directionControl(msgObjectStatus->nValue, msgObjectStatus->nDisID,
                                                                             msgObjectStatus->nLoopID, msgObjectStatus->nDeviceID);
            break;
        }
        //标志灯闪灯配置
        case NCT_FlashConfig:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->flashConfig(msgObjectStatus->nValue, msgObjectStatus->nDisID,
                                                                    msgObjectStatus->nLoopID, msgObjectStatus->nDeviceID);
            break;
        }
        //标志灯闪灯控制
        case NCT_FlashControl:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->flashControl(msgObjectStatus->nValue, msgObjectStatus->nDisID,
                                                                     msgObjectStatus->nLoopID, msgObjectStatus->nDeviceID);
            break;
        }
        //可变状态控制
        case NCT_ChangeControl:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->ChangeControl(msgObjectStatus->nValue, msgObjectStatus->nDisID,
                                                                      msgObjectStatus->nLoopID, msgObjectStatus->nDeviceID);
            break;
        }
        //设备复位
        case NCT_DeviceReset:
        {
            CMsgDeviceInfo* msgDeviceInfo = static_cast<CMsgDeviceInfo*>(msgStruct);
            CGlobal::instance()->processDistribution()->lightReset(msgDeviceInfo->deviceInfo.value(DI_DistributionId, 0), msgDeviceInfo->deviceInfo.value(DI_LoopId, 0),
                                                    msgDeviceInfo->deviceInfo.value(DI_DeviceId, 0));
            break;
        }
        //节能模式
        case NCT_EnergyMode:
        {
            CMsgObjectStatus* msg = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->energysave(msg->nStatusID,msg->nDisID,msg->nLoopID,msg->nDeviceID);
            break;
        }
        //系统设置
        case NCT_SystemSet:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            qDebug() << "===CProcessClient::procRecvData===NCT_SystemSet" << type << msgObjectStatus->ndata.toHex();
            CGlobal::instance()->processController()->SystemSet(msgObjectStatus);
            break;
        }
        //读设备信息
        case NCT_ReadDeviceInfo:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->readDeviceInfor(msgObjectStatus->nDisID,msgObjectStatus->nLoopID,msgObjectStatus->nDeviceID);
            break;
        }     
        //自带电池灯具关闭备电输出
        case NCT_CloseBatteryOut:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->CloseBatteryOut(msgObjectStatus->nValue, msgObjectStatus->nDisID,
                                                                        msgObjectStatus->nLoopID, msgObjectStatus->nDeviceID);
            break;
        }
        //配电箱应急回路输出控制
        case NCT_SetLoopOutStatus:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->SetLoopOutStatus(msgObjectStatus->nValue, msgObjectStatus->nDisID, msgObjectStatus->nLoopID);
            break;
        }
        //读取配电箱应急回路输出状态
        case NCT_ReadLoopOutStatus:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processDistribution()->ReadLoopOutStatus(msgObjectStatus->nDisID, msgObjectStatus->nLoopID);
            break;
        }
        //信息记录页码
        case NCT_RecordPage:
        {
            CMsgObjectStatus* msgObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            CGlobal::instance()->processController()->RecordPage(msgObjectStatus);
            break;
        }
        case NCT_XmlNone:
        {
            CMsgObjectStatus* xmlObjectStatus = static_cast<CMsgObjectStatus*>(msgStruct);
            if(xmlObjectStatus->nType == NCT_XmlNone)//服务端回复数据
                CGlobal::instance()->processServer()->replyXmlMsg(xmlObjectStatus->ndata);
            else
                CGlobal::instance()->processSocket()->sendXmlMsg(xmlObjectStatus->nType,xmlObjectStatus->ndata);
            break;
        }
    }
    delete msgStruct;
}

void CProcessClient::clientReset()
{
    QByteArray data = m_msgNull->data(NCT_ClientReset);
    CGlobal::instance()->DealEvent(NCT_ClientReset, data);
}

void CProcessClient::mute(bool enable)
{
    Q_UNUSED(enable);
    QByteArray byte;
    if(enable)
        byte = m_msgNull->data(NCT_Mute);
    else
        byte = m_msgNull->data(NCT_CancelMute);
    CGlobal::instance()->DealEvent(enable?NCT_Mute:NCT_CancelMute, byte);
}

void CProcessClient::factoryReset()
{
    QByteArray byte = m_msgNull->data(NCT_FactoryReset);
    CGlobal::instance()->DealEvent(NCT_FactoryReset, byte);
}

void CProcessClient::StartCommunication()
{
    QByteArray byte = m_msgNull->data(NCT_StartCommunication);
    CGlobal::instance()->DealEvent(NCT_StartCommunication, byte);
}

void CProcessClient::StopCommunication()
{
    QByteArray byte = m_msgNull->data(NCT_StopCommunication);
    CGlobal::instance()->DealEvent(NCT_StopCommunication, byte);
}

void CProcessClient::sendPrintSet()
{
    QByteArray data;
    CMsgPrintSet msgPrintSet;
    msgPrintSet.printStart = CGlobal::instance()->print()->isPrintType(CPrint::Start);
    msgPrintSet.printFault = CGlobal::instance()->print()->isPrintType(CPrint::Fault);
    msgPrintSet.printOther = CGlobal::instance()->print()->isPrintType(CPrint::Other);
    data = msgPrintSet.data(NCT_PrintSet);
    CGlobal::instance()->DealEvent(NCT_PrintSet, data);
}

void CProcessClient::sendCheckSet()
{
    QByteArray data;
    CMsgCheckSet msgCheckSet;
    msgCheckSet.yearCycle = CGlobal::instance()->processController()->yearCycle();
    msgCheckSet.yearDuration = CGlobal::instance()->processController()->yearDuration();
    msgCheckSet.monthCycle = CGlobal::instance()->processController()->monthCycle();
    msgCheckSet.monthDuration = CGlobal::instance()->processController()->monthDuration();
    data = msgCheckSet.data(NCT_CheckSet);
    CGlobal::instance()->DealEvent(NCT_CheckSet, data);
}


