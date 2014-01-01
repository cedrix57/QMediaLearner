#ifndef FFMPEGMETADATAREADERCONTROL_H
#define FFMPEGMETADATAREADERCONTROL_H

#include <QMetaDataReaderControl>
#include <QMap>

class FFmpegMetaDataReaderControl
        : public QMetaDataReaderControl{
    Q_OBJECT
public:
    explicit FFmpegMetaDataReaderControl(QObject *parent);
    virtual bool isMetaDataAvailable() const;

    virtual QVariant metaData(const QString &key) const;
    virtual QStringList availableMetaData() const;

public slots:
    void clearMetaData();
    void addMetaData(QString key, QVariant value);
    void addMetaData(QMap<QString, QVariant> &metaDatas);
    void setMetaData(QMap<QString, QVariant> &metaDatas);

protected:
    QMap<QString, QVariant> metaDatas;
    
};

#endif // FFMPEGMETADATAREADERCONTROL_H
