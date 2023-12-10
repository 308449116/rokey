#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CanvasRectItem.h"
#include "canvastextItem.h"
//#include "decoratoritemgraphics.h"
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width = 400;
    int height = 400;

    QRectF rect(-width/2, -height/2, width, height);
    m_scene = new SceneGraphics(this);
    m_scene->setSceneRect(rect);

    QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemSendsGeometryChanges);

    QPen pen;
    pen.setWidth(2);
    pen.setStyle(Qt::DashLine);
    QGraphicsLineItem *lineItemX = new QGraphicsLineItem(rect.left(), 0, rect.left() + rect.width(), 0);
    QGraphicsLineItem *lineItemY = new QGraphicsLineItem(0, rect.top(), 0, rect.top() + rect.height());
    lineItemX->setPen(pen);
    lineItemY->setPen(pen);

    m_scene->addItem(rectItem);
    m_scene->addItem(lineItemX);
    m_scene->addItem(lineItemY);
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setCursor(QCursor());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_shpeBtn_clicked()
{
    CanvasRectItem* rectItem = new CanvasRectItem();
    rectItem->setSize(100, 100);
//    RectGrabber* gabber = new RectGrabber(QSizeF(8, 8));
//    DecoratorItemGraphics* item = new DecoratorItemGraphics(rectItem, gabber);
    m_scene->addItem(rectItem);
}


void MainWindow::on_textBtn_clicked()
{
    QFont font;
    font.setPixelSize(50);
    CanvasTextItem* textItem = new CanvasTextItem();
//    textItem->setCurrentText("123");
//    textItem->setCurrentFont(font);
    //    RectGrabber* gabber = new RectGrabber(QSizeF(8, 8));
    //    DecoratorItemGraphics* item = new DecoratorItemGraphics(rectItem, gabber);
    m_scene->addItem(textItem);
}


void MainWindow::on_imageBtn_clicked()
{
    QGraphicsRectItem *rectItem = new QGraphicsRectItem(QRectF(0,0,100,100));
    rectItem->setFlags(QGraphicsItem::ItemIsSelectable |
                       QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemSendsGeometryChanges);
    m_scene->addItem(rectItem);

}
