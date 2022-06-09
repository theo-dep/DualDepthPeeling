#pragma once

#include "Renderers/AbstractRenderer.h"

#include <Geom/Point.h>

namespace gui::gl
{

    /**
     * Class to render a GL path.
     */
    class PathRenderer final : public AbstractRenderer
    {
    public:
        //!< Constructor with complex path
        explicit PathRenderer(const QList<geom::Point>& p_pointContainer, const QVector3D& p_color, const Scene& p_scene, const Camera& p_camera, bool p_renderWithStrip = false);
        //!< Constructor with simple path between 2 points
        explicit PathRenderer(const geom::Point& p_point1, const geom::Point& p_point2, const QVector3D& p_color, const Scene& p_scene, const Camera& p_camera, bool p_renderWithStrip = false);
        virtual ~PathRenderer(void);

        inline void setThickness(GLfloat p_thickness) { m_thickness = p_thickness; }

        void render(void) override;

    protected:
        void init(const QList<geom::Point>& p_pointContainer, const QVector4D& p_color);
        void init(const geom::Point& p_point1, const geom::Point& p_point2, const QVector4D& p_color);

        inline const float* vertices() const { return m_vertexObject.data(); }
        inline const QVector4D* colors() const { return m_colors.data(); }

        bool isOtherGlFunctionsInitialized(void) const override;
        bool updateOtherGlFunctions(void) override;
        bool initOtherGlFunctions(void) override;
        void deleteOtherGlFunctions(void) override;

        inline bool isRenderTargetsInitialized(void) const override { return true; }
        inline bool updateRenderTargets(int, int) override { return true; };
        inline bool initRenderTargets(int, int) override { return true; };
        inline void deleteRenderTargets(void) override {};

        inline bool isShadersInitialized(void) const override { return m_shaderPath.isLinked(); }
        bool initShaders(void) override;
        void deleteShaders(void) override;

    private:
        GLuint m_vaoID; // array object: data access
        GLuint m_vboID; // buffer object: data

        GLfloat m_thickness;

        QVector<float> m_vertexObject;
        QVector<QVector4D> m_colors;

        bool m_renderWithStrip;

        QOpenGLShaderProgram m_shaderPath;

        Q_DISABLE_COPY_MOVE(PathRenderer);
    };

}
