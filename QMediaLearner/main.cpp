#include "gui/MainWindow.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QGraphicsProxyWidget>
#include <QVideoWidget>
#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
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

    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
