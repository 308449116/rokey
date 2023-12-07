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

    m_scene = new SceneGraphics(this);
    m_scene->setSceneRect(0,0,400,400);

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

