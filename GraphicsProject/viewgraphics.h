#ifndef VIEWGRAPHICS_H
#define VIEWGRAPHICS_H

#include <QGraphicsView>

class ViewGraphics : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ViewGraphics(QWidget* parent = nullptr);

signals:

};

#endif // VIEWGRAPHICS_H
