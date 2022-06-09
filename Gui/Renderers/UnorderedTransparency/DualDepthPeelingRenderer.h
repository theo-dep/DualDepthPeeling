#pragma once

#include "Renderers/UnorderedTransparency/TransparencyRenderer.h"

#include <array>

namespace gui::gl
{

    /*
     * \class DualDepthPeelingRenderer
     * \brief Render a RMeshModel with transparency
     *
     * Class to make independent transparency with Dual Depth Peeling technique
     *
    */
    class DualDepthPeelingRenderer final : public TransparencyRenderer
    {
    public:
        explicit DualDepthPeelingRenderer(const Scene& p_scene, const Camera& p_camera);
        virtual ~DualDepthPeelingRenderer(void);

        //!< Automatically detect the number of needed passes to render a model (default true)
        inline void setOpenGlQueryEnable(bool p_isEnabled) { m_useOQ = p_isEnabled; }
        //!< Set a fixed number of passes, OpenGlQuery must be disabled
        inline void setNumberOfPasses(size_t p_number) { m_numberOfPasses = p_number; }

    protected:
        bool isOtherGlFunctionsInitialized(void) const override;
        bool updateOtherGlFunctions(void) override;
        bool initOtherGlFunctions(void) override;
        void deleteOtherGlFunctions(void) override;

        void renderTransparentObjects(void) override;

        bool isRenderTargetsInitialized(void) const override;
        bool updateTransparentRenderTargets() override;
        bool initTransparentRenderTargets() override;
        void deleteRenderTargets(void) override;

        inline bool isShadersInitialized(void) const override { return (m_shaderDualInit.isLinked() && m_shaderDualPeel.isLinked() && m_shaderDualBlend.isLinked() && m_shaderDualFinal.isLinked()); }
        bool initShaders(void) override;
        void deleteShaders(void) override;

    private:
        QOpenGLShaderProgram m_shaderDualInit;
        QOpenGLShaderProgram m_shaderDualPeel;
        QOpenGLShaderProgram m_shaderDualBlend;
        QOpenGLShaderProgram m_shaderDualFinal;

        bool m_useOQ;
        GLuint m_queryId;
        size_t m_numberOfPasses;

        //GLuint m_dualBackBlenderFboId;
        GLuint m_dualPeelingSingleFboId;
        GLuint m_dualBackBlenderTexId;
        std::array<GLuint, 2> m_dualDepthTexId;
        std::array<GLuint, 2> m_dualFrontBlenderTexId;
        std::array<GLuint, 2> m_dualBackTempTexId;

        static constexpr GLfloat MAX_DEPTH = 1.f;
        static constexpr std::array<GLenum, 7> DRAW_BUFFERS{
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3,
            GL_COLOR_ATTACHMENT4,
            GL_COLOR_ATTACHMENT5,
            GL_COLOR_ATTACHMENT6
        };

        Q_DISABLE_COPY_MOVE(DualDepthPeelingRenderer);
    };

}
