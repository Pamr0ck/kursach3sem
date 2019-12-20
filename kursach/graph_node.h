#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H


#include <QGraphicsItem>
#include <QList>
#include <QString>


class Edge;
class graph;
class QGraphicsSceneMouseEvent;
class graph_node : public QGraphicsItem
{
public:
    graph_node(graph *graphWidget, QString data);


    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QList<Edge *> edgeList;
    QPointF newPos;
    graph *graphh;
    QString data;
};

#endif // GRAPH_NODE_H
