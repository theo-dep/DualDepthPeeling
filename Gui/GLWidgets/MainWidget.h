#pragma once

#include "GLWidgets/GLWidget.h"
#include "Renderers/MeshRenderer.h"
#include "Renderers/UnorderedTransparency/DualDepthPeelingRenderer.h"

#include <Mesh/MeshModel.h>

#ifdef _DEBUG
#include <QtGui/QOpenGLDebugLogger>
#endif

#include <array>

class MainWidget : public gui::GLWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    virtual ~MainWidget();

protected:
    void initializeGL() override;

    void loadModel();
    void computeBoundingBox(geom::Point& p_minVal, geom::Point& p_maxVal);

    void resizeGL(int w, int h) override;
    void paintGL() override;

    inline int scaleToHighDpi(int p_screenSize) const { return static_cast<int>(static_cast<qreal>(p_screenSize) * devicePixelRatioF()); }

protected slots:
#ifdef _DEBUG
    Q_SLOT void logOpenGLError(const QOpenGLDebugMessage & debugMessage);
#endif

protected:
    MeshModel m_model;
    static constexpr const char* MESH_NAME{ "dragon" };

    gui::gl::MeshRenderer* m_meshRenderer;
    gui::gl::DualDepthPeelingRenderer m_transparencyRenderer;

    static constexpr std::array<float, 3> WHITE{ 1.f, 1.f, 1.f };
    static constexpr std::array<float, 3> BLACK{ 0.f, 0.f, 0.f };

#ifdef _DEBUG
    QOpenGLDebugLogger m_logger;
#endif
};
