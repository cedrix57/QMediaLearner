#include "ffmpegvideorendercontrol.h"

#include <QAbstractVideoSurface>

//====================================
FFmpegVideoRenderControl::FFmpegVideoRenderControl(
        QObject *parent) :
    QVideoRendererControl(parent){
    this->_surface = NULL;
}
//====================================
QAbstractVideoSurface *FFmpegVideoRenderControl::surface() const{
    return this->_surface;
}
//====================================
void FFmpegVideoRenderControl::setSurface(
        QAbstractVideoSurface *surface){
    this->_surface = surface;
}
//====================================
void FFmpegVideoRenderControl::setDisplayedFrame(
        QVideoFrame frame){
    if(this->_surface != NULL){
        //TODO warning if format not supported
        this->_surface->present(
                    frame);
    }
}
//====================================

