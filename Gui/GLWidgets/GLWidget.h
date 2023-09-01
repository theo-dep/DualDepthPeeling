#pragma once

#include "GLWidgets/Camera.h"
#include "GLWidgets/Scene.h"

#include <QtGui/QColor>
#include <QtGui/QFont>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>
#include <QtOpenGLWidgets/QOpenGLWidget>
#else
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtWidgets/QOpenGLWidget>
#endif
#include <QtWidgets/QGestureEvent>
#include <QtCore/QString>
#include <QtCore/QPoint>

namespace gui
{

    class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
    {
    public:
        GLWidget(QWidget* p_parent = nullptr);
        virtual ~GLWidget();

        // Set camera rotation in degrees!

        void setXRotation(int angle);
        void setYRotation(int angle);
        void setZRotation(int angle);
        void setTranslation(const QVector3D& p_translation);
        void setScaling(float scaling);
        void setZoom(double p_zoom);
        void resetCameraParameters();

    protected:
        virtual void resizeGL( int w, int h ) override;
        virtual void initializeGL() override;

        virtual void mousePressEvent(QMouseEvent* p_event) override;
        virtual void mouseMoveEvent(QMouseEvent*  p_event) override;
        virtual void wheelEvent(QWheelEvent* p_event) override;

        bool event(QEvent * p_event) override;
        virtual bool gestureEvent(QGestureEvent * p_event);
        void pinchTriggered(QPinchGesture *gesture);

    protected:
        Scene m_scene;
        Camera m_camera;

        QPoint m_lastPos; // 2D pos of mouse
    };

}
