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

// 原始的消息处理器
//void myMessageOutput(QtMsgType type, const char *msg)
//{
//    switch (type) {
//    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s\n", msg);
//        break;
//    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s\n", msg);
//        break;
//    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s\n", msg);
//        break;
//    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s\n", msg);
//        abort(); // 立即终止程序
//    }
//}

// 异常信号处理器
//void signalHandler(int signum)
//{
//    // 在这里执行异常处理逻辑，比如记录日志、提示用户等
//    qDebug() << "Caught signal: " << signum;
//}

int main(int argc, char *argv[])
{
    QApplication::setStyle("plastique");
    QApplication a(argc, argv);

//    qInstallMsgHandler(myMessageOutput); // 安装自定义消息处理器

//    // 注册信号处理器
//    signal(SIGSEGV, signalHandler); // 段错误
//    signal(SIGABRT, signalHandler); // abort 调用
//    signal(SIGILL, signalHandler);  // 非法指令
//    signal(SIGFPE, signalHandler);  // 浮点异常
//    signal(SIGTERM, signalHandler); // 终止信号
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

// 异常处理函数的实现
//void handleException()
//{
//    // 记录错误信息到日志文件
//    QFile logFile("/home/xfss/root/error.txt");
//    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
//    {
//        QTextStream stream(&logFile);
//        stream << "An exception occurred.\n";
//        // 可以进一步记录异常信息
//        logFile.close();
//    }

//    // 提示用户程序发生了错误
//    QMessageBox::critical(NULL, "Error", "An exception occurred. Please check error.log for details.");
//}
