#pragma once

#include "Renderers/AbstractRenderer.h"
#include "Renderers/Common/MultipleLightsRenderer.h"

class MeshModel;

namespace gui::gl
{
    /**
     * Class to render a GL mesh (RMeshModel) with multiple lights and clip distance shaders.
     * This class allows multiple mesh to render all in single pass.
     * Each mesh can have its own referential.
     */
    class MeshRenderer : public AbstractRenderer, public MultipleLightsRenderer
    {
    public:
        //!< default constructor
        explicit MeshRenderer(const MeshModel& p_mesh, const Scene& p_scene, const Camera& p_camera);
        virtual ~MeshRenderer(void) override;

        void render(void) override;

        /**
         * \brief Render the full mesh list with options
         * \param p_program the shader program to render the mesh list
         * \param p_withLightColorShader render mesh with lights (disable if user need only depth)
         * \param p_beforeRenderMeshFunc other parameters to pass to p_program in a (lambda) function before rendering a mesh (ex a texture binding)
         * \param p_afterRenderMeshFunc other parameters to pass to p_program in a (lambda) function after rendering a mesh (ex a texture unbinding)
         */
        void renderMesh(QOpenGLShaderProgram& p_program, bool p_withLightColorShader, std::function<void()> p_beforeRenderMeshFunc = []() {}, std::function<void()> p_afterRenderMeshFunc = []() {});

        inline void setIsClassicalRendering(bool p_rendering) { m_isClassicalRendering = p_rendering; }
        inline bool isClassicalRendering(void) const { return m_isClassicalRendering; } //!< If true, enable GL_BLEND when opacity is different from one

    protected:
        bool isOtherGlFunctionsInitialized(void) const override;
        bool updateOtherGlFunctions(void) override;
        bool initOtherGlFunctions(void) override;
        inline void deleteOtherGlFunctions(void) override;

        inline bool isRenderTargetsInitialized(void) const override { return true; }
        inline bool updateRenderTargets(int, int) override { return true; }
        inline bool initRenderTargets(int, int) override { return true; }
        inline void deleteRenderTargets(void) override {}

        inline bool isShadersInitialized(void) const override { return m_shaderMultipleLights.isLinked(); }
        bool initShaders(void) override;
        void deleteShaders(void) override;

        virtual inline bool defaultAmbiantLightUser(void) const { return true; }
        virtual inline bool defaultMaterialOn(void) const { return true; }
        virtual inline QVector3D defaultMaterialAmbiantColor(void) const { return QVector3D(0.35f, 0.35f, 0.35f); }
        virtual inline QVector3D defaultMaterialDiffuseColor(void) const { return QVector3D(0.8f, 0.8f, 0.8f); }
        virtual inline QVector3D defaultMaterialSpecularColor(void) const { return QVector3D(0.0f, 0.0f, 0.0f); }

    private:
        QOpenGLShaderProgram m_shaderMultipleLights;

        bool m_isClassicalRendering;

        const MeshModel& m_mesh;

        GLuint m_vaoID; // array object: data access
        GLuint m_vboID; // buffer object: data

        Q_DISABLE_COPY_MOVE(MeshRenderer);
    };

}
