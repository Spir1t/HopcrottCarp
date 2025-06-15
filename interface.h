#ifndef INTERFACE_H
#define INTERFACE_H
#include <QGraphicsScene>
#include <QGraphicsView>
#include "matching.h"

class mousePressEventFilter;

class Interface : public QWidget
{
Q_OBJECT
public:
    Interface();    

    bool event(QEvent* ev) override;

    void setFilter();


    //junk
    //QRectF boundingRect() const override;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *widget) override;

public slots:
    void test();
    void addLeft();
    void addRight();
    void eraseConnections();
    void Delete();
    void hide();
    void show();

protected:
    //void mousePressEvent(QMouseEvent * event) override;


public:
    QGraphicsScene * scene;
    QGraphicsView * view;
    matching * curMatching;
    bool isNodeSelected = false;
    int potentialLeft;
    std::vector<matching *> matchings;
};

class mousePressEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit mousePressEventFilter(QObject *parent = nullptr, Interface * someInterface = nullptr);

    Interface * myInterface;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // INTERFACE_H
