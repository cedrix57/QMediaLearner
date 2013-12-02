#ifndef CLICKABLESLIDER_H
#define CLICKABLESLIDER_H

#include <QSlider>

//====================================
class ClickableSlider : public QSlider{
    Q_OBJECT
public:
    explicit ClickableSlider(QWidget *parent = 0);
    
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    int _getSliderValFromClick(QPoint pos);
    bool clicked;


    
};
//====================================

#endif // CLICKABLESLIDER_H
