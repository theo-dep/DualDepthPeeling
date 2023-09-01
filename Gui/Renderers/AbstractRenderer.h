#pragma once

#if defined(_DEBUG) && defined(AUTO_SHADER)
#define DEBUG_AUTO_SHADER
#include "Renderers/Debug/AutoShaderReloader.h"
#endif

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>
#include <QtOpenGL/QOpenGLShaderProgram>
#else
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QOpenGLShaderProgram>
#endif

namespace gui
{
    class Scene;
    class Camera;
}

#ifndef Q_DISABLE_COPY_MOVE // until Qt 5.13
#define Q_DISABLE_COPY_MOVE(Class) \
Q_DISABLE_COPY(Class); \
Class(Class&&) noexcept Q_DECL_EQ_DELETE; \
Class& operator=(Class&&) noexcept Q_DECL_EQ_DELETE;
#endif

#ifdef _DEBUG
#include <QtCore/QDebug>
#define VERIFY_GL_CONTEXT if (QOpenGLContext::currentContext() == nullptr) qCritical() << "No current context, the GL memory cannot be released";
#else
#define VERIFY_GL_CONTEXT
#endif

#ifdef DEBUG_AUTO_SHADER
#define DECLARE_STATIC
#else
#define DECLARE_STATIC static
#endif

namespace gui::gl
{

    /**
     * Base class to render an OpenGL Object.
     * This class allows to pushRef and popRef for each rendering objects
     */
    class AbstractRenderer : protected QOpenGLFunctions_3_3_Core
    {
#ifdef DEBUG_AUTO_SHADER
        friend class AutoShaderReloader;
#endif

    public:
        AbstractRenderer(void) = delete;
        explicit AbstractRenderer(const Scene& p_scene, const Camera& p_camera);
        virtual ~AbstractRenderer(void) = 0;

        void cleanup(void); //!< Free GL memory, call this before deleting objets
        bool setSize(int p_width, int p_height); //!< Mandatory if the QOpenGlWidget parent is resizing
        bool initialize(int p_width, int p_height); //!< Mandatory before render()

        inline bool isInitialized(void) const { return (m_shaderInitialized && m_renderTargetsInitialized && m_otherGlFunctionsInitialized && isOtherGlFunctionsInitialized() && isRenderTargetsInitialized() && isShadersInitialized()); }

        virtual void render(void) = 0; //!< render GL textures

        //!< Call cleanup(), delete @a p_ptr and assign it to nullptr
        //! Use Macro below to populate @a p_ptrObject, @a p_file and @a p_line
        template <class Class>
        static inline void deleteRenderer(Class*& p_ptr, const char* p_ptrObject, const char* p_file, int p_line);
        //!< Loop on Container to call deleteRenderer
        template <typename Container>
        static inline void deleteAllRenderers(Container& p_container, const char* p_ptrObject, const char* p_file, int p_line);

    protected:
        inline void requestUpdateOtherGlFunctions(void) { m_otherGlFunctionsInitialized = false; } //!< must call initialize after
        virtual bool isOtherGlFunctionsInitialized(void) const = 0;
        virtual bool updateOtherGlFunctions(void) = 0;
        virtual bool initOtherGlFunctions(void) = 0;
        virtual void deleteOtherGlFunctions(void) = 0;

        inline void requestUpdateRenderTargets(void) { m_renderTargetsInitialized = false; } //!< must call initialize after
        virtual bool isRenderTargetsInitialized(void) const = 0;
        virtual bool updateRenderTargets(int p_width, int p_height) = 0;
        virtual bool initRenderTargets(int p_width, int p_height) = 0;
        virtual void deleteRenderTargets(void) = 0;

        inline void requestUpdateShaders(void) { m_shaderInitialized = false; } //!< must call initialize after
        virtual bool isShadersInitialized(void) const = 0;
        virtual bool initShaders(void) = 0;
        virtual void deleteShaders(void) = 0;

        DECLARE_STATIC bool addCacheableShaderFromSourceFileList(QOpenGLShaderProgram& p_program, QOpenGLShader::ShaderTypeBit p_shaderType, const QStringList& p_filepathList);
        //!< use this to load and link a QOpenGLShaderProgram with shaders list
        ///@{
        DECLARE_STATIC bool loadShaders(QOpenGLShaderProgram& p_program, const QStringList& p_vertexFilepathList, const QStringList& p_fragmentFilepathList);
        DECLARE_STATIC bool loadShaders(QOpenGLShaderProgram& p_program, const QStringList& p_vertexFilepathList, const QStringList& p_geometryFilepathList, const QStringList& p_fragmentFilepathList);
        ///@}

        const Scene& m_scene;
        const Camera& m_camera;

    private:
        bool m_shaderInitialized; //!< if false, reload the shaders
        bool m_renderTargetsInitialized; //!< if false, reload the render targets (specially for GL buffers)
        bool m_otherGlFunctionsInitialized; //!< if false, reload other GL functions
        bool m_isFullyInitialized; //!< true after the first load to avoid to reload all

#ifdef DEBUG_AUTO_SHADER
        AutoShaderReloader m_shaderReloader;
#endif

        Q_DISABLE_COPY_MOVE(AbstractRenderer);
    };

}

#define DELETE_GL_RENDERER(Ptr) gui::gl::AbstractRenderer::deleteRenderer(Ptr, #Ptr, __FILE__, __LINE__)
#define DELETE_ALL_GL_RENDERERS(Container) gui::gl::AbstractRenderer::deleteAllRenderers(Container, #Container, __FILE__, __LINE__)

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    template <class Class>
    inline void AbstractRenderer::deleteRenderer(Class*& p_ptr, const char* p_ptrObject, const char* p_file, int p_line)
    //---------------------------------------------------------------------------------------
    {
        if (p_ptr != nullptr)
        {
#ifdef _DEBUG
            if (QOpenGLContext::currentContext() == nullptr)
            {
                qCritical() << QString("No current context, the GL memory cannot be released (Object %0 in %1 (%2))").arg(p_ptrObject).arg(p_file).arg(p_line);
            }
#else
            Q_UNUSED(p_ptrObject);
            Q_UNUSED(p_file);
            Q_UNUSED(p_line);
#endif
            if constexpr (std::is_base_of_v<AbstractRenderer, Class>)
            {
                p_ptr->cleanup();
            }
            delete p_ptr;
            p_ptr = nullptr;
        }
    }

    //---------------------------------------------------------------------------------------
    template <typename Container>
    inline void AbstractRenderer::deleteAllRenderers(Container& p_container, const char* p_ptrObject, const char* p_file, int p_line)
    //---------------------------------------------------------------------------------------
    {
        for (typename Container::iterator it{ p_container.begin() }; it != p_container.end(); ++it)
        {
            deleteRenderer(*it, p_ptrObject, p_file, p_line);
        }
    }

}
