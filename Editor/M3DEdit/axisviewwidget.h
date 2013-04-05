#ifndef AXISVIEWWIDGET_H
#define AXISVIEWWIDGET_H

#include <QGLWidget>

namespace M3DEditGUI{
class AxisViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit AxisViewWidget(QWidget *parent = 0);
};
}

#endif // AXISVIEWWIDGET_H
