#ifndef STRUCT_H
#define STRUCT_H

#include <QDateTime>

// ??Monitor??????
enum NCommandType{
    NCT_None = 0,                       // ����������
    NCT_GetProcessTypeName = 1,         // ��ȡ�������������ƶ�Ӧ��
    NCT_SetInterfaceBoardInfo = 2,      // ���ýӿڰ���Ϣ,Dll����Server
    NCT_SetRouteConfig = 3,             // ����·������,Server����Dll
    NCT_ObjectStatus = 4,               // ���Ͷ���״̬
    NCT_AddLog = 5,                     // �����־
    NCT_GetLog = 6,                     // ��ȡ��־
    NCT_Reset = 7,                      // ��λ
    NCT_Mute = 8,                       // ����
    NCT_EmergencyStart = 9,             // ϵͳӦ������
    NCT_EmergencyStop = 10,              // ϵͳӦ��ֹͣ
    NCT_AutomaticEvacuation = 11,        // �Զ���ɢģʽ
    NCT_ManualEvacuation = 12,           // �ֶ���ɢģʽ
    NCT_SystemSelfCheck = 13,            // ϵͳ�Լ�
    NCT_SystemMonthCheck = 14,           // ϵͳ�¼�
    NCT_SystemYearCheck = 15,            // ϵͳ���
    NCT_Relogin = 16,                    // ���µ�¼

    NCT_ManualFire = 18,                 // �ֶ�����𾯵���ɢ
    NCT_PrintSet = 19,                   // ��ӡ����
    NCT_CheckSet = 20,                   // �������
    NCT_DirectionTest = 21,              // �������
    NCT_FactoryReset = 22,               // �ָ���������
    NCT_AutoFire = 23,                  // ����Ա�û���Ϣ
    NCT_CheckTimeSync = 24,
    NCT_SelfCheckStart = 25,             // ϵͳ�Լ쿪ʼ
    NCT_SelfCheckStop = 26,              // ϵͳ�Լ����
    NCT_UndefinedObject = 27,            // ����δ����Object
    NCT_ClientReset = 28,
    NCT_ForceEmergency = 29,             // ǿ��Ӧ����������
    NCT_Log = 30,                        // ��־����
    NCT_LedStatus = 31,           // ����״̬ͬ��
    NCT_ControllerInfo = 32,             // ��������Ϣ
    NCT_GetControllerInfo = 33,          // ��ȡ��������Ϣ
    NCT_StopCommunication = 34,                 // ������ֹͣ
    NCT_StartCommunication = 35,                // ����������
//    NCT_GetCurrentValue = 36,            // ��ȡ��·������Ϣ
//    NCT_GetVoltageValue = 37,            // ��ȡ��·��ѹ��Ϣ
    NCT_DuplicateCheck = 36,
    NCT_ChangeDeviceAddress = 37,
    NCT_ClearUndefinedObject = 38,       // ���δ�������ݶ���
    NCT_AddUndefinedObject = 39,         // ���δ�������ݶ���
    NCT_DelUndefinedObject = 40,         // ɾ��δ�������ݶ���
    NCT_MonthCheckBegin = 41,            // �¼쿪ʼ
    NCT_MonthCheckEnd = 42,              // �¼����
    NCT_YearCheckBegin = 43,             // ��쿪ʼ
    NCT_YearCheckEnd = 44,               // ������
    NCT_ProjectInitComplete = 45,  // �ͻ��˹��̳�ʼ�����
    NCT_CancelMute = 46,                 // ȡ������
    NCT_PowerInfo = 47,                  // ���е�Դ��Ϣ
    NCT_StartEvacuationGroup = 48,       // ������ɢԤ��
    NCT_StartFlowEvacuationGroup = 49,   // ������ɢԤ������������
    NCT_StopEvacuationGroup = 50,        // ֹͣ��ɢԤ��
    NCT_DeviceEmergencyStart = 51,       // �豸Ӧ������
    NCT_DeviceEmergencyStop = 52,        // �豸Ӧ��ֹͣ
    NCT_GetLayerEvacuationFlow = 53,     // ��ȡͼ�����
    NCT_LayerEvacuationFlow = 54,        // ͼ�����
    NCT_ClearEvacuationFlow = 55,        // ���������
    NCT_HasFASSystem = 56,               // ���ڱ���ϵͳ
    NCT_StartSpeedUp = 57,               // ��ʼģ�����
    NCT_StopSpeedUp = 58,                // ����ģ�����
    NCT_StartCharge = 59,               // ������
    NCT_StopCharge = 60,                // ��ֹ���
    NCT_ExitProgram = 61,               // �˳�����
    NCT_EnergyMode = 62,                // ����ģʽ
    NCT_SystemSet = 63,                 // ϵͳ����
    NCT_LoginInfo = 64,                 //ȫ��ע��
    NCT_Unlogin = 65,                   //ȫ�����ע��
    NCT_LoginInfoLoop = 17,             //����ע��
    NCT_UnLoginLoop = 66,               //�������ע��
    NCT_DeviceTestStart = 67,           //�豸���Կ�ʼ
    NCT_DeviceReset = 68,               //�豸��λ
    NCT_DeviceAdd = 69,                 //�޸��豸��ַ
    NCT_DeviceType = 70,                //�޸��豸����
    NCT_ReadProtectInfor = 71,          //��ȡ��������
    NCT_WriteProtectInfor = 72,         //д��������
    NCT_DeviceDefaultOn = 73,           //�ƾ�Ĭ�ϵ���
    NCT_DeviceDirection = 74,           //��־�Ʒ������
    NCT_InputEmergency = 75,            //����������Ӧ�������룩
    NCT_ReadDeviceInfo = 76,            //���豸��Ϣ
    NCT_FlashControl = 77,              //���ƿ���
    NCT_ChangeControl = 78,             //�ɱ�״̬����
    NCT_CloseBatteryOut = 79,           //�Դ���صƾ߹رձ������
    NCT_SetLoopOutStatus = 80,          //�����Ӧ����·�������
    NCT_ReadLoopOutStatus = 81,         //��ȡ�����Ӧ����·���״̬
    NCT_RecordPage = 82,                //��Ϣ��¼ҳ��
    NCT_DeviceDefaultOff = 83,          //�ƾ�Ĭ��Ϩ��
    NCT_DeviceDefaultDrection = 84,     //�ƾ�Ĭ�Ϸ���


    NCT_XmlNone = 100,                          // ����������
    NCT_XmlHeartbeat  = 101,                    // ����
    NCT_XmlApplyForProjectID = 102,             // ������Ŀʶ���
    NCT_XmlUploadDeviceInfo = 103,              // �ϴ�Ӧ����ɢ���������Ϣ
    NCT_XmlCanDeviceInfo = 104,                 // CAN�豸��Ϣ
    NCT_XmlChannelInfo = 105,                   // ͨ����Ϣ
    NCT_XmlLampInfo = 106,                      // �ƾ���Ϣ
    NCT_XmlLayoutPage = 107,                    // ����ҳ����Ϣ
    NCT_XmlPicture = 108,                       // ����ͼƬ
    NCT_XmlLampCoordinate = 109,                // �ƾ�����
    NCT_XmlFirePoint = 110,                    // �Ż����Ϣ
    NCT_XmlFirePointCoordinate = 111,          // �Ż������
    NCT_XmlLampToFirePoint = 112,              // �ƾ����Ż�����
    NCT_XmlLampDefaultDirection = 113,         // �ƾ�Ĭ�Ϸ���
    NCT_XmlHostElectricParameter = 114,        // ���������
    NCT_XmlCanDeviceElectricParameter = 115,   // CAN�豸�����
    NCT_XmlEnableUpload = 116,                 // ʹ�ܿ���
    NCT_XmlHostStateUpload = 117,              // ��������״̬
    NCT_XmlCanDeviceStateUpload = 118,         // CAN�豸����״̬
    NCT_XmlChannelStateUpload = 119,           // ͨ������״̬
    NCT_XmlLampStateUpload  = 120,             // �ƾ�״̬
    NCT_XmlEmergencyInputUpload = 121,         // ���������뱨��
    NCT_XmlManualLaunchUpload = 122,           // �ֶ�Ӧ��
    NCT_XmlUploadFirePointWarning = 123,       // �Ż�㱨��
    NCT_XmlUploadLampDirection = 124,          // �ƾ�ָ��
    NCT_XmlResetDeclare = 125,                 // ��λ����
    NCT_XmlHostControl = 126,                  // ������������
    NCT_XmlLampControl = 127,                  // �ƾ߿�������
    NCT_XmlQueryAllState = 128,                // ��ǰ������״̬
    NCT_XmlStatus = 129,                       // ͨѶ״̬
    NCT_XmlLampConnection = 130,               // �ƾ�������Ϣ
    NCT_XmlLampSoftwareInfo = 131,             // �ƾ������Ϣ
    NCT_XmlDeviceSoftwareInfo = 132,           // ���е�Դ�����Ϣ
    NCT_XmlDeviceRealtimeData = 133,           // ���е�Դ���в���
    NCT_XmlEventList = 134,                    // ��ѯ��¼��Ϣ


    NCT_Emergency = 85,                       // ����Ӧ��ָ��
    NCT_FlashConfig = 86,                     // ��������
};

enum TcpType{
    TCP_None = 0,                       // ����������
    TCP_Socket = 1,                     // ��ƽ̨
    TCP_Server = 2,                     // �����
    TCP_All = 3,                        // ������
};

#endif // STRUCT_H
