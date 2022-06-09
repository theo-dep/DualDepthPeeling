#include "Renderers/AbstractRenderer.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtGui/QOpenGLFramebufferObject>

//!< Q_INIT_RESOURCE does not work inside a namespace
static void initResources(void)
{
    static bool isInit(false);
    if (!isInit)
    {
#ifdef DEBUG_AUTO_SHADER
        static const QString shaderPath{ gui::gl::AutoShaderReloader::shaderPath() };
#else
        Q_INIT_RESOURCE(ShaderResources);
        static const QString shaderPath{ ":/Shaders/" };
#endif
        qDebug() << "Add shaders search path " << shaderPath;
        QDir::addSearchPath("Shaders", shaderPath);
        isInit = true;
    }
}

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    AbstractRenderer::AbstractRenderer(const Scene& p_scene, const Camera& p_camera) : QOpenGLFunctions_3_3_Core()
        , m_scene(p_scene)
        , m_camera(p_camera)
        , m_shaderInitialized(false)
        , m_renderTargetsInitialized(false)
        , m_otherGlFunctionsInitialized(false)
        , m_isFullyInitialized(false)
#ifdef DEBUG_AUTO_SHADER
        , m_shaderReloader(*this)
#endif
    //---------------------------------------------------------------------------------------
    {
        initResources();
    }

    //---------------------------------------------------------------------------------------
    AbstractRenderer::~AbstractRenderer(void)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    void AbstractRenderer::cleanup(void)
    //---------------------------------------------------------------------------------------
    {
        if (m_isFullyInitialized)
        {
            if (isRenderTargetsInitialized())
            {
                deleteRenderTargets();
            }
            if (isOtherGlFunctionsInitialized())
            {
                deleteOtherGlFunctions();
            }
            if (isShadersInitialized())
            {
                deleteShaders();
            }

            m_isFullyInitialized = false;
        }
    }

    //---------------------------------------------------------------------------------------
    bool AbstractRenderer::setSize(int p_width, int p_height)
    //---------------------------------------------------------------------------------------
    {
        requestUpdateRenderTargets();
        return initialize(p_width, p_height);
    }

    //---------------------------------------------------------------------------------------
    bool AbstractRenderer::initialize(int p_width, int p_height)
    //---------------------------------------------------------------------------------------
    {
        if (m_isFullyInitialized) // a first initialization has been done
        {
            if (!m_shaderInitialized)
            {
                deleteShaders();
                m_shaderInitialized = initShaders();
            }
            if (!m_renderTargetsInitialized)
            {
                m_renderTargetsInitialized = updateRenderTargets(p_width, p_height);
            }
            if (!m_otherGlFunctionsInitialized)
            {
                m_otherGlFunctionsInitialized = updateOtherGlFunctions();
            }

            return isInitialized();
        }

        //!< TODO TDE 2021-03-13
        //!< restore check when GL will be kept between view. Currently, GL is destroyed/recreated and the context is null between state transitions
        /*static QOpenGLContext* context = QOpenGLContext::currentContext();
        if (context == nullptr || !context->isValid())
        {
            GPS_ERR() << "No OpenGL context available";
            return false;
        }*/

        if (!QOpenGLFunctions_3_3_Core::isInitialized())
        {
            if (!initializeOpenGLFunctions())
            {
                qCritical() << "Fail to initialize OpenGL functions";
                return false;
            }
        }

        m_shaderInitialized = initShaders();
        m_renderTargetsInitialized = initRenderTargets(p_width, p_height);
        m_otherGlFunctionsInitialized = initOtherGlFunctions();
        if (!isInitialized())
        {
            qCritical() << "Cannot initialize shaders and render targets";
            return false;
        }
#ifdef _DEBUG
        const GLenum framebufferStatus{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
        if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE
#  ifdef Q_OS_MAC
                // On MacOS, the default framebuffer is targeted after transparency initialization
                && framebufferStatus != GL_FRAMEBUFFER_UNDEFINED
#  endif
           )
        {
            qCritical() << "Fail to bind buffers";
            return false;
        }
#endif

        QOpenGLFramebufferObject::bindDefault();

        m_isFullyInitialized = true;
        return true;
    }

    //---------------------------------------------------------------------------------------
    bool AbstractRenderer::addCacheableShaderFromSourceFileList(QOpenGLShaderProgram& p_program, QOpenGLShader::ShaderTypeBit p_shaderType, const QStringList& p_filepathList)
    //---------------------------------------------------------------------------------------
    {
        bool addOk(true);
        QStringList::const_iterator filepathIt = p_filepathList.cbegin();
        while (filepathIt != p_filepathList.cend() && addOk)
        {
            addOk = p_program.addCacheableShaderFromSourceFile(p_shaderType, *filepathIt);
#ifdef DEBUG_AUTO_SHADER
            m_shaderReloader.addToWatcher(*filepathIt);
#endif
            ++filepathIt;
        }
        return addOk;
    }

    //---------------------------------------------------------------------------------------
    bool AbstractRenderer::loadShaders(QOpenGLShaderProgram& p_program, const QStringList& p_vertexFilepathList, const QStringList& p_fragmentFilepathList)
    //---------------------------------------------------------------------------------------
    {
        bool addOk = addCacheableShaderFromSourceFileList(p_program, QOpenGLShader::Vertex, p_vertexFilepathList);
        addOk &= addCacheableShaderFromSourceFileList(p_program, QOpenGLShader::Fragment, p_fragmentFilepathList);
        addOk &= p_program.link();

        if (!addOk)
        {
            qCritical() << p_program.log();
            return false;
        }

        return true;
    }

    //---------------------------------------------------------------------------------------
    bool AbstractRenderer::loadShaders(QOpenGLShaderProgram& p_program, const QStringList& p_vertexFilepathList, const QStringList& p_geometryFilepathList, const QStringList& p_fragmentFilepathList)
    //---------------------------------------------------------------------------------------
    {
        const bool addOk = addCacheableShaderFromSourceFileList(p_program, QOpenGLShader::Geometry, p_geometryFilepathList);
        if (!addOk)
        {
            qCritical() << p_program.log();
            return false;
        }

        return loadShaders(p_program, p_vertexFilepathList, p_fragmentFilepathList);
    }

}
