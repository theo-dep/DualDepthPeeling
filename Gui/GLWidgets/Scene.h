#pragma once

#include <QtGui/QMatrix4x4>

namespace gui
{

    class Scene
    {
    public:
        Scene();
        virtual ~Scene();

        const QMatrix4x4& modelMatrix() const { return m_modelMatrix; }

    protected:
        QMatrix4x4 m_modelMatrix;
    };

}
