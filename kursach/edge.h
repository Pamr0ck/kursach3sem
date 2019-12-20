#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class graph_node;

class Edge : public QGraphicsItem
{
public:
    Edge(graph_node *sourceNode, graph_node *destNode);

    graph_node *sourceNode() const;
    graph_node *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    graph_node *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif
