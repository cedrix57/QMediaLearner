#include "FontManagerSingleton.h"

#include <QSharedPointer>
#include <QFontDatabase>
#include <QDir>

namespace ML{

//====================================
//====================================
QSharedPointer<FontManagerSingleton> FontManagerSingleton::instance;
//====================================
FontManagerSingleton::FontManagerSingleton(
        QObject *parent) :
    QObject(parent){
    this->loadFonts();

}
//====================================
FontManagerSingleton *FontManagerSingleton::getInstance(){
    if(FontManagerSingleton::instance.isNull()){
        QSharedPointer<FontManagerSingleton>
                localInstance(
                    new FontManagerSingleton);
        FontManagerSingleton::instance = localInstance;
    }
    FontManagerSingleton *pointer
            = FontManagerSingleton::instance.data();
    return pointer;
}
//====================================
void FontManagerSingleton::loadFonts(){
    QFontDatabase::removeAllApplicationFonts();
    QString fontDirPath
            = this->getFontDirectory();
    QDir fontDir(fontDirPath);

    QFileInfoList fileInfos
            = fontDir.entryInfoList(
                QDir::AllEntries
                | QDir::NoDot
                | QDir::NoDotDot);
    while(fileInfos.size() > 0){
        QFileInfo fileInfo
                = fileInfos.takeFirst();
        bool isFile = fileInfo.isFile();
        if(isFile){
            QString filePath
                    = fileInfo.absoluteFilePath();
            if(filePath.endsWith(".ttf")
                    || filePath.endsWith(".otf")){
                int id = QFontDatabase::addApplicationFont(
                            filePath);
                if(id != -1){
                    this->lastFontId = id;
                    QString fontName
                            = QFontDatabase
                            ::applicationFontFamilies(id)
                            .first();
                    this->fontPaths
                            [fontName]
                            = filePath;
                }
            }
        }else{
            QDir currentDir
                    = fileInfo.filePath();
            QFileInfoList newFileInfos
                    = currentDir.entryInfoList(
                        QDir::AllEntries
                        | QDir::NoDot
                        | QDir::NoDotDot);
            fileInfos << newFileInfos;
        }
    }
}
//====================================
QString FontManagerSingleton::getFontDirectory(){
    QString dir = "Fonts";
    dir
            = QDir::currentPath()
            + QDir::separator()
            + dir;
    return dir;
}
//====================================
QStringList FontManagerSingleton::getFontNames(){
    QStringList fontFames
            = this->fontPaths.keys();
    return fontFames;
}
//====================================
QString FontManagerSingleton::getFontPath(
        QString fontName){
    QString fontPath
            = this->fontPaths[fontName];
    return fontPath;
}
//====================================

}

