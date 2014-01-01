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
//#include <qmediaserviceprovider_p.h>
//#include <QMediaServiceProviderFactoryInterface>
//#include <QMediaServiceProviderPlugin>
//#include <QMediaServiceProviderHint>

#include <ffmpegmultimediaplugin.h>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    //ML::SequenceExtractor::loadExtractors();
    //QMediaServiceProvider* defaultServiceProvider
            //= QMediaServiceProvider::defaultServiceProvider();
    //QMediaServiceProviderPlugin *ffmpegBackend
            //= new FFmpegMultimediaPlugin;
    //QString objName = serviceProvider->objectName();
    //QMediaServiceProviderPlugin::setDefaultServiceProvider(
                //ffmpegBackend);
    //QStringList styles = QStyleFactory::keys();
    //app.setStyle("Windows");
    /*
    QMediaPlayer *player = new QMediaPlayer();
    QVideoWidget videoWidget;
    player->setVideoOutput(&videoWidget);
    QGraphicsScene scene;
    QGraphicsProxyWidget *proxy
            = scene.addWidget(&videoWidget);
    QGraphicsView view(&scene);
    player->setMedia(
                QUrl::fromLocalFile("/home/cedric/Vidéos/musique_21sec.mp4"));
    player->play();
    view.show();
    //*/
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
