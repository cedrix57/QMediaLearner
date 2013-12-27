#ifndef FONTMANAGERSINGLETON_H
#define FONTMANAGERSINGLETON_H

#include <QObject>
#include <QMap>

namespace ML{

class FontManagerSingleton : public QObject{
    Q_OBJECT
public:
    static FontManagerSingleton *getInstance();
    QStringList getFontNames();
    QString getFontPath(QString fontName);
    QString getFontDirectory();

signals:
    
public slots:
    
protected:
    explicit FontManagerSingleton(QObject *parent = 0);
    static QSharedPointer<FontManagerSingleton>
    instance;
    void loadFonts();
    QMap<QString, QString> fontPaths;
    int lastFontId;
};

}

#endif // FONTMANAGERSINGLETON_H
