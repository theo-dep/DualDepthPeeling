#include "Renderers/UnorderedTransparency/TransparencyRenderer.h"

#include "GLWidgets/Scene.h"

#include <QtCore/QDebug>

#include <array>

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    TransparencyRenderer::TransparencyRenderer(const Scene& p_scene, const Camera& p_camera) : AbstractRenderer(p_scene, p_camera)
        , m_opaqueTexId(0)
        , m_opaqueDepthTexId(0)
        , m_width(0)
        , m_height(0)
        , m_backgroundColor(1.f, 1.f, 1.f)
        , m_quadVertexArrayId(0)
        , m_quadPositionBufferId(0)
        , m_opaqueFramebufferFboId(0)
        , m_opaqueRenderbufferFboId(0)
        , m_opaqueDepthFramebufferId(0)
        , m_antiAliasing(false)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    TransparencyRenderer::~TransparencyRenderer(void)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::appendOpaqueObject(const QString& p_objectName, AbstractRenderer* p_object)
    //---------------------------------------------------------------------------------------
    {
        if (p_object == nullptr)
        {
            qCritical() << "Cannot add a null renderer";
            return;
        }

        m_opaqueRendererMap.insert(p_objectName, p_object);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::removeOpaqueObject(const QString& p_objectName)
    //---------------------------------------------------------------------------------------
    {
        if (m_opaqueRendererMap.contains(p_objectName))
        {
            m_opaqueRendererMap.remove(p_objectName);
        }
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::appendTransparentObject(const QString& p_objectName, MeshRenderer* p_object)
    //---------------------------------------------------------------------------------------
    {
        if (p_object == nullptr)
        {
            qCritical() << "Cannot add a null renderer";
            return;
        }

        p_object->setIsClassicalRendering(false);
        m_transparencyRendererMap.insert(p_objectName, p_object);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::removeTransparentObject(const QString& p_objectName)
    //---------------------------------------------------------------------------------------
    {
        if (m_transparencyRendererMap.contains(p_objectName))
        {
            MeshRenderer* const object{ m_transparencyRendererMap.take(p_objectName) };
            object->setIsClassicalRendering(true);
        }
    }

    //---------------------------------------------------------------------------------------
    bool TransparencyRenderer::isOtherGlFunctionsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        return (m_quadVertexArrayId != 0u && m_quadPositionBufferId != 0u);
    }

    //---------------------------------------------------------------------------------------
    bool TransparencyRenderer::updateOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        // do not call initFullScreenQuad()
        return true;
    }

    //---------------------------------------------------------------------------------------
    bool TransparencyRenderer::initOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        initFullScreenQuad();

        return true;
    }

    //---------------------------------------------------------------------------------------
    bool TransparencyRenderer::isRenderTargetsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        return (m_opaqueTexId != 0u && m_opaqueFramebufferFboId != 0u && m_opaqueRenderbufferFboId != 0u && m_opaqueDepthTexId != 0u && m_opaqueDepthFramebufferId != 0u);
    }

    //---------------------------------------------------------------------------------------
    bool TransparencyRenderer::updateRenderTargets(int p_width, int p_height)
    //---------------------------------------------------------------------------------------
    {
#if 1
        // Faster
        deleteRenderTargets();
        return initRenderTargets(p_width, p_height);
#else
        if (!MeshRenderer::updateRenderTargets(p_width, p_height))
        {
            return false;
        }

        if (m_width != p_width || m_height != p_height)
        {
            m_width = p_width;
            m_height = p_height;

            glBindTexture(USING_GL_TEXTURE, m_opaqueTexId);
            glTexImage2D(USING_GL_TEXTURE, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

            glBindRenderbuffer(GL_RENDERBUFFER, m_opaqueRenderbufferFboId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_width, m_height);

            glBindTexture(USING_GL_TEXTURE, m_opaqueDepthTexId);
            glTexImage2D(USING_GL_TEXTURE, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

            return updateTransparentRenderTargets();
        }

        return true;
#endif
    }

    //---------------------------------------------------------------------------------------
    bool TransparencyRenderer::initRenderTargets(int p_width, int p_height)
    //---------------------------------------------------------------------------------------
    {
#ifdef _DEBUG
        if (m_antiAliasing)
        {
            GLint buffers(0), samples(0);
            glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
            glGetIntegerv(GL_SAMPLES, &samples);
            qDebug() << QString("MSAA: buffers = %0, samples = %1").arg(buffers).arg(samples);
        }
#endif

        glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

        m_width = p_width;
        m_height = p_height;

        // color tex
        glGenTextures(1, &m_opaqueTexId);

        glBindTexture(USING_GL_TEXTURE, m_opaqueTexId);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(USING_GL_TEXTURE, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

        glGenFramebuffers(1, &m_opaqueFramebufferFboId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_opaqueFramebufferFboId);

        // create depth buffer
        glGenRenderbuffers(1, &m_opaqueRenderbufferFboId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_opaqueRenderbufferFboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_opaqueRenderbufferFboId);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, USING_GL_TEXTURE, m_opaqueTexId, 0);

        // depth tex to compare with transparent tex
        glGenTextures(1, &m_opaqueDepthTexId);

        glBindTexture(USING_GL_TEXTURE, m_opaqueDepthTexId);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(USING_GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(USING_GL_TEXTURE, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        // framebuffer for depth texture
        glGenFramebuffers(1, &m_opaqueDepthFramebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_opaqueDepthFramebufferId);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, USING_GL_TEXTURE, m_opaqueDepthTexId, 0);

        return initTransparentRenderTargets();
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::deleteRenderTargets(void)
    //---------------------------------------------------------------------------------------
    {
        // do not call parent, this delete referentials
        //MeshRenderer::deleteRenderTargets();

        glDeleteTextures(1, &m_opaqueTexId);
        glDeleteFramebuffers(1, &m_opaqueFramebufferFboId);
        glDeleteRenderbuffers(1, &m_opaqueRenderbufferFboId);

        glDeleteTextures(1, &m_opaqueDepthTexId);
        glDeleteFramebuffers(1, &m_opaqueDepthFramebufferId);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::initFullScreenQuad(void)
    //---------------------------------------------------------------------------------------
    {
        // ID generation
        glGenVertexArrays(1, &m_quadVertexArrayId);
        glGenBuffers(1, &m_quadPositionBufferId);

        // lock vbo vao
        glBindVertexArray(m_quadVertexArrayId);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadPositionBufferId);

        constexpr std::array<GLfloat, 8> quadBufferData{
            -1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f
        };
        constexpr unsigned long long BUFFER_SIZE = sizeof quadBufferData;

        // allocate
        glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, quadBufferData.data(), GL_STATIC_DRAW);

        // data access
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // unlock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::deleteFullScreenQuad(void)
    //---------------------------------------------------------------------------------------
    {
        glDeleteVertexArrays(1, &m_quadVertexArrayId);
        glDeleteBuffers(1, &m_quadPositionBufferId);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::drawFullScreenQuad(void)
    //---------------------------------------------------------------------------------------
    {
        glBindVertexArray(m_quadVertexArrayId);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::bindTexture(QOpenGLShaderProgram& p_program, const QString& p_texname, GLuint p_texid, GLint p_texunit)
    //---------------------------------------------------------------------------------------
    {
        p_program.setUniformValue(p_texname.toStdString().c_str(), p_texunit);

        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + p_texunit));
        glBindTexture(USING_GL_TEXTURE, p_texid);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::unbindTexture(GLint p_texunit)
    //---------------------------------------------------------------------------------------
    {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + p_texunit));
        glBindTexture(USING_GL_TEXTURE, 0);
    }

    //---------------------------------------------------------------------------------------
    void TransparencyRenderer::render(void)
    //---------------------------------------------------------------------------------------
    {
        bool isRendererMapInitialized{ true };
        for (AbstractRenderer* const renderer : m_opaqueRendererMap.values())
        {
            if (renderer != nullptr && !renderer->isInitialized())
            {
                isRendererMapInitialized = isRendererMapInitialized && renderer->initialize(m_width, m_height); // reset
            }
        }
        for (MeshRenderer* const renderer : m_transparencyRendererMap.values())
        {
            if (renderer != nullptr && !renderer->isInitialized())
            {
                isRendererMapInitialized = isRendererMapInitialized && renderer->initialize(m_width, m_height); // reset
            }
        }

        if (!isInitialized() || !isRendererMapInitialized)
        {
            qCritical() << "Internal error: data not initialized";
            return;
        }

        // ---------------------------------------------------------------------
        // 0. Render Opaque Targets
        // ---------------------------------------------------------------------
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        if (m_antiAliasing)
        {
            glEnable(GL_MULTISAMPLE);
        }

        // Bug view on Intel UHD Graphics 630 (with no resize but using glTexSubImage2D)
        // Another work around is to delete and recreate the GL texture
        // https://community.intel.com/t5/Graphics/OpenGL-default-framebuffer-dimensions-not-updated-until-glClear/m-p/1195846
        // using glClear on the default framebuffer force the GPU buffer to an update
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, m_opaqueFramebufferFboId);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (AbstractRenderer* const renderer : m_opaqueRendererMap)
        {
            renderer->render();
        }

        // render to depth texture
        glBindFramebuffer(GL_FRAMEBUFFER, m_opaqueDepthFramebufferId);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (AbstractRenderer* const renderer : m_opaqueRendererMap)
        {
            renderer->render();
        }

        renderTransparentObjects();

        if (m_antiAliasing)
        {
            glDisable(GL_MULTISAMPLE);
        }
    }

}
