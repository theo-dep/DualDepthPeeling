#include "Renderers/PlaneRenderer.h"

#include "Renderers/PathRenderer.h"

#include "GLWidgets/Camera.h"
#include "GLWidgets/Scene.h"

#include <Geom/Point.h>

#include <QtCore/QDebug>

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    PlaneRenderer::PlaneRenderer(const Scene& p_scene, const Camera& p_camera) : AbstractRenderer(p_scene, p_camera)
    //---------------------------------------------------------------------------------------
        , m_planeVertexArrayId{ 0u }
        , m_planePositionBufferId{ 0u }
        , m_plane{}
        , m_pathRendererList{}
        , m_color{}
        , m_opacity{ 1.f }
        , m_planeWidth{ 50.f }
        , m_planeShift{ 0.f }
        , m_shaderPlane{}
    {
        m_pathRendererList.fill(nullptr);
    }

    //---------------------------------------------------------------------------------------
    PlaneRenderer::~PlaneRenderer(void)
    //---------------------------------------------------------------------------------------
    {
        clear();
    }

    //---------------------------------------------------------------------------------------
    void PlaneRenderer::clear(void)
    //---------------------------------------------------------------------------------------
    {
        qDeleteAll(m_pathRendererList);
        m_pathRendererList.fill(nullptr);
    }

    //---------------------------------------------------------------------------------------
    void PlaneRenderer::setPlane(const geom::Plane& p_plane, const QVector3D& p_color)
    //---------------------------------------------------------------------------------------
    {
        m_plane = p_plane;
        m_color = p_color;

        {
            geom::Vector i, j;
            p_plane.getBase(i, j);

            const geom::Point planeOrigin{ p_plane.origin() + m_planeShift * j };

            const geom::Point pt1{ planeOrigin + (-m_planeWidth) * i + (-m_planeWidth) * j };
            const geom::Point pt2{ planeOrigin + (-m_planeWidth) * i +   m_planeWidth  * j };
            const geom::Point pt3{ planeOrigin +   m_planeWidth  * i +   m_planeWidth  * j };
            const geom::Point pt4{ planeOrigin +   m_planeWidth  * i + (-m_planeWidth) * j };

            const std::array<geom::Point, SQUARE_EDGE> squareList{ pt1, pt2, pt3, pt4 };

            /*                       /
                 p a t h 0          / i
               p 0 ------ 1 p       \ j
               a |        | a        \
               t |        | t
               h |        | h
               3 3 ------ 2 1
                 p a t h 2
            */
            for (size_t k{ 0u }; k < SQUARE_EDGE; k++)
            {
                PathRenderer*& pathRenderer{ m_pathRendererList.at(k) };
                DELETE_GL_RENDERER(pathRenderer);
                pathRenderer = new PathRenderer(squareList.at(k), squareList.at((k + 1u) % SQUARE_EDGE), m_color, m_scene, m_camera);
            }

            m_planeBuffer = {
                static_cast<GLfloat>(pt1.x()), static_cast<GLfloat>(pt1.y()), static_cast<GLfloat>(pt1.z()),
                static_cast<GLfloat>(pt2.x()), static_cast<GLfloat>(pt2.y()), static_cast<GLfloat>(pt2.z()),
                static_cast<GLfloat>(pt3.x()), static_cast<GLfloat>(pt3.y()), static_cast<GLfloat>(pt3.z()),
                static_cast<GLfloat>(pt4.x()), static_cast<GLfloat>(pt4.y()), static_cast<GLfloat>(pt4.z())
            };
        }

        requestUpdateRenderTargets();
        requestUpdateOtherGlFunctions();
    }

    //---------------------------------------------------------------------------------------
    void PlaneRenderer::setThickness(GLfloat p_thickness)
    //---------------------------------------------------------------------------------------
    {
        for (PathRenderer* const renderer : m_pathRendererList)
        {
            if (renderer != nullptr)
            {
                renderer->setThickness(p_thickness);
            }
        }
    }

    //---------------------------------------------------------------------------------------
    bool PlaneRenderer::initShaders(void)
    //---------------------------------------------------------------------------------------
    {
        const bool res{ loadShaders(m_shaderPlane, { "Shaders:color_vertex.glsl" }, { "Shaders:color_fragment.glsl" }) };
        if (!res) qCritical() << "Fail to initialize shaders";
        return res;
    }

    //---------------------------------------------------------------------------------------
    void PlaneRenderer::deleteShaders(void)
    //---------------------------------------------------------------------------------------
    {
        m_shaderPlane.removeAllShaders();
    }

    //---------------------------------------------------------------------------------------
    bool PlaneRenderer::isRenderTargetsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        bool isInitialized{ m_planeVertexArrayId != 0u && m_planePositionBufferId != 0u };
        for (const PathRenderer* const renderer : m_pathRendererList)
        {
            if (renderer != nullptr)
            {
                isInitialized &= renderer->isInitialized();
            }
        }
        return isInitialized;
    }

    //---------------------------------------------------------------------------------------
    bool PlaneRenderer::updateRenderTargets(int p_width, int p_height)
    //---------------------------------------------------------------------------------------
    {
        // lock vbo
        glBindBuffer(GL_ARRAY_BUFFER, m_planePositionBufferId);

        // fill memory
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof m_planeBuffer, m_planeBuffer.data());

        // unlock vbo
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        bool isInitialized{ true };
        for (PathRenderer* const renderer : m_pathRendererList)
        {
            if (renderer != nullptr && !renderer->isInitialized())
            {
                isInitialized &= renderer->initialize(p_width, p_height);
            }
        }
        return isInitialized;
    }

    //---------------------------------------------------------------------------------------
    bool PlaneRenderer::initRenderTargets(int p_width, int p_height)
    //---------------------------------------------------------------------------------------
    {
        // init vbo vao
        glGenBuffers(1, &m_planePositionBufferId);
        glGenVertexArrays(1, &m_planeVertexArrayId);

        // lock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, m_planePositionBufferId);
        glBindVertexArray(m_planeVertexArrayId);

        // allocate memory
        glBufferData(GL_ARRAY_BUFFER, sizeof m_planeBuffer, m_planeBuffer.data(), GL_STATIC_DRAW);

        // data access
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // unlock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        bool isInitialized{ true };
        for (PathRenderer* const renderer : m_pathRendererList)
        {
            if (renderer != nullptr && !renderer->isInitialized())
            {
                isInitialized &= renderer->initialize(p_width, p_height);
            }
        }
        return isInitialized;
    }

    //---------------------------------------------------------------------------------------
    void PlaneRenderer::deleteRenderTargets(void)
    //---------------------------------------------------------------------------------------
    {
        glDeleteVertexArrays(1, &m_planeVertexArrayId);
        glDeleteBuffers(1, &m_planePositionBufferId);

        DELETE_ALL_GL_RENDERERS(m_pathRendererList);
        m_pathRendererList.fill(nullptr);
    }

    //---------------------------------------------------------------------------------------
    void PlaneRenderer::render(void)
    //---------------------------------------------------------------------------------------
    {
        if (!isInitialized())
        {
            qCritical() << "Internal error: data not initialized";
            return;
        }

        if (m_opacity < 1.f)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        // draw plane
        if (m_shaderPlane.bind())
        {
            m_shaderPlane.setUniformValue("color", QVector4D(m_color, m_opacity));
            m_shaderPlane.setUniformValue("MVPMatrix", m_camera.projMatrix() * m_camera.viewMatrix() * m_scene.modelMatrix());

            // lock vao
            glBindVertexArray(m_planeVertexArrayId);
            // draw plane in quad
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            // unlock vao
            glBindVertexArray(0);

            m_shaderPlane.release();
        }

        if (m_opacity < 1.f)
        {
            glDisable(GL_BLEND);
        }

        // draw opaque square around
        for (PathRenderer* const renderer : m_pathRendererList)
        {
            if (renderer != nullptr && renderer->isInitialized())
            {
                renderer->render();
            }
        }
    }

}

