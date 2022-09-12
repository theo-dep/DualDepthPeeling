#include "GLWidgets/MainWidget.h"

#include <QtGui/QGuiApplication>
#include <QtCore/QThread>

namespace
{
    static constexpr const char* meshName() { return "dragon"; }
}

//---------------------------------------------------------------------------------------
MainWidget::MainWidget(QWidget *parent) : GLWidget(parent)
//---------------------------------------------------------------------------------------
    , m_meshRenderer(nullptr)
    , m_transparencyRenderer(m_scene, m_camera)
#ifdef _DEBUG
    , m_logger(this)
#endif
{
    m_camera.setZoom(1.);

    static const QColor skyColor(44, 183, 185);
    m_transparencyRenderer.setBackgroundColor(QVector3D{ skyColor.redF(), skyColor.greenF(), skyColor.blueF() });
}

//---------------------------------------------------------------------------------------
MainWidget::~MainWidget()
//---------------------------------------------------------------------------------------
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();

    m_meshRenderer->cleanup();
    delete m_meshRenderer;

    m_transparencyRenderer.cleanup();

#ifdef _DEBUG
    m_logger.stopLogging();
#endif

    doneCurrent();
}

#ifdef _DEBUG
//---------------------------------------------------------------------------------------
void MainWidget::logOpenGLError(const QOpenGLDebugMessage & debugMessage)
//---------------------------------------------------------------------------------------
{
    if (debugMessage.severity() == QOpenGLDebugMessage::NotificationSeverity && debugMessage.type() == QOpenGLDebugMessage::OtherType)
    {
        qCritical() << "openGLError: " << debugMessage.message();
    }
    else
    {
        qInfo() << "openGLError: " << debugMessage.message();
    }
}
#endif

//---------------------------------------------------------------------------------------
void MainWidget::initializeGL()
//---------------------------------------------------------------------------------------
{
    GLWidget::initializeGL();

#ifdef _DEBUG
    bool log = m_logger.initialize();
    if (log)
    {
        connect(&m_logger, &QOpenGLDebugLogger::messageLogged, this, &MainWidget::logOpenGLError);
        m_logger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }
    else
    {
        qDebug() << "WARNING the openGL Debug is required but the extention GL_KHR_debug is not available on this system, the debug logger will not be activated.";
    }
#endif

    loadModel();
    m_transparencyRenderer.initialize(scaleToHighDpi(width()), scaleToHighDpi(height()));
}

//---------------------------------------------------------------------------------------
void MainWidget::loadModel()
//---------------------------------------------------------------------------------------
{
    if (m_transparencyRenderer.containsTransparentObject(::meshName()))
    {
        // already loaded
        return;
    }

    qInfo() << "loading model";
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    const QString resolvedPath(m_modelFilepath);
    m_model.loadObjPath(resolvedPath, false);
    QGuiApplication::restoreOverrideCursor();
    qInfo() << "done";

    m_meshRenderer = new gui::gl::MeshRenderer(m_model, m_scene, m_camera);

    static const QColor rustColor(100, 60, 20);
    static const QVector3D rust3DColor(rustColor.redF(), rustColor.greenF(), rustColor.blueF());
    m_meshRenderer->setMaterialAmbiantColor(rust3DColor);
    m_meshRenderer->setOpacity(0.5f);

    m_transparencyRenderer.appendTransparentObject(::meshName(), m_meshRenderer);

    geom::Point modelMin, modelMax;
    computeBoundingBox(modelMin, modelMax);
    const double diag{ modelMax.distance(modelMin) };
    const double scale{ (1. / diag) * 100. };
    m_camera.setScaling(static_cast<float>(scale));
    geom::Point tr{ (modelMin + geom::Vector{ modelMin, modelMax } * 0.5) };
    tr = tr.mul(-scale, -scale, -scale);
    m_camera.setTranslation(QVector3D(tr.x(), tr.y(), tr.z()));
}

//---------------------------------------------------------------------------------------
//componentwise min
inline geom::Point min(const geom::Point& lhs, const geom::Point& rhs)
{
    geom::Point rt;
    rt.x(std::min(lhs.x(), rhs.x()));
    rt.y(std::min(lhs.y(), rhs.y()));
    rt.z(std::min(lhs.z(), rhs.z()));
    return rt;
}
// componentwise max
inline geom::Point max(const geom::Point& lhs, const geom::Point& rhs)
{
    geom::Point rt;
    rt.x(std::max(lhs.x(), rhs.x()));
    rt.y(std::max(lhs.y(), rhs.y()));
    rt.z(std::max(lhs.z(), rhs.z()));
    return rt;
}
void MainWidget::computeBoundingBox(geom::Point& p_minVal, geom::Point& p_maxVal)
//---------------------------------------------------------------------------------------
{
    if (m_model.faceCount() == 0)
    {
        return;
    }

    p_minVal = geom::Point{ HUGE_VAL, HUGE_VAL, HUGE_VAL };
    p_maxVal = p_minVal.mul(-1., -1., -1.);

    for (int i = 0; i < m_model.vtxIndices().size(); i++)
    {
        const geom::Point& pt = m_model.vertices()[m_model.vtxIndices()[i]];
        p_minVal = min(p_minVal, pt);
        p_maxVal = max(p_maxVal, pt);
    }
}

//---------------------------------------------------------------------------------------
void MainWidget::resizeGL(int w, int h)
//---------------------------------------------------------------------------------------
{
    const int width{ scaleToHighDpi(w) };
    const int height{ scaleToHighDpi(h) };

    GLWidget::resizeGL(width, height);

    glViewport(0, 0, width, height);

    static constexpr float znear{ -1000.0f }, zfar{ 1000.0f };
    m_camera.configure({ 0.f, 0.f, static_cast<float>(width), static_cast<float>(height) }, znear, zfar);

    m_transparencyRenderer.setSize(width, height);
}

//---------------------------------------------------------------------------------------
void MainWidget::paintGL()
//---------------------------------------------------------------------------------------
{
    GLWidget::paintGL();

#ifdef AUTO_SHADER
    if (!m_transparencyRenderer.isInitialized())
    {
        m_transparencyRenderer.initialize(scaleToHighDpi(width()), scaleToHighDpi(height()));
    }
#endif

    m_transparencyRenderer.render();
}
