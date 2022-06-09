#include "GLWidgets/Camera.h"

#include <QtGui/QVector2D>

namespace gui
{

    //-----------------------------------------------------------------------------
    Camera::Camera() : m_rotSpeed(0.5f)
    //-----------------------------------------------------------------------------
    {
        resetParameters();
    }

    //-----------------------------------------------------------------------------
    Camera::~Camera()
    //-----------------------------------------------------------------------------
    {
    }

    //-----------------------------------------------------------------------------
    void Camera::setZoom(double p_zoom)
    //-----------------------------------------------------------------------------
    {
        m_zoom = p_zoom;

        pushProjectionMatrix(100, 100, m_nearPlane, m_farPlane);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::zoom(float p_delta)
    //-----------------------------------------------------------------------------
    {
        if (p_delta > 0)
            m_zoom += m_zoom*0.04f;
        else
            m_zoom -= m_zoom*0.04f;

        pushProjectionMatrix(100, 100, m_nearPlane, m_farPlane);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::reset(bool p_resetTrMatrix)
    //-----------------------------------------------------------------------------
    {
        m_rotMatrix.setToIdentity();
        if (p_resetTrMatrix)
            m_trMatrix.setToIdentity();
        m_viewMatrix.setToIdentity();
    }

    //-----------------------------------------------------------------------------
    void Camera::setXRotation(int angle)
    //-----------------------------------------------------------------------------
    {
        normalizeAngle(&angle);

        m_rotMatrix.rotate(angle / 16.0f, 1.0f, 0.0f, 0.0f);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::setYRotation(int angle)
    //-----------------------------------------------------------------------------
    {
        normalizeAngle(&angle);

        m_rotMatrix.rotate(angle / 16.0f, 0.0f, 1.0f, 0.0f);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::setZRotation(int angle)
    //-----------------------------------------------------------------------------
    {
        normalizeAngle(&angle);

        m_rotMatrix.rotate(angle / 16.0f, 0.0f, 0.0f, 1.0f);
        updateViewMatrices();
    }

    //---------------------------------------------------------------------------------------
    void Camera::rotate(float p_x, float p_y)
    //---------------------------------------------------------------------------------------
    {
        const QVector2D vect(p_x, p_y);
        const float angle = m_rotSpeed * vect.length();

        m_rotMatrix.rotate(angle, QVector3D(-p_y, p_x, 0.f) * m_rotMatrix);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::normalizeAngle(int* p_angle)
    //-----------------------------------------------------------------------------
    {
        while (*p_angle < 0)
            *p_angle += 360 * 16;
        while (*p_angle > 360 * 16)
            *p_angle -= 360 * 16;
    }

    //-----------------------------------------------------------------------------
    void Camera::setTranslation(const QVector3D& p_translation)
    //-----------------------------------------------------------------------------
    {
        m_trMatrix.translate(p_translation);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::incXTranslation(float p_dx)
    //-----------------------------------------------------------------------------
    {
        if (p_dx != 0.f)
        {
            m_trMatrix.translate(QVector3D(p_dx, 0, 0));
            updateViewMatrices();
        }
    }

    //-----------------------------------------------------------------------------
    void Camera::incYTranslation(float p_dy)
    //-----------------------------------------------------------------------------
    {
        if (p_dy != 0.f)
        {
            m_trMatrix.translate(QVector3D(0, p_dy, 0));
            updateViewMatrices();
        }
    }

    //-----------------------------------------------------------------------------
    void Camera::setScaling(float p_scaling)
    //-----------------------------------------------------------------------------
    {
        m_scalingMatrix.scale(p_scaling, p_scaling, p_scaling);
        updateViewMatrices();
    }

    //-----------------------------------------------------------------------------
    void Camera::pushProjectionMatrix(double p_width, double p_height, double p_znear, double p_zfar, bool p_preserveW)
    //-----------------------------------------------------------------------------
    {
        const double ratio = (m_viewPort[3] > 0) ? (static_cast<double>(m_viewPort[2]) / static_cast<double>(m_viewPort[3])) : 1.0;

        m_projMatrix.setToIdentity();

        double halfW = p_width / 2. / m_zoom;
        double halfH = p_height / 2. / m_zoom;

        if (p_preserveW)
            halfH = halfW / ratio;
        else
            halfW = halfH * ratio;

        m_projMatrix.ortho(-halfW, halfW, -halfH, halfH, p_znear, p_zfar);
    }

    //---------------------------------------------------------------------------------------
    void Camera::configure(const QVector4D& p_viewPort, float p_nearPlane, float p_farPlane)
    //---------------------------------------------------------------------------------------
    {
        m_viewPort = p_viewPort;
        m_nearPlane = p_nearPlane;
        m_farPlane = p_farPlane;

        pushProjectionMatrix(100, 100, m_nearPlane, m_farPlane);
    }

    //-----------------------------------------------------------------------------
    void Camera::pushTrackballCamera()
    //-----------------------------------------------------------------------------
    {
        pushProjectionMatrix(100, 100, m_nearPlane, m_farPlane);
    }

    //-----------------------------------------------------------------------------
    void Camera::resetParameters()
    //-----------------------------------------------------------------------------
    {
        m_zoom = 1.0;

        m_farPlane = 1000;
        m_nearPlane = -1000;

        m_viewMatrix.setToIdentity();
        m_projMatrix.setToIdentity();
        m_normalMatrix.setToIdentity();

        m_rotMatrix.setToIdentity();
        m_trMatrix.setToIdentity();
        m_scalingMatrix.setToIdentity();
    }

    //---------------------------------------------------------------------------------------
    void Camera::lookAt(const QVector3D & p_eye, const QVector3D & p_target, const QVector3D & p_up)
    //---------------------------------------------------------------------------------------
    {
        m_position = p_eye;

        m_viewMatrix.setToIdentity();
        m_trMatrix.setToIdentity();
        m_rotMatrix.setToIdentity();

        m_rotMatrix.lookAt(p_eye, p_target, p_up);

        updateViewMatrices();
    }

    //---------------------------------------------------------------------------------------
    void Camera::updateViewMatrices()
    //---------------------------------------------------------------------------------------
    {
        m_viewMatrix = m_trMatrix * m_rotMatrix * m_scalingMatrix;
        m_normalMatrix = m_viewMatrix.normalMatrix();
    }

    //---------------------------------------------------------------------------------------
    void Camera::setCameraConfig(const QMatrix4x4& p_rotMatrix, const QMatrix4x4& p_trMatrix, const QMatrix4x4& p_scalingMatrix, double p_zoom)
    //---------------------------------------------------------------------------------------
    {
        setZoom(p_zoom);

        m_rotMatrix = p_rotMatrix;
        m_trMatrix = p_trMatrix;
        m_scalingMatrix = p_scalingMatrix;
        updateViewMatrices();
    }

}
