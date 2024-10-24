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

    NCT_SimulateFire = 18,               // 模拟火警点应急
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
    NCT_CanportInfo = 32,             // 控制器信息
    NCT_GetCanportInfo = 33,          // 获取控制器信息
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
    NCT_Emergency = 85,                 //发送应急指令
    NCT_FlashConfig = 86,               //闪灯配置
    NCT_ActiveInputEmergency = 87,      //有源输入联动应急
    NCT_PassiveInputEmergency = 88,     //无源输入联动应急
    NCT_ControllerParam = 89,           //控制器电参量
    NCT_RemoveFaultLamp = 90,           //清除故障灯具注册状态
    NCT_TestLinkageCom = 91,            //测试火报通讯口
    NCT_LinkageComBaudRate = 92,        //火报通讯口波特率
    NCT_NologinSendToDistribution = 93 //未注册灯具下发到集中电源指定回路
};

enum TcpType{
    TCP_None = 0,                       // 空命令类型
    TCP_Socket = 1,                     // 云平台
    TCP_Server = 2,                     // 服务端
    TCP_All = 3                         // 都存在
};

#endif // SHARDENUM_H
