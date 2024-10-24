#ifndef CDEVICETYPE_H
#define CDEVICETYPE_H

#include <QObject>
#include <QPixmap>
#include <QHash>

class CDeviceType : public QObject
{
    Q_OBJECT
public:
    explicit CDeviceType(QObject *parent = 0);
    CDeviceType(const int type, const QString name, QObject* parent = 0);

    void setName(const QString name) { m_name = name; }
    QString name() { return m_name; }
    void setType(const int type) { m_type = type; }
    int type() { return m_type; }

    void setRawImgReal(const QString& str);
    QByteArray& rawImgReal();

    void setRawImgStandard(const QString &str);
    QByteArray& rawImgStandard();

    QPixmap imageStandard(qreal s = 1.0f) const;
    QPixmap imageReal(qreal s = 1.0f) const;
    QPixmap image(qreal s = 1.0f);

private:
    QPixmap loadImageFromData(qreal s, const QByteArray& data, bool bReal = true) const;
    mutable QHash<QString, QPixmap> m_hashRealPixs;
    mutable QHash<QString, QPixmap> m_hashStandardPixs;
    int m_type;
    QString m_name;
    QPixmap m_imgStandard;  // standard device picture
    QPixmap m_imgReal;  // real device picture
    QByteArray m_rawImgReal;
    QByteArray m_rawImgStandard;
    bool m_bDetector;
    bool m_bDefined;
    bool m_bReserve;
    bool m_bRotation;

    bool m_bNew;
    bool m_bModify;
};

#endif // CDEVICETYPE_H
