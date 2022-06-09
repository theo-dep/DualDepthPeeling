#include "Renderers/Debug/AutoShaderReloader.h"

#if defined(_DEBUG) && defined(AUTO_SHADER)
#include "Renderers/AbstractRenderer.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>

#define _XQUOTE(...) #__VA_ARGS__
#define _QUOTE(...) _XQUOTE(__VA_ARGS__)
#define _XCONC(A, B) A##B
#define _CONC(A, B) _XCONC(A, B)
#define STR_PROJECT_DIR _CONC(R,_QUOTE((PROJECT_DIR)))

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    const QString& AutoShaderReloader::shaderPath()
    //---------------------------------------------------------------------------------------
    {
        static const QString path(QDir::fromNativeSeparators(STR_PROJECT_DIR) + "/Shaders");
        qDebug() << "Shader path: " << path;
        return path;
    }

    //---------------------------------------------------------------------------------------
    AutoShaderReloader::AutoShaderReloader(AbstractRenderer& p_renderer) : QObject(nullptr)
        , m_fileWatcher(this)
        , m_renderer(p_renderer)
    //---------------------------------------------------------------------------------------
    {
        connect(&m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &AutoShaderReloader::slotReloadShader);
    }

    //---------------------------------------------------------------------------------------
    void AutoShaderReloader::addToWatcher(const QString& p_shaderFile)
    //---------------------------------------------------------------------------------------
    {
        const QStringList watchedFileList{ m_fileWatcher.files() };
        if (!watchedFileList.contains(p_shaderFile) && !m_fileWatcher.addPath(p_shaderFile))
        {
            qCritical() << "Fail to add " << p_shaderFile << " to file watcher";
            return;
        }        
    }

    //---------------------------------------------------------------------------------------
    void AutoShaderReloader::slotReloadShader(const QString& p_path)
    //---------------------------------------------------------------------------------------
    {
        qDebug() << "Reload for " << p_path;
        m_renderer.requestUpdateShaders();
    }

}

#endif
