#ifndef GRAPH_H
#define GRAPH_H
#include <QMainWindow>
#include <QGraphicsView>
#include "archivator.h"

class graph_node;

class graph : public QGraphicsView
{
    Q_OBJECT

public:
    graph(QWidget *parent = nullptr, Node* head = nullptr);
    graph_node* construct(QGraphicsScene*, Node*, int, int);
    void itemMoved();
private:
    int timerId;
    graph_node *centerNode = nullptr;
protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scaleView(qreal scaleFactor);
};

#endif // GRAPH_H
