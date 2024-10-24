#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <signal.h>
#include "widget.h"
#include "keyboard/ckbcontext.h"
#include "cfilterobject.h"
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <QtGlobal>

// 全局变量保存关键信息
QStringList debugLogs;

// 原始的消息处理器
void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtDebugMsg:
        // 保存最近的几条 qDebug 打印
        debugLogs.append(QString::fromUtf8(msg));
        if (debugLogs.size() > 10) {
            debugLogs.pop_front(); // 保留最近的10条
        }
        break;
    default:
            break;
    }
}


// 异常信号处理器
void signalHandler(int signum)
{
    QString message = "程序故障！请拍照后联系软件开发人员！！！\n\n";
    // 获取信号的字符串描述
    const char *sigStr;
    if (signum >= 0 && signum < NSIG)
    {
        sigStr = sys_siglist[signum];
    }
    if (sigStr != NULL) {
        message += "Signal received: " + QString(sigStr) + "\n";
    }
    // 添加之前的 qDebug 打印数据到信息中
    message += "Previous qDebug logs:\n";
    QList<QString>::const_iterator it;
    for (it = debugLogs.constBegin(); it != debugLogs.constEnd(); ++it) {
        message += *it + "\n";
    }
//    QFont font;
//    font.setPointSize(12); // 设置字体大小为12像素
    // 在msgBox中设置字体
    QMessageBox msgBox;
//    msgBox.setFont(font);
    msgBox.setWindowTitle("Signal Caught");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
    QCoreApplication::quit(); // 示例：简单处理，退出应用
}

int main(int argc, char *argv[])
{
    QApplication::setStyle("plastique");
    QApplication a(argc, argv);

    // 设置自定义的消息处理函数
    qInstallMsgHandler(myMessageOutput); // 安装自定义消息处理器

    // 注册信号处理器
    signal(SIGSEGV, signalHandler); // 段错误
    signal(SIGABRT, signalHandler); // abort 调用
    signal(SIGILL, signalHandler);  // 非法指令
    signal(SIGFPE, signalHandler);  // 浮点异常
    signal(SIGTERM, signalHandler); // 终止信号
   //获取并设置程序为自定义样式
    QFile styleSheet(":/images/check.qss");
    styleSheet.open(QIODevice::ReadOnly);
    a.setStyleSheet(styleSheet.readAll());

    a.setWindowIcon(QIcon(":/images/SINEWAVE.ICO"));

    QTextCodec* utf8 = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForCStrings(utf8);
    QTextCodec::setCodecForTr(utf8);
    QTextCodec::setCodecForLocale(utf8);

    QApplication::addLibraryPath(QCoreApplication::applicationDirPath() + QLatin1String("/plugins"));

    a.setInputContext(new CKBContext());    /* keyboard support */

    Widget w;

    /*capture keymouse */
    CFilterObject event;
    a.installEventFilter(&event);
    QObject::connect(&event, SIGNAL(keyMouseChanged()), &w, SLOT(slotEventChange()));
    
    return a.exec();
}

