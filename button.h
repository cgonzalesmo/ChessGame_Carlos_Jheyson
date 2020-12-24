#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Button: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Button(const QString & path, int width = 250, int height = 86, QGraphicsItem* parent=nullptr);
    ~Button() = default;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
signals:
    void clicked();
};
#endif // BUTTON_H
