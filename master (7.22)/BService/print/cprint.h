#ifndef CPRINT_H
#define CPRINT_H

#include <QSet>
#include <QDateTime>

class QTimer;
class CPrint : public QObject
{
    Q_OBJECT
public:
    enum PrintType{ Start = 1, Fault = 2, Other = 3, Error = 4 };

    explicit CPrint(const int bufferSize = 1000);
    ~CPrint();
    static QByteArray utf8ToGb2312(const QString& str);
    void setPrintBusy(bool busy){m_bBusy = busy;}
    void addPrintType(const PrintType type) { m_needPrintType.insert(type); }
    void delPrintType(const PrintType type) { m_needPrintType.remove(type); }
    bool isPrintType(const PrintType type) const { return m_needPrintType.contains(type); }
    void print(const PrintType type, const QString& content, const QDateTime& time = QDateTime::currentDateTime());

private slots:
//    void printDataList();

private:
    QString getTypeString(const PrintType type) const;

private:
    int m_printBufferSize;
    bool m_bBusy;
    QSet<PrintType> m_needPrintType;
    QList<QByteArray> m_printDataList;
    QTimer* m_timer;
};

#endif // CPRINT_H
