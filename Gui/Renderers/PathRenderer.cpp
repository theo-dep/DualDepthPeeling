#include "Renderers/PathRenderer.h"

#include "GLWidgets/Camera.h"
#include "GLWidgets/Scene.h"

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    PathRenderer::PathRenderer(const QList<geom::Point>& p_pointContainer, const QVector3D& p_color, const Scene& p_scene, const Camera& p_camera, bool p_renderWithStrip) : AbstractRenderer(p_scene, p_camera)
    //---------------------------------------------------------------------------------------
        , m_vaoID(0)
        , m_vboID(0)
        , m_thickness(2.f)
        , m_renderWithStrip(p_renderWithStrip)
    {
        init(p_pointContainer, p_color);
    }

    //---------------------------------------------------------------------------------------
    PathRenderer::PathRenderer(const geom::Point& p_point1, const geom::Point& p_point2, const QVector3D& p_color, const Scene& p_scene, const Camera& p_camera, bool p_renderWithStrip) : AbstractRenderer(p_scene, p_camera)
    //---------------------------------------------------------------------------------------
        , m_vaoID(0)
        , m_vboID(0)
        , m_thickness(2.f)
        , m_renderWithStrip(p_renderWithStrip)
    {
        init(p_point1, p_point2, p_color);
    }

    //---------------------------------------------------------------------------------------
    PathRenderer::~PathRenderer(void)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    void PathRenderer::init(const QList<geom::Point>& p_pointContainer, const QVector4D& p_color)
    //---------------------------------------------------------------------------------------
    {
        const int containerSize{ p_pointContainer.size() };
        m_vertexObject.resize(6 * containerSize);
        m_vertexObject.squeeze();
        m_colors.resize(2 * containerSize);
        m_colors.squeeze();

        int vertexIndex{ -1 }, colorIndex{ -1 };
        for (const geom::Point& point : p_pointContainer)
        {
            m_vertexObject[++vertexIndex] = point.x();
            m_vertexObject[++vertexIndex] = point.y();
            m_vertexObject[++vertexIndex] = point.z();

            m_colors[++colorIndex] = p_color;

            m_vertexObject[++vertexIndex] = point.x();
            m_vertexObject[++vertexIndex] = point.y();
            m_vertexObject[++vertexIndex] = point.z();

            m_colors[++colorIndex] = p_color;
        }
    }

    //---------------------------------------------------------------------------------------
    void PathRenderer::init(const geom::Point& p_point1, const geom::Point& p_point2, const QVector4D& p_color)
    //---------------------------------------------------------------------------------------
    {
        init({ p_point1, p_point2 }, p_color);
    }

    //---------------------------------------------------------------------------------------
    bool PathRenderer::isOtherGlFunctionsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        return (m_vboID != 0 && m_vaoID != 0);
    }

    //---------------------------------------------------------------------------------------
    bool PathRenderer::updateOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        const GLintptr vertexSizeBytes = m_vertexObject.size() * sizeof(float);
        const GLintptr colorsSizeBytes = m_colors.size() * sizeof(QVector4D);
        if (vertexSizeBytes == 0 || colorsSizeBytes == 0)
        {
            qCritical() << "internal error, empty path" ;
            return false;
        }

        // lock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBindVertexArray(m_vaoID);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSizeBytes, vertices());
        glBufferSubData(GL_ARRAY_BUFFER, vertexSizeBytes, colorsSizeBytes, colors());

        // data access
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(vertexSizeBytes));

        // unlock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return true;
    }

    //---------------------------------------------------------------------------------------
    bool PathRenderer::initOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        // ID generation
        glGenBuffers(1, &m_vboID);
        glGenVertexArrays(1, &m_vaoID);

        // lock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBindVertexArray(m_vaoID);

        const GLintptr vertexSizeBytes = m_vertexObject.size() * sizeof(float);
        const GLintptr colorsSizeBytes = m_colors.size() * sizeof(QVector4D);

        glBufferData(GL_ARRAY_BUFFER, vertexSizeBytes + colorsSizeBytes, nullptr, GL_STATIC_DRAW);

        // unlock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return updateOtherGlFunctions();
    }

    //---------------------------------------------------------------------------------------
    void PathRenderer::deleteOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        glDeleteBuffers(1, &m_vboID);
        m_vboID = 0;
        glDeleteVertexArrays(1, &m_vaoID);
        m_vaoID = 0;
    }

    //---------------------------------------------------------------------------------------
    bool PathRenderer::initShaders(void)
    //---------------------------------------------------------------------------------------
    {
        const bool res{ loadShaders(m_shaderPath, { "Shaders:path_vertex.glsl" }, { "Shaders:path_geometry.glsl" }, { "Shaders:path_fragment.glsl" }) };
        if (!res) qCritical() << "Fail to initialize shaders";
        return res;
    }

    //---------------------------------------------------------------------------------------
    void PathRenderer::deleteShaders(void)
    //---------------------------------------------------------------------------------------
    {
        m_shaderPath.removeAllShaders();
    }

    //---------------------------------------------------------------------------------------
    void PathRenderer::render(void)
    //---------------------------------------------------------------------------------------
    {
        if (!isInitialized())
        {
            qCritical() << "Internal error: data not initialized";
            return;
        }

        glEnable(GL_MULTISAMPLE); 
        glEnable(GL_LINE_SMOOTH);

        if (m_shaderPath.bind())
        {
            m_shaderPath.setUniformValue("ModelViewProjectionMatrix", (m_camera.projMatrix() * m_camera.viewMatrix() * m_scene.modelMatrix()));
            const QVector2D viewport(m_camera.viewPort()[2], m_camera.viewPort()[3]);
            m_shaderPath.setUniformValue("Viewport", viewport);
            m_shaderPath.setUniformValue("Thickness", m_thickness);
            m_shaderPath.setUniformValue("MiterLimit", 0.75f);
            m_shaderPath.setUniformValue("Segments", static_cast<GLint>(m_camera.getZoom()));

            // lock vao
            glBindVertexArray(m_vaoID);

            const GLint drawMode{ m_renderWithStrip ? GL_LINE_STRIP_ADJACENCY : GL_LINES_ADJACENCY };
            glDrawArrays(drawMode, 0, m_vertexObject.size() / 3);

            // unlock vao
            glBindVertexArray(0);

            m_shaderPath.release();
        }

        glDisable(GL_LINE_SMOOTH);
    }

}
