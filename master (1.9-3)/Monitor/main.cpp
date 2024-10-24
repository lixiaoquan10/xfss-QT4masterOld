#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QIcon>
#include "widget.h"
#include "keyboard/ckbcontext.h"
#include "cfilterobject.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle("plastique");
    QApplication a(argc, argv);

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
    QTranslator trlApp;
    if(trlApp.load("Monitor_zh_CN.qm","/root"))
        a.installTranslator(&trlApp);

    a.setInputContext(new CKBContext());    /* keyboard support */

    Widget w;
//    w.showFullScreen();

    /*capture keymouse */
    CFilterObject event;
    a.installEventFilter(&event);
    QObject::connect(&event, SIGNAL(keyMouseChanged()), &w, SLOT(slotEventChange()));
    
    return a.exec();
}
