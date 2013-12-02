#include "ClickableSlider.h"
#include <QStyleOptionSlider>
#include <QMouseEvent>

//====================================
ClickableSlider::ClickableSlider(QWidget *parent) :
    QSlider(parent){
    this->clicked = false;
}
//====================================
void ClickableSlider::mousePressEvent(
        QMouseEvent *event){
    QSlider::mousePressEvent(event);
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr
            = style()->subControlRect(
                QStyle::CC_Slider,
                &opt,
                QStyle::SC_SliderHandle,
                this);
    QPoint pos = event->pos();
    if (event->button() == Qt::LeftButton &&
        !sr.contains(pos)){
        int newVal = this->_getSliderValFromClick(pos);
        this->setValue(newVal);
        emit this->sliderMoved(newVal);
        event->accept();
    }
}
//====================================
int ClickableSlider::_getSliderValFromClick(QPoint pos){
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr
            = style()->subControlRect(
                QStyle::CC_Slider,
                &opt,
                QStyle::SC_SliderHandle,
                this);
    int val = 0;
    if (!sr.contains(pos)){
        int range = this->maximum() - this->minimum();
        if (orientation() == Qt::Vertical){
            val
                    = this->minimum()
                    + (range
                       * (height()-pos.y()) )
                    / height();
        }else{
            val
                    = this->minimum()
                    + (range * pos.x())
                    / width();
        }
        if (this->invertedAppearance()){
            val =  maximum() - val;
        }
    }
    return val;
}
//====================================
void ClickableSlider::mouseMoveEvent(
        QMouseEvent *event){
    QSlider::mouseMoveEvent(event);
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr
            = style()->subControlRect(
                QStyle::CC_Slider,
                &opt,
                QStyle::SC_SliderHandle,
                this);
    QPoint pos = event->pos();
    if (this->clicked &&
        !sr.contains(event->pos())){
        int newVal = this->_getSliderValFromClick(pos);
        this->setValue(newVal);
        emit this->sliderMoved(newVal);
        event->accept();
    }
}
//====================================
void ClickableSlider::mouseReleaseEvent(
        QMouseEvent *event){
    QSlider::mouseReleaseEvent(event);
    this->clicked = false;
}
//====================================

