#include "graph.h"
#include "edge.h"
#include "graph_node.h"
#include <QtGui>
#include <math.h>

graph::graph(QWidget *parent, Node* head)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-400, -400, 800, 800);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(800, 800);
    setWindowTitle(tr("Elastic Nodes"));
    centerNode = construct(scene, head, 0, -200);
}

graph_node* graph::construct(QGraphicsScene* scene, Node* elem, int x, int y)
{
    graph_node* element = new graph_node(this, QString::number(elem->w));
    scene->addItem(element);
    if(elem->left != nullptr)
        scene->addItem(new Edge(element, construct(scene, elem->left, x+75, y-y*0.5)));
    if(elem->right != nullptr)
        scene->addItem(new Edge(element, construct(scene, elem->right, x-75, y-y*0.5)));
    element->setPos(x,y);
    return element;
}

void graph::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void graph::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        foreach (QGraphicsItem *item, scene()->items()) {
            if (qgraphicsitem_cast<graph_node *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void graph::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    QList<graph_node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (graph_node *node = qgraphicsitem_cast<graph_node *>(item))
            nodes << node;
    }

    foreach (graph_node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (graph_node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void graph::wheelEvent(QWheelEvent *event)
{
    scaleView(pow( 2.0 , -event->delta() / 240.0));
}

void graph::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect)

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::yellow);
    gradient.setColorAt(1, Qt::darkYellow);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Click and drag the nodes around, and zoom with the mouse "
                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}

void graph::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

