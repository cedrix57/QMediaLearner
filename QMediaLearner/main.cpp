#include "gui/MainWindow.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QGraphicsProxyWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QStyleFactory>
#include <Utils/log.h>
#include <SettingsManagerSingleton.h>
#include <private/qmediaserviceprovider_p.h>

/*
extern "C"
{
#include <avcodec.h>
#include <avformat.h>
}
//*/

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    char *filePathChar
            = "/home/cedric/Vidéos/musique_21sec.mp4";
    /*
    AVFormatContext *ff_formatContex;
    int ffret = avformat_open_input(
                &ff_formatContex,
                filePathChar,
                NULL,
                NULL);
                //*/
    //ML::SequenceExtractor::loadExtractors();
    //QMediaServiceProvider* defaultServiceProvider
            //= QMediaServiceProvider::defaultServiceProvider();
    //QMediaServiceProviderPlugin *ffmpegBackend
            //= new FFmpegMultimediaPlugin;
    //QString objName = serviceProvider->objectName();
    //QMediaServiceProviderPlugin::setDefaultServiceProvider(
                //ffmpegBackend);
    #ifndef QT_DEBUG
        bool saveInLog
                = SettingsManagerSingleton
                getInstance()->
                isSaveInLogFile();
        if(saveInLog){
            ML::resetLogFile();
            qInstallMessageHandler(
                        ML::fileMessageHandler);
        }
    #endif

    MainWindow mainWindow;
    mainWindow.show();
    
    int ret = app.exec();
    //ML::SequenceExtractor::closeExtractors();
    return ret;
}
