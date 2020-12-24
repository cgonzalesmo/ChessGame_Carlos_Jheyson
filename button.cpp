#include "button.h"
#include <QBrush>
#include <QGraphicsTextItem>

Button::Button(const QString & path, int width, int height, QGraphicsItem *parent)
    :QGraphicsPixmapItem (parent)
{
    setPixmap(QPixmap(path).scaled(width, height));
}


void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
    emit clicked();
}
