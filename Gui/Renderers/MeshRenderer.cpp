#include "Renderers/MeshRenderer.h"

#include "GLWidgets/Camera.h"
#include "GLWidgets/Scene.h"

#include <Mesh/MeshModel.h>

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    MeshRenderer::MeshRenderer(const MeshModel& p_mesh, const Scene& p_scene, const Camera& p_camera) : AbstractRenderer(p_scene, p_camera)
        , m_mesh(p_mesh)
        , m_vaoID(0)
        , m_vboID(0)
    //---------------------------------------------------------------------------------------
    {
        setUseAmbiantLight(defaultAmbiantLightUser());
        setMaterialOn(defaultMaterialOn());

        setMaterialAmbiantColor(defaultMaterialAmbiantColor());
        setMaterialDiffuseColor(defaultMaterialDiffuseColor());
        setMaterialSpecularColor(defaultMaterialSpecularColor());
    }

    //---------------------------------------------------------------------------------------
    MeshRenderer::~MeshRenderer(void)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    bool MeshRenderer::isOtherGlFunctionsInitialized(void) const
    //---------------------------------------------------------------------------------------
    {
        return (m_vaoID != 0 && m_vboID != 0);
    }

    //---------------------------------------------------------------------------------------
    bool MeshRenderer::updateOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        if (m_mesh.vertices().isEmpty())
        {
            qCritical() << "internal error, empty mesh" ;
            return false;
        }

        // lock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBindVertexArray(m_vaoID);

        const int numTriangle{ m_mesh.faceCount() };
        QVector<float> vertices(9 * numTriangle);
        QVector<float> normals(9 * numTriangle);
        const GLintptr bufferSize{ static_cast<GLintptr>(9 * numTriangle * sizeof(float)) };

        // transfer data
        // vertices
        for (int i = 0, iCount = 0; i < m_mesh.vtxIndices().size(); i++)
        {
            const geom::Point & pt = m_mesh.vertices()[m_mesh.vtxIndices()[i]];
            vertices[iCount++] = static_cast<float>(pt.x());
            vertices[iCount++] = static_cast<float>(pt.y());
            vertices[iCount++] = static_cast<float>(pt.z());
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, vertices.data());

        // normals
        for (int i = 0, iCount = 0; i < m_mesh.vtxIndices().size(); i++)
        {
            const geom::Vector & norm = m_mesh.normals()[m_mesh.vtxIndices()[i]];
            normals[iCount++] = static_cast<float>(norm.x());
            normals[iCount++] = static_cast<float>(norm.y());
            normals[iCount++] = static_cast<float>(norm.z());
        }
        glBufferSubData(GL_ARRAY_BUFFER, bufferSize, bufferSize, normals.data());

        // data access
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(bufferSize));

        // unlock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return true;
    }

    //---------------------------------------------------------------------------------------
    bool MeshRenderer::initOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        // ID generation
        glGenBuffers(1, &m_vboID);
        glGenVertexArrays(1, &m_vaoID);

        // lock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBindVertexArray(m_vaoID);

        // allocate memory
        const int numTriangle{ m_mesh.faceCount() };
        const GLintptr bufferSize{ static_cast<GLintptr>(9 * numTriangle * sizeof(float)) };

        glBufferData(GL_ARRAY_BUFFER, 2 * bufferSize, nullptr, GL_STATIC_DRAW);  // vertices + normals

        // unlock vbo vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return updateOtherGlFunctions();
    }

    //---------------------------------------------------------------------------------------
    void MeshRenderer::deleteOtherGlFunctions(void)
    //---------------------------------------------------------------------------------------
    {
        glDeleteBuffers(1, &m_vboID);
        m_vboID = 0;
        glDeleteVertexArrays(1, &m_vaoID);
        m_vaoID = 0;
    }

    //---------------------------------------------------------------------------------------
    bool MeshRenderer::initShaders(void)
    //---------------------------------------------------------------------------------------
    {
        const bool res{ loadShaders(m_shaderMultipleLights,
            { MultipleLightsRenderer::shadeVertex(), "Shaders:multiple_lights_vertex.glsl" },
            { MultipleLightsRenderer::shadeFragment(), "Shaders:multiple_lights_fragment.glsl" })
        };
        if (!res) qCritical() << "Fail to initialize shaders";
        return res;
    }

    //---------------------------------------------------------------------------------------
    void MeshRenderer::deleteShaders(void)
    //---------------------------------------------------------------------------------------
    {
        m_shaderMultipleLights.removeAllShaders();
    }

    //---------------------------------------------------------------------------------------
    void MeshRenderer::renderMesh(QOpenGLShaderProgram& p_program, bool p_withLightColorShader, std::function<void()> p_beforeRenderMeshFunc, std::function<void()> p_afterRenderMeshFunc)
    //---------------------------------------------------------------------------------------
    {
        if (p_program.bind())
        {
            const bool enableBlending{ isClassicalRendering() && opacity() < 1.f };
            if (enableBlending)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            p_program.setUniformValue("ModelViewProjectionMatrix", (m_camera.projMatrix() * m_camera.viewMatrix() * m_scene.modelMatrix()));
            if (p_withLightColorShader)
            {
                bindLightColor(p_program, (m_camera.viewMatrix() * m_scene.modelMatrix()));
            }

            p_beforeRenderMeshFunc();

            // lock vao
            glBindVertexArray(m_vaoID);

            glDrawArrays(GL_TRIANGLES, 0, 3 * m_mesh.faceCount());

            // unlock vao
            glBindVertexArray(0);

            p_afterRenderMeshFunc();

            p_program.release();

            if (enableBlending)
            {
                glDisable(GL_BLEND);
            }
        }
    }

    //---------------------------------------------------------------------------------------
    void MeshRenderer::render(void)
    //---------------------------------------------------------------------------------------
    {
        if (!isInitialized())
        {
            qCritical() << "Internal error: data not initialized";
            return;
        }

        renderMesh(m_shaderMultipleLights, true);
    }

}
