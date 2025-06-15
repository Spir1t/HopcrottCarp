#ifndef MATCHING_H
#define MATCHING_H
#include <QGraphicsEllipseItem>
#include <vector>
#include <QGraphicsScene>

class node : public QGraphicsObject
{
Q_OBJECT
public:
    node(float x, float y, QString name1);

    ~node();

    //add to scene
    void addToScene(QGraphicsScene * scene);

    //connect to rightNode
    void connectTo(node * rightNode, QGraphicsScene * scene);
    void connectToSolution(node * rightNode, QGraphicsScene * scene);

    //delete connection with right node
    void deleteConnection(node * rightNode, QGraphicsScene * scene);
    void deleteSolution(node * rightNode, QGraphicsScene * scene);

    //visibility
    void hideNode();
    void showNode();

    //properties
    //connections
    std::vector<node *> matchings;
    node * solutionNode;

    //geometrical properties
    QPoint center;
    QPoint leftHost;
    QPoint rightHost;
    QGraphicsEllipseItem * shape;
    QString strName;
    QGraphicsTextItem * name;

    //connections
    std::vector<QGraphicsLineItem*> connections;
    QGraphicsLineItem * solutionLine;

    //states
    bool hasSolution;

    //junk
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget) override;
};

class matching : public QGraphicsObject
{
Q_OBJECT
public:
    matching(int leftNum, int rightNum);
    matching();
    ~matching();

    //connect
    void connectLeftWithRight(int left, int right);
    void connectSolution(int left, int right);

    //delete connection
    void deleteConnection(int left, int right);
    void deleteConnection(node * left, node * right);
    void deleteSolution(int left, int right);

    //add nodes
    void addLeft();
    void addRight();

    //add to scene
    void addToScene(QGraphicsScene * scene);
    void setScene(QGraphicsScene * scene1);

    //visibility
    void hideMatching();
    void showMatching();

    //nodes
    std::vector<node *> leftNodes;
    std::vector<node *> rightNodes;

    //states
    bool isNull;

    //scene
    QGraphicsScene * scene;

    //junk
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget) override;
};


#endif // MATCHING_H
