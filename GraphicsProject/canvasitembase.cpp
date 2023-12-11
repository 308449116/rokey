#include "canvasitembase.h"
#include "Utils/utils.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector2D>
#include <QVector3D>
#include <cmath>
#include <math.h>
#include <QJsonObject>
#include <QApplication>
//#include "NDNodeBase.h"
//#include "NDAttributeBase.h"
//#include "NDRealAttribute.h"
//#include "NDIntAttribute.h"
//#include "RALLBlockSignal.h"
//#include "UICanvasItemManager.h"
//#include "CanvasView.h"

//QImage CanvasItemBase::m_closeIcon;
//QImage CanvasItemBase::m_resizeIcon;
//QImage CanvasItemBase::m_rotateIcon;

CanvasItemBase::CanvasItemBase(QGraphicsItem* parentItem)
    :QGraphicsItem(parentItem)
    , m_cPenColor(255, 0, 0)
    , m_cBrushColor(200, 100, 100)
{
//    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
//    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
               QGraphicsItem::ItemIsFocusable |
               QGraphicsItem::ItemSendsGeometryChanges);
//    initIcon();

    // 初始化基本属性
//    initNodeBase();

    // 获取中心位置
    m_size = QSize(100, 100);
//    QPointF centerPos = g_currentCanvasManager->getCurrentCanvasView()->getCenterPos();
//    this->setPos(centerPos);
    this->setPos(0, 0);
}

void CanvasItemBase::setItemResizeable(bool resizeable)
{
    m_isResizeable = resizeable;
}

//NDNodeBase* CanvasItemBase::getCurrentNode(void) const
//{
//    return m_pNode;
//}

void CanvasItemBase::setItemResizeRatio(bool resizeRation, qreal rationValue)
{
    m_isRatioScale = resizeRation;
    m_ratioValue = rationValue;
}

QRectF CanvasItemBase::boundingRect() const
{
    QRectF rectF = getCustomRect();
    if (!this->isSelected())
        return rectF;

    rectF.adjust(-m_nInterval, -m_nInterval, m_nInterval, m_nInterval);
    rectF.adjust(-m_nEllipseWidth, -m_nEllipseWidth, m_nEllipseWidth, m_nEllipseWidth);

    return rectF;
}

void CanvasItemBase::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    // 自定义绘制
    customPaint(painter, option, widget);

    if (!this->isSelected())
        return;

    // 设置画笔
    QPen pen;
    pen.setWidth(m_nPenWidth);
    pen.setColor(m_cPenColor);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    QRectF itemRect = this->getCustomRect();
//    qDebug() << "itemRect:" <<itemRect;
    // 绘制轮廓线
    QRectF outLintRect = itemRect.adjusted(-m_nInterval, -m_nInterval, m_nInterval, m_nInterval);
    painter->drawRect(outLintRect);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(m_cBrushColor));

    // 绘制控制点
    painter->drawEllipse(outLintRect.topRight(), m_nEllipseWidth, m_nEllipseWidth);
    if (!m_closePixmap.isNull())
        painter->drawPixmap(QRect(outLintRect.topRight().x() - m_nEllipseWidth / 2, \
            outLintRect.topRight().y() - m_nEllipseWidth / 2, \
            m_nEllipseWidth, m_nEllipseWidth), m_closePixmap);

    painter->drawEllipse(outLintRect.bottomLeft(), m_nEllipseWidth, m_nEllipseWidth);
    if (!m_rotatePixmap.isNull())
        painter->drawPixmap(QRect(outLintRect.bottomLeft().x() - m_nEllipseWidth / 2, \
            outLintRect.bottomLeft().y() - m_nEllipseWidth / 2, \
            m_nEllipseWidth, m_nEllipseWidth), m_rotatePixmap);

    if (m_isResizeable)
    {
        painter->drawEllipse(outLintRect.bottomRight(), m_nEllipseWidth, m_nEllipseWidth);
        if (!m_resizePixmap.isNull())
            painter->drawPixmap(QRect(outLintRect.bottomRight().x() - m_nEllipseWidth / 2, \
                outLintRect.bottomRight().y() - m_nEllipseWidth / 2, \
                m_nEllipseWidth, m_nEllipseWidth), m_resizePixmap);
    }
}

QPainterPath CanvasItemBase::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void CanvasItemBase::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QRectF itemRect = this->getCustomRect();
    QRectF outLintRect = itemRect.adjusted(-m_nInterval, -m_nInterval, m_nInterval, m_nInterval);
    qDebug() << "1111 itemRect:" << itemRect;
    qDebug() << "1111 m_itemOper:" << m_itemOper;

    // 获取当前模式
    QPointF pos = event->pos();
    QPointF scenePos = event->scenePos();

    if (g_utilTool->getDistance(pos, outLintRect.topRight()) <= m_nEllipseWidth)
    {
//        g_currentCanvasManager->deleteCanvasItem(m_pNode);
        return;
    }
    else if (g_utilTool->getDistance(pos, outLintRect.bottomLeft()) <= m_nEllipseWidth) {
        m_itemOper = t_rotate;
    }
    else if (g_utilTool->getDistance(pos, outLintRect.bottomRight()) <= m_nEllipseWidth) {
        m_itemOper = t_resize;
    } else {
        if (itemRect.contains(pos)) {
            m_itemOper = t_move;
        }
    }
    qDebug() << "1111 m_itemOper:" << m_itemOper;

    // 保存当前的一些信息
    m_pos = pos;
    m_pressedPos = scenePos;
    m_startPos = this->pos();
    m_startRotate = m_rotate;
    m_startSize = m_size;
    qDebug() << "2222 m_startPos:" << m_startPos;
//    update();
    return QGraphicsItem::mousePressEvent(event);
}

void CanvasItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF pos = event->pos();
    qDebug() << "m_itemOper:" << m_itemOper;
    qDebug() << "2222 m_startPos:" << m_startPos;

    if (m_itemOper == t_move)
    {
        // 处理移动
        mouseMoveMoveOperator(scenePos, pos);
    }
    else if (m_itemOper == t_resize)
    {
        // 处理更改大小
        mouseMoveResizeOperator(scenePos, pos);
    }
    else if (m_itemOper == t_rotate)
    {
        // 处理旋转
        mouseMoveRotateOperator(scenePos, pos);
    }

    return QGraphicsItem::mouseMoveEvent(event);
}

void CanvasItemBase::mouseMoveMoveOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
    qreal xInterval = scenePos.x() - m_pressedPos.x();
    qreal yInterval = scenePos.y() - m_pressedPos.y();

    this->setPos(m_startPos + QPointF(xInterval, yInterval));
    this->update();
}

void CanvasItemBase::mouseMoveResizeOperator(const QPointF &scenePos, const QPointF &loacalPos)
{
    qreal itemWidth = abs(loacalPos.x()) - m_nInterval * 2 - m_nEllipseWidth;
    qreal itemHeight = abs(loacalPos.y()) - m_nInterval * 2 - m_nEllipseWidth;
    //    if (m_isRatioScale)
    //        itemHeight = itemWidth * 1.0 / ratio;
    m_scaleX = itemWidth / m_originSize.width();
    m_scaleY = itemHeight / m_originSize.height();

    //    qDebug() << "m_size:" << m_size;
    // 设置图片的最小大小为10
    if (itemWidth < 10 || itemHeight < 10)
        return;

    this->prepareGeometryChange();

    m_size = QSize(m_originSize.width() * m_scaleX, m_originSize.height() * m_scaleY);
    this->update();
}
//void CanvasItemBase::mouseMoveResizeOperator(const QPointF& scenePos, const QPointF& loacalPos)
//{
//    if (!m_isResizeable)
//        return;

//    qreal ratio = m_ratioValue;
//    qreal itemWidth = abs(loacalPos.x()) * 2 - m_nInterval - m_nEllipseWidth;
//    qreal itemHeight = abs(loacalPos.y()) * 2 - m_nInterval - m_nEllipseWidth;
////    if (m_isRatioScale)
////        itemHeight = itemWidth * 1.0 / ratio;

//    // 设置图片的最小大小为10
//    if (itemWidth < 10 || itemHeight < 10)
//        return;

//    m_size = QSize(itemWidth, itemHeight);
////    m_pWidthAttribute->setValue(m_size.width());
////    m_pHeightAttribute->setValue(m_size.height());

//    this->update();
//}

void CanvasItemBase::mouseMoveRotateOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
    QPointF originPos = this->boundingRect().center();
    // 从原点延伸出去两条线，鼠标按下时的点和当前鼠标位置所在点的连线
    QLineF p1 = QLineF(originPos, m_pos);
    QLineF p2 = QLineF(originPos, loacalPos);
    // 旋转角度
    qreal dRotateAngle = p2.angleTo(p1);

    // 设置旋转中心
    this->setTransformOriginPoint(originPos);

    // 计算当前旋转的角度
    qreal dCurAngle = this->rotation() + dRotateAngle;

    //    qDebug() << "dCurAngle" << dCurAngle;
    //    qDebug() << "this->rotation()" << this->rotation();
    //    qDebug() << "dRotateAngle" << dRotateAngle;
    //    qDebug();
    while (dCurAngle > 360.0) {
        dCurAngle -= 360.0;
    }

    this->prepareGeometryChange();

    // 设置旋转角度
    this->setRotation(dCurAngle);
    this->update();
}

//void CanvasItemBase::mouseMoveRotateOperator(const QPointF& scenePos, const QPointF& loacalPos)
//{
//    // 获取并设置为单位向量
//    QVector2D startVec(m_pos.x() - 0, m_pos.y() - 0);
//    startVec.normalize();
//    QVector2D endVec(loacalPos.x() - 0, loacalPos.y() - 0);
//    endVec.normalize();

//    // 单位向量点乘，计算角度
//    qreal dotValue = QVector2D::dotProduct(startVec, endVec);
//    if (dotValue > 1.0)
//        dotValue = 1.0;
//    else if (dotValue < -1.0)
//        dotValue = -1.0;

//    dotValue = qAcos(dotValue);
//    if (isnan(dotValue))
//        dotValue = 0.0;

//    // 获取角度
//    qreal angle = dotValue * 1.0 / (PI / 180);

//    // 向量叉乘获取方向
//    QVector3D crossValue = QVector3D::crossProduct(QVector3D(startVec, 1.0), QVector3D(endVec, 1.0));
//    if (crossValue.z() < 0)
//        angle = -angle;
//    m_rotate += angle;

//    // 设置角度在0~360之间
//    if (m_rotate >= 360)
//        m_rotate -= 360;
//    else if (m_rotate < 0)
//        m_rotate += 360;

//    // 设置变化矩阵
//    QTransform transform;
//    transform.rotate(m_rotate);
//    this->setTransform(transform);

//    this->update();
//}

void CanvasItemBase::mouseReleaseMoveOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
//    QList<NDAttributeBase*> attributes;
//    QVector<QVariant> values;
//    QVector<QVariant> oldValues;

//    attributes << m_pXPostionAttribute << m_pYPostionAttribute;
//    oldValues << m_startPos.x() << m_startPos.y();
//    values << this->pos().x() << this->pos().y();

//    g_currentCanvasManager->changedAttributeValues(attributes, oldValues);
//    g_currentCanvasManager->changedAttributeValues(attributes, values, true);
}

void CanvasItemBase::mouseReleaseResizeOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
//    m_startSize = m_size;
//    QList<NDAttributeBase*> attributes;
//    QVector<QVariant> values;
//    QVector<QVariant> oldValues;

//    attributes << m_pWidthAttribute << m_pHeightAttribute;
//    oldValues << m_startSize.width() << m_startSize.height();
//    values << m_size.width() << m_size.height();

//    g_currentCanvasManager->changedAttributeValues(attributes, oldValues);
//    g_currentCanvasManager->changedAttributeValues(attributes, values, true);
}

void CanvasItemBase::mouseReleaseRotateOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
//    qreal tempRotate = m_rotate;

//    m_pRotateAttribute->setValue(m_startRotate);
//    m_pRotateAttribute->setValue(tempRotate, true);
}
void CanvasItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF pos = event->pos();

    if (m_itemOper == t_move)
    {
        // 处理移动
        mouseReleaseMoveOperator(scenePos, pos);
    }
    else if (m_itemOper == t_resize)
    {
        // 处理更改大小
        mouseReleaseResizeOperator(scenePos, pos);
    }
    else if (m_itemOper == t_rotate)
    {
        // 处理旋转
        mouseReleaseRotateOperator(scenePos, pos);
    }

    m_itemOper = t_none;
    return QGraphicsItem::mouseReleaseEvent(event);
}

void CanvasItemBase::customPaint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

}

QRectF CanvasItemBase::getCustomRect(void) const
{
    QPointF centerPos(0, 0);
    return QRectF(centerPos.x(), centerPos.y(), m_size.width(), m_size.height());
//    return QRectF(centerPos.x() - m_size.width() / 2, centerPos.y() - m_size.height() / 2, \
//                                            m_size.width(), m_size.height());
}

//void CanvasItemBase::initIcon(void)
//{
//    QString curAppPath = qApp->applicationDirPath() + "/";

//    if (m_closeIcon.isNull())
//        m_closeIcon.load(curAppPath + "./Images/close.png");
//    if (m_resizeIcon.isNull())
//        m_resizeIcon.load(curAppPath + "./Images/resize.png");
//    if (m_rotateIcon.isNull())
//        m_rotateIcon.load(curAppPath + "./Images/rotate.png");

//    m_closePixmap = QPixmap::fromImage(m_closeIcon);
//    m_resizePixmap = QPixmap::fromImage(m_resizeIcon);
//    m_rotatePixmap = QPixmap::fromImage(m_rotateIcon);
//}


void CanvasItemBase::setCurrentIndex(int index)
{
    m_nIndex = index;
}

int CanvasItemBase::getCurrentIndex(void)
{
    return m_nIndex;
}
/*
QVariant CanvasItemBase::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
        prepareGeometryChange();
    else if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        updateAttribute();
    }

    return QGraphicsItem::itemChange(change, value);
}

void CanvasItemBase::initNodeBase(void)
{
    // 创建节点
    m_pNode = new NDNodeBase(this);

    // 添加属性组
    QString attributeGroupString = tr("Base Attribute");
    NDAttributeGroup* pGroup = m_pNode->addAttributeGroup("BaseAttr", attributeGroupString);

    // X坐标
    m_pXPostionAttribute = new NDRealAttribute;
    m_pXPostionAttribute->setDisplayName(tr("X Postion: "));
    m_pXPostionAttribute->setName("xPt");
    m_pXPostionAttribute->setValueRange(0, 5000);
    m_pNode->addAttribute(pGroup, m_pXPostionAttribute);
    // Y坐标
    m_pYPostionAttribute = new NDRealAttribute;
    m_pYPostionAttribute->setDisplayName(tr("Y Postion: "));
    m_pYPostionAttribute->setName("yPt");
    m_pYPostionAttribute->setValueRange(0, 5000);
    m_pNode->addAttribute(pGroup, m_pYPostionAttribute);
    // Z坐标
    m_pZPostionAttribute = new NDRealAttribute;
    m_pZPostionAttribute->setDisplayName(tr("Z Postion: "));
    m_pZPostionAttribute->setName("zPt");
    m_pZPostionAttribute->setValueRange(0, 10);
    m_pNode->addAttribute(pGroup, m_pZPostionAttribute);

    // 宽度
    m_pWidthAttribute = new NDIntAttribute;
    m_pWidthAttribute->setDisplayName(tr("width: "));
    m_pWidthAttribute->setName("width");
    m_pWidthAttribute->setValueRange(10, 5000);
    m_pNode->addAttribute(pGroup, m_pWidthAttribute);
    // 高度
    m_pHeightAttribute = new NDIntAttribute;
    m_pHeightAttribute->setDisplayName(tr("height: "));
    m_pHeightAttribute->setName("height");
    m_pHeightAttribute->setValueRange(10, 5000);
    m_pNode->addAttribute(pGroup, m_pHeightAttribute);

    // 旋转
    m_pRotateAttribute = new NDRealAttribute;
    m_pRotateAttribute->setDisplayName(tr("Rotate Angle: "));
    m_pRotateAttribute->setName("rotate");
    m_pRotateAttribute->setValueRange(0, 360);
    m_pRotateAttribute->setValue(0);
    m_pNode->addAttribute(pGroup, m_pRotateAttribute);

    // 连接信号和槽
    QObject::connect(m_pXPostionAttribute, &NDRealAttribute::valueChanged, this, &CanvasItemBase::onXPostionAttributeValueChanged);
    QObject::connect(m_pYPostionAttribute, &NDRealAttribute::valueChanged, this, &CanvasItemBase::onYPostionAttributeValueChanged);
    QObject::connect(m_pZPostionAttribute, &NDRealAttribute::valueChanged, this, &CanvasItemBase::onZPostionAttributeValueChanged);
    QObject::connect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &CanvasItemBase::onWidthAttributeValueChanged);
    QObject::connect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &CanvasItemBase::onHeightAttributeValueChanged);
    QObject::connect(m_pRotateAttribute, &NDRealAttribute::valueChanged, this, &CanvasItemBase::onRotateAttributeValueChanged);
}

void CanvasItemBase::updateAttribute(void)
{
    QPointF pos = this->pos();

    m_pXPostionAttribute->setValue(pos.x());
    m_pYPostionAttribute->setValue(pos.y());
    m_pZPostionAttribute->setValue(this->zValue());

    m_pWidthAttribute->setValue(m_size.width());
    m_pHeightAttribute->setValue(m_size.height());

    m_pRotateAttribute->setValue(m_rotate);
}

// 获取属性字符串
QJsonObject CanvasItemBase::getStoreJson(int interval)
{
    QJsonObject object;
    object.insert("name", m_pNode->getNodeName());
    object.insert("type", m_pNode->getNodeType());

    // 获取属性组
    QList<NDAttributeGroup*> attrGroups;
    m_pNode->getAllAttributeGroups(attrGroups);

    QJsonObject attrObject;
    for (auto iter = attrGroups.begin(); iter != attrGroups.end(); ++iter)
    {
        // 获取属性列表
        QList<NDAttributeBase*> attrs;
        (*iter)->getAttributes(attrs);

        for (auto attrIter = attrs.begin(); attrIter != attrs.end(); ++attrIter)
        {
            QString attrName = (*attrIter)->getName();
            QVariant value = (*attrIter)->getValue();

            if (attrName == "xPt" || attrName == "yPt")
                value = value.toDouble() + interval;

            attrObject.insert(attrName, QJsonValue::fromVariant(value));
        }
    }

    object.insert("attribute", attrObject);
    return object;
}

void CanvasItemBase::fillJsonInfo(const QJsonObject& jsonObject)
{
    QJsonObject object = jsonObject.value("attribute").toObject();

    for (auto iter = object.begin(); iter != object.end(); ++iter)
    {
        QString attrName = iter.key();
        QVariant value = iter.value().toVariant();

        NDAttributeBase* pAttr = m_pNode->getAttribute(attrName);
        if (pAttr == nullptr)
            continue;

        pAttr->setValue(value);
    }
}

void CanvasItemBase::onXPostionAttributeValueChanged(const QVariant& value)
{
    QPointF pos = this->pos();
    pos.setX(value.toDouble());

    this->setPos(pos);
}

void CanvasItemBase::onYPostionAttributeValueChanged(const QVariant& value)
{
    QPointF pos = this->pos();
    pos.setY(value.toDouble());

    this->setPos(pos);
}

void CanvasItemBase::onZPostionAttributeValueChanged(const QVariant& value)
{
    this->setZValue(value.toDouble());
}

void CanvasItemBase::onWidthAttributeValueChanged(const QVariant& value)
{
    if (!m_isResizeable)
    {
        RALLBlockSingal wdithBlockSingal(m_pWidthAttribute);
        m_pWidthAttribute->setValue(m_size.width());
        return;
    }

    qreal ratio = m_ratioValue;
    qreal itemWidth = value.toInt();
    qreal itemHeight = m_size.height();
    if (m_isRatioScale)
        itemHeight = itemWidth * 1.0 / ratio;

    // 设置图片的最小大小为10
    if (itemWidth < 10 || itemHeight < 10)
    {
        RALLBlockSingal wdithBlockSingal(m_pWidthAttribute);
        m_pWidthAttribute->setValue(m_size.width());
        return;
    }

    m_size.setWidth(itemWidth);
    m_size.setHeight(itemHeight);

    QObject::disconnect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &CanvasItemBase::onHeightAttributeValueChanged);
    m_pHeightAttribute->setValue((int)itemHeight);
    QObject::connect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &CanvasItemBase::onHeightAttributeValueChanged);

    this->update();
    this->prepareGeometryChange();
}

void CanvasItemBase::onHeightAttributeValueChanged(const QVariant& value)
{
    if (!m_isResizeable)
    {
        RALLBlockSingal wdithBlockSingal(m_pHeightAttribute);
        m_pHeightAttribute->setValue(m_size.height());
        return;
    }

    qreal ratio = m_ratioValue;
    qreal itemWidth = m_size.width();
    qreal itemHeight = value.toInt();
    if (m_isRatioScale)
        itemWidth = itemHeight * 1.0 * ratio;

    // 设置图片的最小大小为10
    if (itemWidth < 10 || itemHeight < 10)
    {
        RALLBlockSingal wdithBlockSingal(m_pHeightAttribute);
        m_pHeightAttribute->setValue(m_size.height());
        return;
    }

    m_size.setWidth(itemWidth);
    m_size.setHeight(itemHeight);

    QObject::disconnect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &CanvasItemBase::onWidthAttributeValueChanged);
    m_pWidthAttribute->setValue((int)itemWidth);
    QObject::connect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &CanvasItemBase::onWidthAttributeValueChanged);

    this->update();
    this->prepareGeometryChange();
}

void CanvasItemBase::onRotateAttributeValueChanged(const QVariant& value)
{
    m_rotate = value.toDouble();

    QTransform transform;
    transform.rotate(m_rotate);
    this->setTransform(transform);

    this->update();
}
*/


