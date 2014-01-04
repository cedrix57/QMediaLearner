#include "ffmpegvideorendercontrol.h"

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QDebug>

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
        QVideoFrame::PixelFormat frameFormat
                = frame.pixelFormat();
        QVideoSurfaceFormat surfaceFormat
                = this->_surface->surfaceFormat();
        bool isSurfaceFormatValid = surfaceFormat.isValid();
        qDebug() << "isSurfaceFormatValid: "<< isSurfaceFormatValid;
        qDebug() << "frameFormat: "<< frameFormat;
        qDebug() << "surfaceFormat: "<< surfaceFormat;
        if(!isSurfaceFormatValid){
            QSize frameSize = frame.size();
            QVideoSurfaceFormat rightSurfaceFormat(
                        frameSize,
                        frameFormat);
            bool formatSet = this->_surface->start(rightSurfaceFormat);
            qDebug() << "formatSet: "<< formatSet;

        }
        QList<QVideoFrame::PixelFormat> supportedFormats
                = this->_surface->supportedPixelFormats();
        bool isFormatSupported = supportedFormats.contains(frameFormat);
        qDebug() << "isFormatSupported: "<< isFormatSupported;
        this->_surface->present(
                    frame);
    }
}
//====================================

