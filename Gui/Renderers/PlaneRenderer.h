#pragma once

#include "Renderers/AbstractRenderer.h"

#include <Geom/Plane.h>

#include <array>

namespace gui::gl
{
    class PathRenderer;

    /**
     * Class to render a GL plane.
     */
    class PlaneRenderer : public AbstractRenderer
    {
    public:
        //!< Constructor with complex path
        explicit PlaneRenderer(const Scene& p_scene, const Camera& p_camera);
        //!< Constructor with simple path between 2 points
        virtual ~PlaneRenderer(void);

        void clear(void);

        void setPlane(const geom::Plane& p_plane, const QVector3D& p_color);

        inline void setOpacity(GLfloat p_opacity) { m_opacity = p_opacity; }
        inline void setPlaneWidth(GLfloat p_width) { m_planeWidth = p_width; }
        inline void setPlaneShift(GLfloat p_shift) { m_planeShift = p_shift; }
        void setThickness(GLfloat p_thickness);

        void render(void) override;

    protected:
        inline bool isOtherGlFunctionsInitialized(void) const override { return true; }
        inline bool updateOtherGlFunctions(void) override { return true; }
        inline bool initOtherGlFunctions(void) override { return true; }
        inline void deleteOtherGlFunctions(void) override {}

        bool isRenderTargetsInitialized(void) const override;
        bool updateRenderTargets(int, int) override;
        bool initRenderTargets(int, int) override;
        void deleteRenderTargets(void) override;

        inline bool isShadersInitialized(void) const override { return m_shaderPlane.isLinked(); }
        bool initShaders(void) override;
        void deleteShaders(void) override;

    private:
        GLuint m_planeVertexArrayId, m_planePositionBufferId;

        geom::Plane m_plane;
        std::array<GLfloat, 12> m_planeBuffer;

        static constexpr size_t SQUARE_EDGE{ 4u };
        using PathRendererList = std::array<PathRenderer*, SQUARE_EDGE>;
        PathRendererList m_pathRendererList;

        QVector3D m_color;
        GLfloat m_opacity;
        GLfloat m_planeWidth;
        GLfloat m_planeShift;

        QOpenGLShaderProgram m_shaderPlane;

        Q_DISABLE_COPY_MOVE(PlaneRenderer);
    };

}
