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
#include <QPainter>
//====================================
void FFmpegVideoRenderControl::setDisplayedFrame(
        QSharedPointer<QImage> image){
    qDebug() << "aa1";
    if(this->_surface != NULL){
       qDebug() << "aa2";
        static int i = 0;
        //image->save("/home/cedric/Images/atmp2/qmediaLearner" + QString::number(i) + ".bmp");
        /*
        i++;
        QPainter painter;
        painter.begin(image.data());
        painter.setPen(QPen(QColor(Qt::green)));
        painter.setBrush(QBrush(QColor(Qt::green), Qt::NoBrush));
        QFont font = painter.font();
        font.setPointSize(80);
        painter.setFont(font);
        painter.drawText(
                    QRectF(100, 100, 200, 200),
                    QString::number(i));
        painter.end();
        //*/
        QVideoFrame videoFrame(*image);
       qDebug() << "aa3";
        //TODO warning if format not supported
        QVideoFrame::PixelFormat frameFormat
                = videoFrame.pixelFormat();
        QVideoSurfaceFormat surfaceFormat
                = this->_surface->surfaceFormat();
        bool isSurfaceFormatValid = surfaceFormat.isValid();
       qDebug() << "aa4";
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
       qDebug() << "aa5";
        bool isFormatSupported = supportedFormats.contains(frameFormat);
        qDebug() << "isFormatSupported: "<< isFormatSupported;
        this->_surface->present(
                    videoFrame);
       qDebug() << "aa6";
    }
}
//====================================
//void FFmpegVideoRenderControl::setFFmpegPlayerSession(
        //FFmpegPlayerSession *session){
    //this->session = session;
//}
//====================================

