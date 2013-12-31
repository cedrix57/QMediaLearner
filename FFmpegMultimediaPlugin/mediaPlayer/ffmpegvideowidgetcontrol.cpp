#include "ffmpegvideowidgetcontrol.h"
#include <QPainter>

//====================================
VideoWidget::VideoWidget(
        QWidget *parent)
    :QWidget(parent){
}
//====================================
void VideoWidget::setDisplayedFrame(
        QVideoFrame frame){
    this->currentFrame = frame;
}
//====================================
void VideoWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    if (this->currentFrame.map(
                QAbstractVideoBuffer::ReadOnly)){

         //const QTransform oldTransform = painter.transform();
         //if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
            //painter.scale(1, -1);
            //painter.translate(0, -widget->height());
         //}

         QVideoFrame::PixelFormat format
                 = this->currentFrame.pixelFormat();
         QImage::Format imageFormat
                 = QVideoFrame::imageFormatFromPixelFormat(
                     format);
         if(imageFormat != QImage::Format_Invalid){
             QImage image(
                     this->currentFrame.bits(),
                     this->currentFrame.width(),
                     this->currentFrame.height(),
                     this->currentFrame.bytesPerLine(),
                     imageFormat);
             QRect sourceRect = image.rect();
             QRect targetRect = this->rect();
             painter.drawImage(targetRect, image, sourceRect);
             //painter.setTransform(oldTransform);
             this->currentFrame.unmap();
         }
     }
}
//====================================
void VideoWidget::resizeEvent(QResizeEvent *event){
}
//====================================
FFmpegVideoWidgetControl::FFmpegVideoWidgetControl(QObject *parent) :
    QVideoWidgetControl(parent){
    this->_aspectRatioMode = Qt::KeepAspectRatio;
    this->_brightness = 100;
    this->_contrast = 100;
    this->_hue = 100;
    this->_saturation = 100;
}
//====================================
void FFmpegVideoWidgetControl::setDisplayedFrame(
        QVideoFrame frame){
    this->_videoWidget.setDisplayedFrame(frame);
}
//====================================
QWidget *FFmpegVideoWidgetControl::videoWidget(){
    return &this->_videoWidget;
}
//====================================
Qt::AspectRatioMode
FFmpegVideoWidgetControl::aspectRatioMode() const{
    return this->aspectRatioMode();
}
//====================================
void FFmpegVideoWidgetControl::setAspectRatioMode(
        Qt::AspectRatioMode mode){
    this->_aspectRatioMode = mode;
}
//====================================
bool FFmpegVideoWidgetControl::isFullScreen() const{
    bool is = this->_videoWidget.isFullScreen();
    return is;
}
//====================================
void FFmpegVideoWidgetControl::setFullScreen
(bool fullScreen){
    this->_videoWidget.setWindowState(
                Qt::WindowFullScreen);
}
//====================================
int FFmpegVideoWidgetControl::brightness() const{
    return this->_brightness;
}
//====================================
void FFmpegVideoWidgetControl::setBrightness(
        int brightness){
    this->_brightness = brightness;
}
//====================================
int FFmpegVideoWidgetControl::contrast() const{
    return this->_contrast;
}
//====================================
void FFmpegVideoWidgetControl::setContrast(
        int contrast){
    this->_contrast = contrast;
}
//====================================
int FFmpegVideoWidgetControl::hue() const{
    return this->_hue;
}
//====================================
void FFmpegVideoWidgetControl::setHue(int hue){
    this->_hue = hue;
}
//====================================
int FFmpegVideoWidgetControl::saturation() const{
    return this->_saturation;
}
//====================================
void FFmpegVideoWidgetControl::setSaturation(
        int saturation){
    this->_saturation = saturation;
}
//====================================

