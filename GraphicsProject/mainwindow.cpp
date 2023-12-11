#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CanvasRectItem.h"
#include "canvastextitem.h"
#include "canvasbarcodeitem.h"
//#include "decoratoritemgraphics.h"
#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width = 1400;
    int height = 1400;

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

    qDebug() << "ui->graphicsView->rect"<< ui->graphicsView->rect();
    ui->graphicsView->setScene(m_scene);
//    ui->graphicsView->setGeometry(0, 0, width, height);
    qDebug() << "ui->graphicsView->geometry:"<< ui->graphicsView->geometry();
    qDebug() << "ui->graphicsView->sceneRect:"<< ui->graphicsView->sceneRect();
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
//    ui->graphicsView->setCursor(QCursor());
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
    font.setPixelSize(100);
    textItem = new CanvasTextItem();
//    textitem2 = new QGraphicsSimpleTextItem("jpkD");
//    textitem2->setFont(font);
//    textitem2->setFlags(QGraphicsItem::ItemIsSelectable |
//                        QGraphicsItem::ItemIsMovable);
//    textitem2->setPos(textItem->pos());
    qDebug() << "textItem->pos"<< textItem->pos();
//    qDebug() << "textitem2->pos"<< textitem2->pos();
//    textItem->setCurrentText("123");
//    textItem->setCurrentFont(font);
    //    RectGrabber* gabber = new RectGrabber(QSizeF(8, 8));
    //    DecoratorItemGraphics* item = new DecoratorItemGraphics(rectItem, gabber);
    m_scene->addItem(textItem);
//    m_scene->addItem(textitem2);
}


void MainWindow::on_barcodeBtn_clicked()
{
    CanvasBarcodeItem *barcodeitem = new CanvasBarcodeItem();
    m_scene->addItem(barcodeitem);
}

void MainWindow::on_underLineBtn_clicked(bool checked)
{
    qDebug() << "checked:"<< checked;
    QFont font = textItem->getCurrentFont();
    font.setUnderline(true);
    font.setOverline(true);
    font.setItalic(true);
    font.setStrikeOut(true);
    font.setBold(true);
    textItem->setCurrentFont(font);
}
