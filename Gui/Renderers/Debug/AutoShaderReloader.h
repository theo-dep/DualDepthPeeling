#pragma once

#ifndef _DEBUG
#ifdef AUTO_SHADER
#error This class must be used only in debug mode
#endif
#endif

#include <QtCore/QFileSystemWatcher>

namespace gui::gl
{
    class AbstractRenderer;

    /**
     * Debug class to auto reload a shader (to avoid recompilation at each glsl update).
     */
    class AutoShaderReloader : public QObject
    {
        Q_OBJECT

    public:
        AutoShaderReloader(AbstractRenderer& p_renderer);

        void addToWatcher(const QString& p_shaderFile);

        static const QString& shaderPath();

    protected:
        Q_SLOT void slotReloadShader(const QString& p_path);

        QFileSystemWatcher m_fileWatcher;
        AbstractRenderer& m_renderer;
    };

}
