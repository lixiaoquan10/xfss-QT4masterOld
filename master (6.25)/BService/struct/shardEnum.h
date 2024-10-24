#ifndef SHARDENUM_H
#define SHARDENUM_H

#include "cmsglog.h"
#include "cmsgadminuser.h"
#include "cmsgfileinfo.h"
#include "cmsgfiledata.h"
#include "cmsginterfaceboardinfo.h"
#include "cmsgrouteconfig.h"
#include "cmsgprocesstypename.h"
#include "cmsgobjectstatus.h"
#include "cmsgnull.h"
#include "cmsgloginfo.h"
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

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define md_sleep(n) Sleep(n)
#else
#include <unistd.h>
#define md_sleep(n) usleep(n)
#endif

enum TXINIT {
    TXINIT_SUCCESS = 0,         // 初始化成功
    TXINIT_NOT_PROJECT = -1,    // 工程文件不存在
    TXINIT_LOAD_FAIL = -2       // 加载工程文件失败
};

// 与Server通讯类型
enum MsgCommandType{
    MCT_None = 0,                   // 空命令类型
    MCT_GetAdminUser = 1,           // 获取管理员用户,Dll调用Server,暂时无用
    MCT_AddLog = 2,                 // 添加日志,Server调用Dll
    MCT_UpdateLog = 3,              // 更新日志显示,Dll调用Server
    MCT_SetInterfaceBoardInfo = 4,  // 设置接口板信息,Dll调用Server
    MCT_SetRouteConfig = 5,         // 设置路由配置,Server调用Dll
    MCT_GetProcessTypeName = 6,     // 获取处理器类型名称对应表,Server调用Dll
    MCT_FactoryReset = 7,           // 恢复出厂设置,Server调用Dll
    MCT_SendData = 8,               // 发送数据,Dll调用Server
    MCT_RecvData = 9,               // 接收数据,Server调用Dll
    MCT_ExitProgram = 10            // 退出程序,Dll调用Server
};

// 与Client通讯类型
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
    NCT_ReloginLoop = 17,                    // 重新登录
    NCT_ManualFire = 18,                 // 手动输入火警点疏散
    NCT_PrintSet = 19,                   // 打印设置
    NCT_CheckSet = 20,                   // 检测设置
    NCT_DirectionTest = 21,              // 方向测试
    NCT_FactoryReset = 22,               // 恢复出厂设置
    NCT_AdminUser = 23,                  // 管理员用户信息
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
    NCT_ExitProgram = 59                 // 退出程序
};

#endif // SHARDENUM_H
