#include "GLWidgets/GLWidget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>

namespace gui
{

    //-----------------------------------------------------------------------------
    GLWidget::GLWidget(QWidget* p_parent) : QOpenGLWidget(p_parent)
        , m_scene()
    //-----------------------------------------------------------------------------
    {
        // SBO 2020/03/03 force palette background color to black to avoid solarization effects with Qt5.12.6
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::black);
        setAutoFillBackground(true);
        setPalette(pal);

        grabGesture(Qt::PinchGesture);
    }

    //-----------------------------------------------------------------------------
    GLWidget::~GLWidget()
    //-----------------------------------------------------------------------------
    {
        if (isInitialized())
        {
            makeCurrent();
        }
    }

    //-----------------------------------------------------------------------------
    void GLWidget::resizeGL(int w, int h)
    //-----------------------------------------------------------------------------
    {
        glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
    }

    //-----------------------------------------------------------------------------
    void GLWidget::setXRotation(int angle)
    //-----------------------------------------------------------------------------
    {
        m_camera.setXRotation(16 * angle);
    }

    //-----------------------------------------------------------------------------
    void GLWidget::setYRotation(int angle)
    //-----------------------------------------------------------------------------
    {
        m_camera.setYRotation(16 * angle);
    }

    //-----------------------------------------------------------------------------
    void GLWidget::setZRotation(int angle)
    //-----------------------------------------------------------------------------
    {
        m_camera.setZRotation(16 * angle);
    }

    //-----------------------------------------------------------------------------
    void GLWidget::setTranslation(const QVector3D& p_translation)
    //-----------------------------------------------------------------------------
    {
        m_camera.setTranslation(p_translation);
    }

    //-----------------------------------------------------------------------------
    void GLWidget::setZoom(double p_zoom)
    //-----------------------------------------------------------------------------
    {
        m_camera.setZoom(p_zoom);
    }

    //-----------------------------------------------------------------------------
    void GLWidget::setScaling(float scaling)
    //-----------------------------------------------------------------------------
    {
        m_camera.setScaling(scaling);
    }

    //-----------------------------------------------------------------------------
    void GLWidget::resetCameraParameters()
    //-----------------------------------------------------------------------------
    {
        m_camera.resetParameters();
    }

    //-----------------------------------------------------------------------------
    void GLWidget::initializeGL()
    //-----------------------------------------------------------------------------
    {
        const QOpenGLContext* c = context();
        if (c != nullptr)
        {
            qDebug() << "OpenGL context version " << c->format().majorVersion() << "." << c->format().minorVersion();
        }
        else
        {
            qCritical() << "no OpenGL context available";
        }

        QOpenGLWidget::initializeGL();

        if (!initializeOpenGLFunctions())
        {
            qCritical() << "fail to initialize OpenGL functions";
            hide();
            QMessageBox::critical(nullptr, "Error", "Unable to initialize required OpenGL features, software will exit.\nYou may need to update your graphics driver.");
            qApp->quit();
        }
    }

    //-----------------------------------------------------------------------------
    void GLWidget::mousePressEvent(QMouseEvent* p_event)
    //-----------------------------------------------------------------------------
    {
        QOpenGLWidget::mousePressEvent(p_event);
        m_lastPos = p_event->pos();
        p_event->accept();
    }

    //-----------------------------------------------------------------------------
    void GLWidget::mouseMoveEvent(QMouseEvent* p_event)
    //-----------------------------------------------------------------------------
    {
        QOpenGLWidget::mouseMoveEvent(p_event);

        const QPointF move((p_event->x() - m_lastPos.x()), (m_lastPos.y() - p_event->y()));

        // Rotate with left button
        if (p_event->buttons() & Qt::LeftButton)
        {
            if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
            {
                if (move.x() != 0. || move.y() != 0.)
                {
                    m_camera.incXTranslation(move.x() / 10.);
                    m_camera.incYTranslation(move.y() / 10.);
                }
            }
            else
            {
                m_camera.rotate(move.x(), move.y());

            }
        }

        m_lastPos = p_event->pos();
        update();
        p_event->accept();
    }

    //-----------------------------------------------------------------------------
    void GLWidget::wheelEvent(QWheelEvent* p_event)
    //-----------------------------------------------------------------------------
    {
        if (p_event->delta() != 0)
        {
            m_camera.zoom(p_event->delta());

            update();
        }
    }

    //-----------------------------------------------------------------------------------------------
    bool GLWidget::event(QEvent *p_event)
    //-----------------------------------------------------------------------------------------------
    {
        if (p_event->type() == QEvent::Gesture)
        {
            return gestureEvent(static_cast<QGestureEvent*>(p_event));
        }

        return QOpenGLWidget::event(p_event);
    }

    //-----------------------------------------------------------------------------------------------
    bool GLWidget::gestureEvent(QGestureEvent *event)
    //-----------------------------------------------------------------------------------------------
    {
        if (QGesture * pinch = event->gesture(Qt::PinchGesture))
        {
            pinchTriggered(static_cast<QPinchGesture *>(pinch));
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------
    void GLWidget::pinchTriggered(QPinchGesture *gesture)
    //-----------------------------------------------------------------------------------------------
    {
        const QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
        if (changeFlags & QPinchGesture::ScaleFactorChanged)
        {
            m_camera.zoom(gesture->totalScaleFactor() > 1. ? 100 : -100);
        }

        update();
    }

}
