#include "Renderers/UnorderedTransparency/DualDepthPeelingRenderer.h"

#include "GLWidgets/Camera.h"
#include "GLWidgets/Scene.h"

#include <QtGui/QOpenGLFramebufferObject>
#include <QtCore/QDebug>

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    DualDepthPeelingRenderer::DualDepthPeelingRenderer(const Scene& p_scene, const Camera& p_camera) : TransparencyRenderer(p_scene, p_camera)
        , m_useOQ(true)
        , m_queryId(0)
        , m_numberOfPasses(4)
        //, m_dualBackBlenderFboId(0)
        , m_dualPeelingSingleFboId(0)
        , m_dualBackBlenderTexId(0)
        , m_dualDepthTexId{ {0, 0} }
        , m_dualFrontBlenderTexId{ {0, 0} }
        , m_dualBackTempTexId{ {0, 0} }
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    DualDepthPeelingRenderer::~DualDepthPeelingRenderer(void)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::isOtherGlFunctionsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        if (!TransparencyRenderer::isOtherGlFunctionsInitialized())
        {
            return false;
        }

        return (m_queryId != 0u);
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::updateOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        // do not call glGenQueries
        return TransparencyRenderer::updateOtherGlFunctions();
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::initOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        if (!TransparencyRenderer::initOtherGlFunctions())
        {
            return false;
        }

        glGenQueries(1, &m_queryId);

        return true;
    }

    //---------------------------------------------------------------------------------------
    void DualDepthPeelingRenderer::deleteOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        TransparencyRenderer::deleteOtherGlFunctions();

        glDeleteQueries(1, &m_queryId);
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::isRenderTargetsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        if (!TransparencyRenderer::isRenderTargetsInitialized())
        {
            return false;
        }

        bool isInitialized{ m_dualPeelingSingleFboId != 0u && m_dualBackBlenderTexId != 0u };
        for (size_t i = 0; i < 2 && isInitialized; i++)
        {
            isInitialized = isInitialized && m_dualDepthTexId.at(i) != 0u;
            isInitialized = isInitialized && m_dualFrontBlenderTexId.at(i) != 0u;
            isInitialized = isInitialized && m_dualBackTempTexId.at(i) != 0u;
        }
        return isInitialized;
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::updateTransparentRenderTargets()
    //---------------------------------------------------------------------------------------
    {
        for (size_t i = 0; i < 2; i++)
        {
            glBindTexture(USING_GL_TEXTURE, m_dualDepthTexId.at(i));
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RG32F, m_width, m_height, 0, GL_RG, GL_FLOAT, nullptr);

            glBindTexture(USING_GL_TEXTURE, m_dualFrontBlenderTexId.at(i));
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

            glBindTexture(USING_GL_TEXTURE, m_dualBackTempTexId.at(i));
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
        }

        glBindTexture(USING_GL_TEXTURE, m_dualBackBlenderTexId);
        glTexImage2D(USING_GL_TEXTURE, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);

        return true;
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::initTransparentRenderTargets()
    //---------------------------------------------------------------------------------------
    {
        glGenTextures(2, m_dualDepthTexId.data());
        glGenTextures(2, m_dualFrontBlenderTexId.data());
        glGenTextures(2, m_dualBackTempTexId.data());
        glGenFramebuffers(1, &m_dualPeelingSingleFboId);

        for (size_t i = 0; i < 2; i++)
        {
            glBindTexture(USING_GL_TEXTURE, m_dualDepthTexId.at(i));
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RG32F, m_width, m_height, 0, GL_RG, GL_FLOAT, nullptr);

            glBindTexture(USING_GL_TEXTURE, m_dualFrontBlenderTexId.at(i));
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

            glBindTexture(USING_GL_TEXTURE, m_dualBackTempTexId.at(i));
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
        }

        glGenTextures(1, &m_dualBackBlenderTexId);
        glBindTexture(USING_GL_TEXTURE, m_dualBackBlenderTexId);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(USING_GL_TEXTURE, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);

        //glGenFramebuffers(1, &m_dualBackBlenderFboId);
        //glBindFramebuffer(GL_FRAMEBUFFER, m_dualBackBlenderFboId);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, USING_GL_TEXTURE, m_dualBackBlenderTexId, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, m_dualPeelingSingleFboId);

        size_t j = 0;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, USING_GL_TEXTURE, m_dualDepthTexId.at(j), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, USING_GL_TEXTURE, m_dualFrontBlenderTexId.at(j), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, USING_GL_TEXTURE, m_dualBackTempTexId.at(j), 0);

        j = 1;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, USING_GL_TEXTURE, m_dualDepthTexId.at(j), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, USING_GL_TEXTURE, m_dualFrontBlenderTexId.at(j), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, USING_GL_TEXTURE, m_dualBackTempTexId.at(j), 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, USING_GL_TEXTURE, m_dualBackBlenderTexId, 0);

        return true;
    }

    //---------------------------------------------------------------------------------------
    void DualDepthPeelingRenderer::deleteRenderTargets(void)
    //---------------------------------------------------------------------------------------
    {
        TransparencyRenderer::deleteRenderTargets();

        //glDeleteFramebuffers(1, &m_dualBackBlenderFboId);
        glDeleteFramebuffers(1, &m_dualPeelingSingleFboId);
        glDeleteTextures(1, &m_dualBackBlenderTexId);
        glDeleteTextures(2, m_dualDepthTexId.data());
        glDeleteTextures(2, m_dualFrontBlenderTexId.data());
        glDeleteTextures(2, m_dualBackTempTexId.data());
    }

    //---------------------------------------------------------------------------------------
    bool DualDepthPeelingRenderer::initShaders(void)
    //---------------------------------------------------------------------------------------
    {
        bool isOk{ loadShaders(m_shaderDualInit,
            { "Shaders:UnorderedTransparency/init_vertex.glsl" },
            { "Shaders:UnorderedTransparency/init_fragment.glsl" })
        };

        isOk &= loadShaders(m_shaderDualPeel,
            { MultipleLightsRenderer::shadeVertex(), "Shaders:UnorderedTransparency/peel_vertex.glsl" },
            { MultipleLightsRenderer::shadeFragment(), "Shaders:UnorderedTransparency/peel_fragment.glsl" });

        isOk &= loadShaders(m_shaderDualBlend, { quadVertex() }, { "Shaders:UnorderedTransparency/blend_fragment.glsl" });

        isOk &= loadShaders(m_shaderDualFinal, { quadVertex() }, { "Shaders:UnorderedTransparency/final_fragment.glsl" });

        return isOk;
    }

    //---------------------------------------------------------------------------------------
    void DualDepthPeelingRenderer::deleteShaders(void)
    //---------------------------------------------------------------------------------------
    {
        m_shaderDualInit.removeAllShaders();
        m_shaderDualPeel.removeAllShaders();
        m_shaderDualBlend.removeAllShaders();
        m_shaderDualFinal.removeAllShaders();
    }

    //---------------------------------------------------------------------------------------
    void DualDepthPeelingRenderer::renderTransparentObjects(void)
    //---------------------------------------------------------------------------------------
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        // ---------------------------------------------------------------------
        // 1. Initialize Min-Max Depth Buffer
        // ---------------------------------------------------------------------

        glBindFramebuffer(GL_FRAMEBUFFER, m_dualPeelingSingleFboId);

        // Render targets 1 and 2 store the front and back colors
        // Clear to 0.0 and use MAX blending to filter written color
        // At most one front color and one back color can be written every pass
        glDrawBuffers(2, &DRAW_BUFFERS[1]);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render target 0 stores (-minDepth, maxDepth, alphaMultiplier)
        glDrawBuffer(DRAW_BUFFERS[0]);
        glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendEquation(GL_MAX);

        for (MeshRenderer* const renderer : m_transparencyRendererMap)
        {
            renderer->renderMesh(m_shaderDualInit, false);
        }

        // ---------------------------------------------------------------------
        // 2. Dual Depth Peeling + Blending
        // ---------------------------------------------------------------------

        // Since we cannot blend the back colors in the geometry passes,
        // we use another render target to do the alpha blending
        //glBindFramebuffer(GL_FRAMEBUFFER, g_dualBackBlenderFboId);
        glDrawBuffer(DRAW_BUFFERS[6]);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        size_t currId{ 0 };
        GLuint sampleCount{ 1u };
        for (size_t pass = 1; (m_useOQ && sampleCount > 0u) || (!m_useOQ && pass < m_numberOfPasses); pass++)
        {
            currId = pass % 2;
            const size_t prevId{ 1 - currId };
            const size_t bufId{ currId * 3 };

            //glBindFramebuffer(GL_FRAMEBUFFER, m_dualPeelingFboId[currId]);

            glDrawBuffers(2, &DRAW_BUFFERS.at(bufId + 1));
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawBuffer(DRAW_BUFFERS.at(bufId + 0));
            glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            // Render target 0: RG32F MAX blending
            // Render target 1: RGBA MAX blending
            // Render target 2: RGBA MAX blending
            glDrawBuffers(3, &DRAW_BUFFERS.at(bufId + 0));
            glBlendEquation(GL_MAX);

            for (MeshRenderer* const renderer : m_transparencyRendererMap)
            {
                renderer->renderMesh(m_shaderDualPeel, true,
                    [this, &prevId]()
                    {
                        bindTexture(m_shaderDualPeel, "DepthBlenderTex", m_dualDepthTexId.at(prevId), 0);
                        bindTexture(m_shaderDualPeel, "FrontBlenderTex", m_dualFrontBlenderTexId.at(prevId), 1);
                        bindTexture(m_shaderDualPeel, "OpaqueDepthTex", m_opaqueDepthTexId, 2);
                        bindTexture(m_shaderDualPeel, "OpaqueColorTex", m_opaqueTexId, 3);
                        //m_shaderDualPeel.setUniformValue("Width", static_cast<GLfloat>(m_width));
                        //m_shaderDualPeel.setUniformValue("Height", static_cast<GLfloat>(m_height));
                    },
                    [this]()
                    {
                        unbindTexture(0);
                        unbindTexture(1);
                        unbindTexture(2);
                        unbindTexture(3);
                    });
            }

            // Full screen pass to alpha-blend the back color
            glDrawBuffer(DRAW_BUFFERS[6]);

            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (m_useOQ)
            {
                glBeginQuery(GL_SAMPLES_PASSED, m_queryId);
            }

            m_shaderDualBlend.bind();
            bindTexture(m_shaderDualBlend, "TempTex", m_dualBackTempTexId.at(currId), 0);
            drawFullScreenQuad();
            unbindTexture(0);
            m_shaderDualBlend.release();

            if (m_useOQ)
            {
                glEndQuery(GL_SAMPLES_PASSED);
                glGetQueryObjectuiv(m_queryId, GL_QUERY_RESULT, &sampleCount);
            }
        }

        glDisable(GL_BLEND);

        // ---------------------------------------------------------------------
        // 3. Final Pass
        // ---------------------------------------------------------------------

        QOpenGLFramebufferObject::bindDefault();

        m_shaderDualFinal.bind();
        bindTexture(m_shaderDualFinal, "OpaqueTex", m_opaqueTexId, 0);
        bindTexture(m_shaderDualFinal, "FrontBlenderTex", m_dualFrontBlenderTexId.at(currId), 1);
        bindTexture(m_shaderDualFinal, "BackBlenderTex", m_dualBackBlenderTexId, 2);
        drawFullScreenQuad();
        unbindTexture(0);
        unbindTexture(1);
        unbindTexture(2);
        m_shaderDualFinal.release();

        glEnable(GL_DEPTH_TEST);
    }

}
