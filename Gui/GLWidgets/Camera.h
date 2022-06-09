#pragma once

#include <QtGui/QMatrix4x4>

namespace gui
{

    class Camera
    {
    public:
        Camera();
        virtual ~Camera();

        const QMatrix4x4& viewMatrix() const { return m_viewMatrix; }
        const QMatrix4x4& projMatrix() const { return m_projMatrix; }
        const QMatrix3x3& normalMatrix() const { return m_normalMatrix; }
        const QMatrix4x4& rotMatrix() const { return m_rotMatrix; }
        const QMatrix4x4& trMatrix() const { return m_trMatrix; }
        const QMatrix4x4& scalingMatrix() const { return m_scalingMatrix; }
        const QVector4D& viewPort() const { return m_viewPort; }
        const QVector3D& position() const { return m_position; }

        void setXRotation(int angle);
        void setYRotation(int angle);
        void setZRotation(int angle);
        void setTranslation(const QVector3D& p_translation);
        void rotate(float p_x, float p_y);

        void incXTranslation(float p_dx);
        void incYTranslation(float p_dy);

        void setScaling(float scaling);
        void zoom(float p_delta); // zoom in if delta>0, zoom out if delta<0

        void pushTrackballCamera();

        void setZoom(double p_zoom);
        double getZoom() const { return m_zoom; }

        void resetParameters();

        void configure(const QVector4D& p_viewPort, float p_nearPlane, float p_farPlane);

        void reset(bool p_resetTrMatrix = true);

        void lookAt(const QVector3D & p_position, const QVector3D & p_target, const QVector3D & p_up);

        void setCameraConfig(const QMatrix4x4& p_rotMatrix, const QMatrix4x4& p_trMatrix, const QMatrix4x4& p_scalingMatrix, double p_zoom);

    protected:
        void pushProjectionMatrix(double p_width, double p_height, double p_zmin, double p_zfar, bool p_preserveW = true);

        void normalizeAngle(int* p_angle);

        void updateViewMatrices();

    protected:
        double m_zoom; // zoom = 1: 1mm <-> 1pix

        // Projection matrix
        QMatrix4x4 m_projMatrix;
        // Model View matrix
        QMatrix4x4 m_viewMatrix;
        // Normal matrix
        QMatrix3x3 m_normalMatrix;

        /*! Min/Max view port */
        QVector4D m_viewPort;
        /*! Near camera plane distance */
        float m_nearPlane;
        /*! Far camera plane distance */
        float m_farPlane;

        // Trackball
        QMatrix4x4 m_rotMatrix;
        QMatrix4x4 m_trMatrix;
        QMatrix4x4 m_scalingMatrix;

        QVector3D m_position;

        float m_rotSpeed;
    };

}
