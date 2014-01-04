#include "ffmpegaudiodecodercontrol.h"

//====================================
FFmpegAudioDecoderControl::FFmpegAudioDecoderControl(
        QObject *parent) :
    QAudioDecoderControl(parent){
    this->_device = NULL;
    this->_state = QAudioDecoder::StoppedState;
}
//====================================
QAudioDecoder::State FFmpegAudioDecoderControl::state() const{
    return this->_state;
}
//====================================
QString FFmpegAudioDecoderControl::sourceFilename() const{
    return this->_sourceFileName;
}
//====================================
void FFmpegAudioDecoderControl::setSourceFilename(
        const QString &fileName){
    this->_sourceFileName = fileName;

}
//====================================
QIODevice* FFmpegAudioDecoderControl::sourceDevice() const{
    return this->_device;
}
//====================================
void FFmpegAudioDecoderControl::setSourceDevice(
        QIODevice *device){
    this->_device = device;
}
//====================================
void FFmpegAudioDecoderControl::start(){
    this->_state = QAudioDecoder::DecodingState;
}
//====================================
void FFmpegAudioDecoderControl::stop(){
    this->_state = QAudioDecoder::StoppedState;
}
//====================================
QAudioFormat FFmpegAudioDecoderControl::audioFormat() const{
    return this->_format;
}
//====================================
void FFmpegAudioDecoderControl::setAudioFormat(
        const QAudioFormat &format){
    this->_format = format;
}
//====================================
QAudioBuffer FFmpegAudioDecoderControl::read(){
    QAudioBuffer buffer;
    return buffer;
}
//====================================
bool FFmpegAudioDecoderControl::bufferAvailable() const{
    return true;
}
//====================================
qint64 FFmpegAudioDecoderControl::position() const{
    return 1004;
}
//====================================
qint64 FFmpegAudioDecoderControl::duration() const{
    return 30000;
}
//====================================
