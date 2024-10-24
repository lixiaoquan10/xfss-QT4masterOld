#include "cprint.h"
#include "cglobal.h"
#include "process/ccommunicationroute.h"
#include <QTimer>
#include <QTextStream>
#include <QTextCodec>
#include <QSettings>
#include <QDebug>

CPrint::CPrint(const int bufferSize) :
    QObject(), m_bBusy(false)
{
    Q_UNUSED(bufferSize);
    QSettings setting("zsyf", "Monitor");
    if(setting.value("printStart", 1).toInt())
        addPrintType(Start);
    if(setting.value("printFault", 1).toInt())
        addPrintType(Fault);
    if(setting.value("printOther", 1).toInt())
        addPrintType(Other);
    m_timer = new QTimer(this);
//    connect(m_timer, SIGNAL(timeout()), this, SLOT(printDataList()));
//    m_timer->start(1200);
}

CPrint::~CPrint()
{
    delete m_timer;
    QSettings setting("zsyf", "Monitor");
    setting.setValue("printStart", m_needPrintType.contains(Start) ? 1 : 0);
    setting.setValue("printFault", m_needPrintType.contains(Fault) ? 1 : 0);
    setting.setValue("printOther", m_needPrintType.contains(Other) ? 1 : 0);
}

QByteArray CPrint::utf8ToGb2312(const QString &str)
{
    QTextCodec* utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec* gb2312 = QTextCodec::codecForName("GB2312");
    QByteArray byte = gb2312->fromUnicode(utf8->toUnicode(str.toUtf8()));
    return byte;
}

void CPrint::print(const CPrint::PrintType type, const QString &content, const QDateTime &time)
{
    if(!m_needPrintType.contains(type))
        return;
    QByteArray byte;
    QString printStr;
    if(content.size() > 15)
    {
        QString a = content.mid(0, 15);
        QString b = content.mid(15);
        printStr = QString("%1").arg(b);
        byte.append(utf8ToGb2312(printStr));
        byte.append(0x0A);
        printStr = QString("说明:");
        byte.append(utf8ToGb2312(printStr));
        printStr = QString("%1").arg(a);
        byte.append(utf8ToGb2312(printStr));
        byte.append(0x0A);
    }else{
        printStr = QString(tr("说明:"));
        byte.append(utf8ToGb2312(printStr));
        printStr = QString("%1").arg(content);
        byte.append(utf8ToGb2312(printStr));
        byte.append(0x0A);
    }
    printStr = QString("%1").arg(getTypeString(type));
    byte.append(utf8ToGb2312(printStr));
    byte.append(0x0A);
    printStr = QString("%1").arg(time.toString("yyyy-MM-dd hh:mm:ss"));
    byte.append(utf8ToGb2312(printStr));
    byte.append(0x0A);
    m_printDataList.append(byte);
    if(m_printDataList.size() > 10000)
        m_printDataList.removeFirst();
}

//void CPrint::printDataList()
//{
//    if(m_printDataList.isEmpty())
//        return;
//    if(m_bBusy) return;
//    QByteArray data;
//    data.append(m_printDataList.takeFirst());
//    CGlobal::instance()->communicationRoute()->printData(data);
//}

QString CPrint::getTypeString(const CPrint::PrintType type) const
{
    QString str;
    switch(type)
    {
    case Start:
        str = "应急";
        break;
    case Fault:
        str = "故障";
        break;
    case Other:
        str = "其它";
        break;
    case Error:
        str = "错误";
        break;
    }
    return str;
}
