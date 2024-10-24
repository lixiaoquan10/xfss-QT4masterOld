#ifndef SHARDENUM_H
#define SHARDENUM_H

#include "cmsglog.h"
#include "cmsgloginfo.h"
#include "cmsgrouteconfig.h"
#include "cmsgprocesstypename.h"
#include "cmsgobjectstatus.h"
#include "cmsgnull.h"
#include "cmsglogin.h"
#include "cmsgfirepoint.h"
#include "cmsgprintset.h"
#include "cmsgcheckset.h"
#include "cmsgchecktime.h"
#include "cmsgdirectiontest.h"
#include "cmsgobjectstatusinfo.h"
#include "cmsgcontrollerinfo.h"
#include "cmsgdeviceinfo.h"
#include "cmsgpowerinfo.h"
#include "cmsgevacuationgroup.h"
#include "cmsgledinfo.h"

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define md_sleep(n) Sleep(n)
#else
#include <unistd.h>
#define md_sleep(n) usleep(n)
#endif


enum { PT_None = 0, PT_Distribution_CAN = 1001, PT_Power_CAN = 2001,
       PT_FAS_GST = 3001, PT_FAS_BIRD = 3002, PT_FAS_LEADER = 3003,
       PT_FAS_SONGJIANG = 3004, PT_FAS_YIAI = 3005, PT_FAS_TANDA = 3006,
       PT_FAS_SHANYING = 3007, PT_FAS_FUAN = 3008};

enum NCommandType{
    NCT_None = 0,                       // 空命令类型
    NCT_GetProcessTypeName = 1,         // 获取处理器类型名称对应表
    NCT_SetInterfaceBoardInfo = 2,      // 设置接口板信息,Dll调用Server
    NCT_SetRouteConfig = 3,             // 设置路由配置,Server调用Dll
    NCT_ObjectStatus = 4,               // 发送对象状态
    NCT_AddLog = 5,                     // 添加日志
    NCT_GetLog = 6,                     // 获取日志
    NCT_Reset = 7,                      // 复位
    NCT_Mute = 8,                       // 消音
    NCT_EmergencyStart = 9,             // 系统应急启动
    NCT_EmergencyStop = 10,              // 系统应急停止
    NCT_AutomaticEvacuation = 11,        // 自动疏散模式
    NCT_ManualEvacuation = 12,           // 手动疏散模式
    NCT_SystemSelfCheck = 13,            // 系统自检
    NCT_SystemMonthCheck = 14,           // 系统月检
    NCT_SystemYearCheck = 15,            // 系统年检
    NCT_Relogin = 16,                    // 重新登录

    NCT_ManualFire = 18,                 // 手动输入火警点疏散
    NCT_PrintSet = 19,                   // 打印设置
    NCT_CheckSet = 20,                   // 检测设置
    NCT_DirectionTest = 21,              // 方向测试
    NCT_FactoryReset = 22,               // 恢复出厂设置
    NCT_AutoFire = 23,                  // 管理员用户信息
    NCT_CheckTimeSync = 24,
    NCT_SelfCheckStart = 25,             // 系统自检开始
    NCT_SelfCheckStop = 26,              // 系统自检结束
    NCT_UndefinedObject = 27,            // 工程未定义Object
    NCT_ClientReset = 28,
    NCT_ForceEmergency = 29,             // 强制应急（按键）
    NCT_Log = 30,                        // 日志数据
    NCT_LedStatus = 31,           // 对象状态同步
    NCT_ControllerInfo = 32,             // 控制器信息
    NCT_GetControllerInfo = 33,          // 获取控制器信息
    NCT_StopCommunication = 34,                 // 服务器停止
    NCT_StartCommunication = 35,                // 服务器启动
    NCT_GetCurrentValue = 36,            // 获取回路电流信息
    NCT_GetVoltageValue = 37,            // 获取回路电压信息
    NCT_ClearUndefinedObject = 38,       // 清空未定义数据对象
    NCT_AddUndefinedObject = 39,         // 添加未定义数据对象
    NCT_DelUndefinedObject = 40,         // 删除未定义数据对象
    NCT_MonthCheckBegin = 41,            // 月检开始
    NCT_MonthCheckEnd = 42,              // 月检结束
    NCT_YearCheckBegin = 43,             // 年检开始
    NCT_YearCheckEnd = 44,               // 年检结束
    NCT_ProjectInitComplete = 45,  // 客户端工程初始化完成
    NCT_CancelMute = 46,                 // 取消消音
    NCT_PowerInfo = 47,                  // 集中电源信息
    NCT_StartEvacuationGroup = 48,       // 启动疏散预案
    NCT_StartFlowEvacuationGroup = 49,   // 启动疏散预案（导光流）
    NCT_StopEvacuationGroup = 50,        // 停止疏散预案
    NCT_DeviceEmergencyStart = 51,       // 设备应急启动
    NCT_DeviceEmergencyStop = 52,        // 设备应急停止
    NCT_GetLayerEvacuationFlow = 53,     // 获取图层光流
    NCT_LayerEvacuationFlow = 54,        // 图层光流
    NCT_ClearEvacuationFlow = 55,        // 清除导光流
    NCT_HasFASSystem = 56,               // 存在报警系统
    NCT_StartSpeedUp = 57,               // 开始模拟加速
    NCT_StopSpeedUp = 58,                // 结束模拟加速
    NCT_StartCharge = 59,               // 允许充电
    NCT_StopCharge = 60,                // 禁止充电
    NCT_ExitProgram = 61,               // 退出程序
    NCT_EnergyMode = 62,                // 节能模式
    NCT_SystemSet = 63,                 // 系统设置
    NCT_LoginInfo = 64,                 //全部注册
    NCT_Unlogin = 65,                   //全部清除注册
    NCT_LoginInfoLoop = 17,             //单个注册
    NCT_UnLoginLoop = 66,               //单个清除注册
    NCT_DeviceTestStart = 67,           //设备测试开始
    NCT_DeviceReset = 68,               //设备复位
    NCT_DeviceAdd = 69,                 //修改设备地址
    NCT_DeviceType = 70,                //修改设备类型
    NCT_ReadProtectInfor = 71,          //读取保护参数
    NCT_WriteProtectInfor = 72,         //写保护参数
    NCT_DeviceDefaultOn = 73,            //灯具默认点亮
    NCT_DeviceDirection = 74,           //标志灯方向控制
    NCT_InputEmergency = 75,            //开关量联动应急（输入）
    NCT_ReadDeviceInfo = 76,            //读设备信息
    NCT_FlashControl = 77,              //闪灯控制
    NCT_ChangeControl = 78,             //可变状态控制
    NCT_CloseBatteryOut = 79,           //自带电池灯具关闭备电输出
    NCT_SetLoopOutStatus = 80,          //配电箱应急回路输出控制
    NCT_ReadLoopOutStatus = 81,         //读取配电箱应急回路输出状态
    NCT_RecordPage = 82,                //信息记录页码
    NCT_DeviceDefaultOff = 83,          //灯具默认熄灭
    NCT_DeviceDefaultDrection = 84,     //灯具默认方向

    NCT_XmlNone = 100,                          // 空命令类型//服务端数据回复
    NCT_XmlHeartbeat  = 101,                    // 心跳
    NCT_XmlApplyForProjectID = 102,             // 申请项目识别号
    NCT_XmlUploadDeviceInfo = 103,              // 上传应急疏散监控主机信息
    NCT_XmlCanDeviceInfo = 104,                 // CAN设备信息
    NCT_XmlChannelInfo = 105,                   // 通道信息
    NCT_XmlLampInfo = 106,                      // 灯具信息
    NCT_XmlLayoutPage = 107,                    // 布局页面信息
    NCT_XmlPicture = 108,                       // 背景图片
    NCT_XmlLampCoordinate = 109,                // 灯具坐标
    NCT_XmlFirePoint = 110,                    // 着火点信息
    NCT_XmlFirePointCoordinate = 111,          // 着火点坐标
    NCT_XmlLampToFirePoint = 112,              // 灯具与着火点关联
    NCT_XmlLampDefaultDirection = 113,         // 灯具默认方向
    NCT_XmlHostElectricParameter = 114,        // 主机电参量
    NCT_XmlCanDeviceElectricParameter = 115,   // CAN设备电参量
    NCT_XmlEnableUpload = 116,                 // 使能开关
    NCT_XmlHostStateUpload = 117,              // 主机故障状态
    NCT_XmlCanDeviceStateUpload = 118,         // CAN设备故障状态
    NCT_XmlChannelStateUpload = 119,           // 通道故障状态
    NCT_XmlLampStateUpload  = 120,             // 灯具状态
    NCT_XmlEmergencyInputUpload = 121,         // 开关量输入报警
    NCT_XmlManualLaunchUpload = 122,           // 手动应急
    NCT_XmlUploadFirePointWarning = 123,       // 着火点报警
    NCT_XmlUploadLampDirection = 124,          // 灯具指向
    NCT_XmlResetDeclare = 125,                 // 复位申明
    NCT_XmlHostControl = 126,                  // 主机控制命令
    NCT_XmlLampControl = 127,                  // 灯具控制命令
    NCT_XmlQueryAllState = 128,                // 当前的所有状态
    NCT_XmlStatus = 129,                       // 通讯状态
    NCT_XmlLampConnection = 130,               // 灯具连接信息
    NCT_XmlLampSoftwareInfo = 131,             // 灯具软件信息
    NCT_XmlDeviceSoftwareInfo = 132,           // 集中电源软件信息
    NCT_XmlDeviceRealtimeData = 133,           // 集中电源运行参数
    NCT_XmlEventList = 134,                    // 查询记录信息

    NCT_Emergency = 85,                       // 发送应急指令
    NCT_FlashConfig = 86,                     // 闪灯配置
};

enum TcpType{
    TCP_None = 0,                       // 空命令类型
    TCP_Socket = 1,                     // 云平台
    TCP_Server = 2,                     // 服务端
    TCP_All = 3,                        // 都存在
};

#endif // SHARDENUM_H
