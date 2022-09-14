#pragma once

#include "Renderers/MeshRenderer.h"

#include <Geom/Plane.h>

class MeshModel;

// GL_TEXTURE_2D does not work on old graphic cards for Dual Depth Peeling
//#define USING_GL_TEXTURE GL_TEXTURE_2D
#define USING_GL_TEXTURE GL_TEXTURE_RECTANGLE

namespace gui::gl
{
    class AbstractRenderer;

    /**
     * \class TransparencyRenderer
     * \brief Render one or a lot RMeshModel with transparency
    */
    class TransparencyRenderer : public AbstractRenderer
    {
    public:
        explicit TransparencyRenderer(const Scene& p_scene, const Camera& p_camera);
        virtual ~TransparencyRenderer(void);

        //!< AbstractRenderer::setSize call initialize but TransparencyRenderer::initialize is long for
        //!< some graphic cards. To speed up the resize event when the TransparencyRenderer is not active,
        //!< call only prepareForResize(). The isInitialized function becomes false and an initialize will
        //!< be requested when the TransparencyRenderer will be active.
        inline void prepareForResize(void) { requestUpdateRenderTargets(); }

        //!< Save the background color for rendering
        inline void setBackgroundColor(const QVector3D& p_color) { m_backgroundColor = p_color; }

        //!< add all opaque objects to render them before transparent mesh.
        //!< The sub classes blend automatically opaque and transparent objects for depth test
        void appendOpaqueObject(const QString& p_objectName, AbstractRenderer* p_object); // NOT OWNER BUT NOT CONST FOR RENDER
        void removeOpaqueObject(const QString& p_objectName);
        inline void clearOpaqueObject(void) { m_opaqueRendererMap.clear(); }
        inline int opaqueObjectSize(void) const { return m_opaqueRendererMap.size(); }
        inline bool containsOpaqueObject(const QString& p_objectName) const { return m_opaqueRendererMap.contains(p_objectName); }

        //!< add all transparent objects to render them before transparent mesh.
        void appendTransparentObject(const QString& p_objectName, MeshRenderer* p_object); // NOT OWNER BUT NOT CONST FOR RENDER
        void removeTransparentObject(const QString& p_objectName);
        inline void clearTransparentObject(void) { m_transparencyRendererMap.clear(); }
        inline int opaqueTransparentSize(void) const { return m_transparencyRendererMap.size(); }
        inline bool containsTransparentObject(const QString& p_objectName) const { return m_transparencyRendererMap.contains(p_objectName); }

        inline void setAntiAliasing(bool p_enable) { m_antiAliasing = p_enable; }

        void render(void) final;

    protected:
        virtual void renderTransparentObjects(void) = 0;

        bool isOtherGlFunctionsInitialized(void) const override;
        bool updateOtherGlFunctions(void) override;
        bool initOtherGlFunctions(void) override;
        inline void deleteOtherGlFunctions(void) override { deleteFullScreenQuad(); }

        bool isRenderTargetsInitialized(void) const override;
        bool updateRenderTargets(int p_width, int p_height) final;
        virtual bool updateTransparentRenderTargets() = 0;
        bool initRenderTargets(int p_width, int p_height) final;
        virtual bool initTransparentRenderTargets() = 0;

        void deleteRenderTargets(void) override;

        static constexpr const char* quadVertex(void) { return "Shaders:UnorderedTransparency/quad_vertex.glsl"; }
        void initFullScreenQuad(void);
        void deleteFullScreenQuad(void);

        void drawFullScreenQuad(void); //!< bind texture in 2 screen triangles

        void bindTexture(QOpenGLShaderProgram& p_program, const QString& p_texname, GLuint p_texid, GLint p_texunit);
        void unbindTexture(GLint p_texunit);

        GLuint m_opaqueTexId;
        GLuint m_opaqueDepthTexId;

        int m_width;
        int m_height;

        QVector3D m_backgroundColor;

        QHash<QString, MeshRenderer*> m_transparencyRendererMap;

    private:
        GLuint m_quadVertexArrayId, m_quadPositionBufferId;

        QHash<QString, AbstractRenderer*> m_opaqueRendererMap;

        GLuint m_opaqueFramebufferFboId, m_opaqueRenderbufferFboId;

        GLuint m_opaqueDepthFramebufferId;

        bool m_antiAliasing;

        Q_DISABLE_COPY_MOVE(TransparencyRenderer);
    };

}
