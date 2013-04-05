#ifndef RENDEREDVIEWWIDGET_H
#define RENDEREDVIEWWIDGET_H

#include <QGLWidget>

namespace M3DEditGUI{
class RenderedViewWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit RenderedViewWidget(QWidget *parent = 0);

signals:

public slots:

};
}

#endif // RENDEREDVIEWWIDGET_H
