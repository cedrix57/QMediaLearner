#include "ffmpegvideorendercontrol.h"

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QDebug>


//#include "ffmpegplayersession.h"

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
        QSharedPointer<QImage> image){
    qDebug() << "this->_surface != NULL?";
    if(this->_surface != NULL){
        qDebug() << "Displaying frame...";
        image->save("/home/cedric/Vidéos/debug.bmp");
        QVideoFrame videoFrame(*image);
        //TODO warning if format not supported
        QVideoFrame::PixelFormat frameFormat
                = videoFrame.pixelFormat();
        QVideoSurfaceFormat surfaceFormat
                = this->_surface->surfaceFormat();
        bool isSurfaceFormatValid = surfaceFormat.isValid();
        qDebug() << "isSurfaceFormatValid: "<< isSurfaceFormatValid;
        qDebug() << "frameFormat: "<< frameFormat;
        qDebug() << "surfaceFormat: "<< surfaceFormat;
        if(!isSurfaceFormatValid){
            QSize frameSize = videoFrame.size();
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
                    videoFrame);
                    //*/
    }
}
//====================================
//void FFmpegVideoRenderControl::setFFmpegPlayerSession(
        //FFmpegPlayerSession *session){
    //this->session = session;
//}
//====================================

