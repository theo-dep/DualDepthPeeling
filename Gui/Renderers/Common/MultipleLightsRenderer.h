#pragma once

#include <QtGui/QVector3D>
#include <QtOpenGL/QGL>

class QOpenGLShaderProgram;

namespace gui::gl
{

    /**
     * Common class for common shaders to render colors.
     */
    class MultipleLightsRenderer
    {
        friend class MeshRenderer;
        friend class DualDepthPeelingRenderer;

    public:
        explicit MultipleLightsRenderer(void);

        inline void setUseAmbiantLight(bool p_useAmbiantLight) { m_useAmbiantLight = p_useAmbiantLight; } //!< use spots if true, otherwise the mesh lights the scene
        inline void setMaterialOn(bool p_materialOn) { m_materialOn = p_materialOn; } //!< if false, plastic effect (see implants)

        inline void setColor(const QVector3D& p_color) { m_color = p_color; }
        inline void setOpacity(GLfloat p_opacity) { m_opacity = p_opacity; }
        inline GLfloat opacity(void) const { return m_opacity; }

        inline void setMaterialAmbiantColor(const QVector3D& p_color) { m_materialColor.ambiant = p_color; }
        inline void setMaterialDiffuseColor(const QVector3D& p_color) { m_materialColor.diffuse = p_color; }
        inline void setMaterialSpecularColor(const QVector3D& p_color) { m_materialColor.specular = p_color; }

    protected:
        static constexpr const char* shadeVertex(void) { return "Shaders:Common/shade_vertex.glsl"; }
        static constexpr const char* shadeFragment(void) { return "Shaders:Common/shade_fragment.glsl"; }

        void bindLightColor(QOpenGLShaderProgram& p_program, const QMatrix4x4& p_modelViewMatrix) const;

    private:
        struct Material
        {
            QVector3D ambiant;
            QVector3D diffuse;
            QVector3D specular;
        } m_materialColor;

        bool m_useAmbiantLight;
        bool m_materialOn;

        QVector3D m_color;
        GLfloat m_opacity;

        //Q_DISABLE_COPY_MOVE(MultipleLightsRenderer);
    };

}
