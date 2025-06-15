#include "matching.h"
#include "qpainter.h"

node::node(float x, float y, QString name1)
{
    setX(x + 25);
    setY(y + 25);
    center = QPoint(x + 25, y + 25);
    leftHost = QPoint(x, y + 25);
    rightHost = QPoint(x + 50, y + 25);
    shape = new QGraphicsEllipseItem(x, y, 50, 50);
    strName = name1;
}

node::~node()
{
    solutionNode = nullptr;

    //geometrical properties
    delete shape;
    delete name;
}

void node::connectTo(node *rightNode, QGraphicsScene * scene)
{
    this->deleteConnection(rightNode, scene);
    this->deleteSolution(rightNode, scene);
    this->matchings.push_back(rightNode);
    rightNode->matchings.push_back(this);
    QLine line(this->rightHost, rightNode->leftHost);
    this->connections.push_back(scene->addLine(line));
}

void node::connectToSolution(node *rightNode, QGraphicsScene *scene)
{
    this->deleteConnection(rightNode, scene);
    this->deleteSolution(rightNode, scene);

    this->solutionNode = rightNode;
    this->hasSolution = true;

    rightNode->solutionNode = this;
    this->hasSolution = true;

    QLine line(this->rightHost, rightNode->leftHost);
    QPen pen(Qt::red, 3);
    this->solutionLine = new QGraphicsLineItem;
    solutionLine = scene->addLine(line, pen);
}

void node::deleteConnection(node *rightNode, QGraphicsScene *scene)
{
    int k = 0;
    if(this->matchings.size() == 0)
    {
        return;
    }
    for(node * someNode : this->matchings)
    {
        if(someNode == rightNode)
        {
            this->matchings.erase(matchings.begin() + k);
        }
        k += 1;
    }
    k = 0;
    for(node * someNode : rightNode->matchings)
    {
        if(someNode == this)
        {
            rightNode->matchings.erase(rightNode->matchings.begin() + k);
        }
        k += 1;
    }
    for(QGraphicsLineItem * line : this->connections)
    {
        if(QPoint(line->line().x1(), line->line().y1()) == this->rightHost && QPoint(line->line().x2(), line->line().y2()) == rightNode->leftHost)
        {
            if(line != nullptr)
            {
                qDebug() << "center" << rightNode->center;
                delete line;
                qDebug() << "lineEnding" << QPoint(line->line().x2(), line->line().y2());
            }
        }
    }
}

void node::deleteSolution(node * rightNode, QGraphicsScene * scene)
{
    this->solutionNode = nullptr;
    this->hasSolution = false;

    rightNode->solutionNode = nullptr;
    rightNode->hasSolution = false;
}

void node::hideNode()
{
    this->shape->setVisible(false);
    this->name->setVisible(false);
    if(solutionLine != nullptr)
    {
        this->solutionLine->setVisible(false);
    }
    if(!this->connections.empty())
    {
    for(QGraphicsLineItem * line : this->connections)
    {
       line->setVisible(false);
    }
    }
}

void node::showNode()
{
    this->shape->setVisible(true);
    this->name->setVisible(true);
    if(solutionLine != nullptr)
    {
        this->solutionLine->setVisible(true);
    }
    if(!this->connections.empty())
    {
    for(QGraphicsLineItem * line : this->connections)
    {
       line->setVisible(true);
    }
    }
}


void node::addToScene(QGraphicsScene *scene)
{
    scene->addItem(this);
    scene->addItem(this->shape);
    name = scene->addText(strName);
    name->setPos(center - QPoint(5, 10));
}

matching::matching(int leftNum, int rightNum)
{
    for(int i = 0; i < leftNum; i++)
    {
        node * node1 = new node(0, 0 + i * 75, QString::number(i + 1));
        this->leftNodes.push_back(node1);
    }
    for(int i = 0; i < rightNum; i++)
    {
        node * node1 = new node(300, 0 + i * 75, QString('A' + i));
        this->rightNodes.push_back(node1);
    }
}

matching::matching()
{
    this->isNull = true;
}

matching::~matching()
{
    for(node * a : this->leftNodes)
    {
        delete a;
    }
    for(node * a : this->rightNodes)
    {
        delete a;
    }
}

void matching::connectLeftWithRight(int left, int right)
{
    leftNodes[left]->connectTo(rightNodes[right], scene);
}

void matching::connectSolution(int left, int right)
{
    leftNodes[left]->connectToSolution(rightNodes[right], scene);
}

void matching::deleteConnection(int left, int right)
{
    leftNodes[left]->deleteConnection(rightNodes[right], scene);
}

void matching::deleteConnection(node * left, node * right)
{
    left->deleteConnection(right, scene);
}

void matching::deleteSolution(int left, int right)
{
    leftNodes[left]->deleteSolution(rightNodes[right], scene);
}

void matching::addLeft()
{
    node * node1 = new node(0, 0 + (leftNodes.size()) * 75, QString::number(leftNodes.size() + 1));
    this->leftNodes.push_back(node1);
    node1->addToScene(scene);
}

void matching::addRight()
{
    node * node1 = new node(300, 0 + (rightNodes.size()) * 75, QString('A' + int(rightNodes.size())));
    this->rightNodes.push_back(node1);
    node1->addToScene(scene);
}



void matching::addToScene(QGraphicsScene *scene)
{
    scene->addItem(this);
    for(node * a : this->leftNodes)
    {
        a->addToScene(scene);
    }
    for(node * a : this->rightNodes)
    {
        a->addToScene(scene);
    }
}

void matching::setScene(QGraphicsScene *scene1)
{
    this->scene = scene1;
}

void matching::hideMatching()
{
    for(node * node1 : this->leftNodes)
    {
        node1->hideNode();
    }
    for(node * node1 : this->rightNodes)
    {
        node1->hideNode();
    }
}

void matching::showMatching()
{
    for(node * node1 : this->leftNodes)
    {
        node1->showNode();
    }
    for(node * node1 : this->rightNodes)
    {
        node1->showNode();
    }
}


//junk
QRectF node::boundingRect() const {}

void node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

QRectF matching::boundingRect() const {}

void matching::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}
