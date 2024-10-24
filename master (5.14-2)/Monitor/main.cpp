#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include <QMessageBox>
#include <QTextStream>
#include "widget.h"
#include "keyboard/ckbcontext.h"
#include "cfilterobject.h"

// 声明异常处理函数
void handleException();

int main(int argc, char *argv[])
{
    QApplication::setStyle("plastique");
    QApplication a(argc, argv);

    // 设置异常处理函数
//    std::set_terminate(handleException);

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
