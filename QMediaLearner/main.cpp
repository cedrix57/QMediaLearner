#include "gui/MainWindow.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QGraphicsProxyWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QStyleFactory>

#include <Utils/log.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
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
        ML::resetLogFile();
        qInstallMessageHandler(
                    ML::fileMessageHandler);
    #endif

    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
