#include <GLWidgets/MainWidget.h>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
// https://bugreports.qt.io/browse/QTBUG-70206
#if defined(Q_OS_MACOS) && QT_VERSION <= QT_VERSION_CHECK(5, 12, 6)
    qputenv("QT_MAC_WANTS_LAYER", "1");
#endif

    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);// mandatory to shared GL context between all windows

#if defined(Q_OS_MACOS) && QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setSamples(8); // do not set value over 8, some computers do oversampling otherwise
    format.setProfile(QSurfaceFormat::CoreProfile);
#ifdef _DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWidget w;
    w.setModelFilepath(":/Model/dragon.obj");
    w.setMinimumSize(1024, 768);
    w.show();
    return a.exec();
}
