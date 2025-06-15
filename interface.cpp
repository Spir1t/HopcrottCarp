#include "interface.h"
#include "qapplication.h"
#include <QWidget>
#include <QEvent>
#include <QPushButton>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <qtimer.h>

Interface::Interface()
{
    this->scene = new QGraphicsScene();
    this->view = new QGraphicsView(this->scene);
    this->setMouseTracking(true);
    QTimer * timer = new QTimer();
    timer->setInterval(100);
    timer->start();
    scene->setSceneRect(-100, -100, 500, 2000);

    QPushButton * startButton = new QPushButton();
    startButton->setGeometry(400, 0, 150, 50);
    startButton->setText("START");
    scene->addWidget(startButton);

    QPushButton * addLeft = new QPushButton();
    addLeft->setGeometry(400, 50, 150, 50);
    addLeft->setText("ADD LEFT");
    scene->addWidget(addLeft);

    QPushButton * addRight = new QPushButton();
    addRight->setGeometry(400, 100, 150, 50);
    addRight->setText("ADD RIGHT");
    scene->addWidget(addRight);

    QPushButton * eraseConnections = new QPushButton();
    eraseConnections->setGeometry(400, 150, 150, 50);
    eraseConnections->setText("ERASE CONNECTIONS");
    scene->addWidget(eraseConnections);

    QPushButton * Delete = new QPushButton();
    Delete->setGeometry(400, 200, 150, 50);
    Delete->setText("DELETE EVERYTHING");
    scene->addWidget(Delete);


    connect(startButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(addLeft, SIGNAL(clicked()), this, SLOT(addLeft()));
    connect(addRight, SIGNAL(clicked()), this, SLOT(addRight()));
    connect(eraseConnections, SIGNAL(clicked()), this, SLOT(eraseConnections()));
    connect(Delete, SIGNAL(clicked()), this, SLOT(Delete()));


    curMatching = new matching();

    this->matchings.push_back(curMatching);

    curMatching->setScene(scene);

    curMatching->addToScene(scene);

    view->show();
}

void Interface::setFilter()
{
    mousePressEventFilter * filter = new mousePressEventFilter(view, this);
    scene->installEventFilter(filter);
}

void Interface::hide()
{
    curMatching->hideMatching();
}

void Interface::show()
{
    curMatching->showMatching();
}

void Interface::test()
{
    qDebug() << curMatching->leftNodes;
}

void Interface::addLeft()
{
    curMatching->addLeft();
}

void Interface::addRight()
{
    curMatching->addRight();
}

void Interface::eraseConnections()
{
    for(node * node1 : curMatching->leftNodes)
    {
        int a = node1->matchings.size();
        for(int i = 0; i < node1->matchings.size(); i++)
        {
            node * node2 = node1->matchings[i];
            curMatching->deleteConnection(node1, node2);
            if(a != int(node1->matchings.size()))
            {
                a = int(node1->matchings.size());
                i--;
            }
        }
    }
}

void Interface::Delete()
{
    if(!(curMatching->leftNodes.empty() || curMatching->rightNodes.empty()))
    {
        this->eraseConnections();
        int a = curMatching->leftNodes.size();
        int b = curMatching->rightNodes.size();
        for(int i = 0; i < a; i++)
        {
            node * node1 = curMatching->leftNodes.back();
            curMatching->leftNodes.pop_back();
            delete node1;
        }

        for(int i = 0; i < b; i++)
        {
            node * node1 = curMatching->rightNodes.back();
            curMatching->rightNodes.pop_back();
            delete node1;
        }
    }
}


bool Interface::event(QEvent *ev)
{
    if(ev->type() != QEvent::MouseButtonPress || this->curMatching->leftNodes.empty() || this->curMatching->rightNodes.empty())
    {
        return false;
    }
    else
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(ev);
            if(!this->isNodeSelected)
            {
                int c = 0;
                QPointF cursor = view->mapToScene(view->mapFromGlobal((QCursor::pos())));
                if(this->curMatching->leftNodes.empty() || this->curMatching->rightNodes.empty())
                {
                    return true;
                }
                for(node * left : this->curMatching->leftNodes)
                {
                    if((cursor.x() - left->x()) * (cursor.x() - left->x())
                            +  (cursor.y() - left->y()) * (cursor.y() - left->y()) < 25 * 25)
                    {

                        this->isNodeSelected = true;
                        this->potentialLeft = c;
                        return true;
                    }
                    c++;
                }
            }
            else
            {
                int c = 0;
                for(node * right : this->curMatching->rightNodes)
                {
                    if(this->curMatching->leftNodes.empty() || this->curMatching->rightNodes.empty())
                    {
                        return true;
                    }
                    QPointF cursor = view->mapToScene(view->mapFromGlobal((QCursor::pos())));
                    if((cursor.x() - right->x()) * (cursor.x() - right->x())
                            +  (cursor.y() - right->y()) * (cursor.y() - right->y()) < 25 * 25)
                    {
                        this->curMatching->connectLeftWithRight(this->potentialLeft, c);
                        this->isNodeSelected = false;
                        this->potentialLeft = 0;
                        return true;

                    }
                    c++;
                }
                this->isNodeSelected = false;
                this->potentialLeft = 0;
            }
    }
    return false;
}


mousePressEventFilter::mousePressEventFilter(QObject *parent, Interface * someInterface) : QObject(parent)
{
    this->myInterface = someInterface;
}

bool mousePressEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(myInterface->event(event) == false)
    {
        return QObject::eventFilter(obj, event);
    }

    return(true);
}
