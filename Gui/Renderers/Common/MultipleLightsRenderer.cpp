#include "Renderers/Common/MultipleLightsRenderer.h"

#include <QtGui/QOpenGLShaderProgram>

#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace gui::gl
{

    //---------------------------------------------------------------------------------------
    MultipleLightsRenderer::MultipleLightsRenderer(void)
        : m_useAmbiantLight(true)
        , m_materialOn(true)
        , m_opacity(1.f)
    //---------------------------------------------------------------------------------------
    {
    }

    //---------------------------------------------------------------------------------------
    void MultipleLightsRenderer::bindLightColor(QOpenGLShaderProgram& p_program, const QMatrix4x4& p_modelViewMatrix) const
    //---------------------------------------------------------------------------------------
    {
        p_program.setUniformValue("UseAmbiantLight", m_useAmbiantLight);
        p_program.setUniformValue("Color", m_color);
        p_program.setUniformValue("Alpha", m_opacity);

        p_program.setUniformValue("ModelViewMatrix", p_modelViewMatrix);
        p_program.setUniformValue("NormalMatrix", p_modelViewMatrix.normalMatrix());

        if (m_useAmbiantLight)
        {
            p_program.setUniformValue("material.ambient", m_materialColor.ambiant);
            p_program.setUniformValue("material.diffuse", m_materialColor.diffuse);
            p_program.setUniformValue("material.specular", m_materialColor.specular);

            p_program.setUniformValue("materialOn", m_materialOn);

            // directional light
            p_program.setUniformValue("dirLight.direction", 0.0f, 0.0f, -1.0f);
            p_program.setUniformValue("dirLight.ambient", 0.2f, 0.2f, 0.2f);
            p_program.setUniformValue("dirLight.diffuse", 0.7f, 0.7f, 0.7f);
            p_program.setUniformValue("dirLight.specular", 0.3f, 0.3f, 0.3f);

            // spotLight
            p_program.setUniformValue("spotLight.position", 200.f, 100.f, 500.f);
            static const QVector3D spotDirection(0.f, .7f, -.7f);
            p_program.setUniformValue("spotLight.direction", spotDirection);
            p_program.setUniformValue("spotLight.ambient", 0.1f, 0.1f, 0.1f);
            p_program.setUniformValue("spotLight.diffuse", 0.4f, 0.4f, 0.4f);
            p_program.setUniformValue("spotLight.specular", 0.0f, 0.0f, 0.0f);
            p_program.setUniformValue("spotLight.constant", 1.0f);
            // The attenuation factors are (1, 0, 0), resulting in no attenuation
            p_program.setUniformValue("spotLight.linear", 0.0f);
            p_program.setUniformValue("spotLight.quadratic", 0.0f);
            static const float cutOff = std::cos(M_PI * 70.f / 180.f);
            p_program.setUniformValue("spotLight.cutOff", cutOff);
            static const float outerCutOff = std::cos(M_PI * 0.f / 180.f);
            p_program.setUniformValue("spotLight.outerCutOff", outerCutOff);
        }
    }

}
