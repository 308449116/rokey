#include "scenegraphics.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>

SceneGraphics::SceneGraphics(QObject* parent)
    : QGraphicsScene{parent}
{
    
}

//void SceneGraphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
    
//}

//void SceneGraphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
    
//}

//void SceneGraphics::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
    
//}

//void SceneGraphics::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
//{
    
//}

void SceneGraphics::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete: {
        foreach (QGraphicsItem *item, selectedItems()) {
            removeItem(item);
//            QList<QGraphicsItem *> itemList = item->childItems();
//            foreach (QGraphicsItem *childItem, itemList) {
//                qDebug() << "type" << childItem->type();
//                if (childItem->type() != QGraphicsItem::UserType + 1) {
//                    delete childItem;
//                    childItem = nullptr;
//                }
//            }
            delete item;
        }
//        deselectItems();
        break;
    }
    case Qt::Key_A: {
        if(QApplication::keyboardModifiers() & Qt::ControlModifier){
            foreach (QGraphicsItem *item, items()) {
                item->setSelected(true);
            }
//            if(selectedItems().length() == 1) signalSelectItem(selectedItems().at(0));
        }
        break;
    }
    default:
        break;
    }
    QGraphicsScene::keyPressEvent(event);
}

void SceneGraphics::deselectItems()
{
    foreach (QGraphicsItem *item, selectedItems()) {
        item->setSelected(false);
    }
    selectedItems().clear();
}
