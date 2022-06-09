#include "Mesh/MeshModel.h"

#include <QtCore/QFile>
#include <QtCore/QtDebug>
#include <QtCore/QTextStream>
#include <QtCore/QVarLengthArray>

//-----------------------------------------------------------------------------
MeshModel::MeshModel()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
MeshModel::MeshModel(const QString &p_filePath, bool p_flipY/*=false*/, bool p_copyNormals/*=false*/)
    : m_fileName(p_filePath)
//-----------------------------------------------------------------------------
{
    loadObjFile(m_fileName, p_flipY, p_copyNormals);
}

//-----------------------------------------------------------------------------
MeshModel::~MeshModel()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void MeshModel::clear()
//-----------------------------------------------------------------------------
{
    m_fileName.clear();
    m_points.clear();
    m_normals.clear();
    m_texIndices.clear();
    m_pointIndices.clear();
}

//-----------------------------------------------------------------------------
void MeshModel::loadObjFile(const QString& p_filePath, bool p_flipY, bool p_copyNormals)
//-----------------------------------------------------------------------------
{
    m_fileName = p_filePath;

    bool hasNormals = false;

    geom::Point boundsMin( 1e9, 1e9, 1e9);
    geom::Point boundsMax(-1e9,-1e9,-1e9);

    QFile file(p_filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "Can't open file '" << p_filePath << "' for reading";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v")
        {
            double px, py, pz;
            ts >> px >> py >> pz;
            boundsMin.x(qMin(boundsMin.x(), px));
            boundsMin.y(qMin(boundsMin.y(), py));
            boundsMin.z(qMin(boundsMin.z(), pz));
            boundsMax.x(qMax(boundsMax.x(), px));
            boundsMax.y(qMax(boundsMax.y(), py));
            boundsMax.z(qMax(boundsMax.z(), pz));

            m_points << geom::Point(px,p_flipY?-py:py,pz);
        }
        else if (id == "f" || id == "fo")
        {
            QVarLengthArray<int, 4> p;
            QVarLengthArray<int, 4> t;
            while (!ts.atEnd())
            {
                QString str;
                ts >> str;
                QStringList strList = str.split('/');
                const int vertexIndex = strList.value(0).toInt();
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : m_points.size() + vertexIndex);

                const int texIndex = strList.value(1).toInt();
                t.append(texIndex > 0 ? texIndex - 1 : -1);
            }


            if (p_flipY)
            {
                for (int i = 0; i < 3; ++i)
                {
                    m_pointIndices << p[2-i];
                    if (t[2-i]>-1) m_texIndices << t[2-i];
                }

                if (p.size() == 4)
                {
                    m_pointIndices << p[0];
                    if (t[0]>-1) m_texIndices << t[0];
                    m_pointIndices << p[3];
                    if (t[3]>-1) m_texIndices << t[3];
                    m_pointIndices << p[2];
                    if (t[2]>-1) m_texIndices << t[2];
                }
            }
            else
            {
                for (int i = 0; i < 3; ++i)
                {
                    m_pointIndices << p[i];
                    if (t[i]>-1) m_texIndices   << t[i];
                }

                if (p.size() == 4)
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        m_pointIndices << p[(i+2)%4]; // 2 3 0
                        if (t[(i+2)%4]>-1) m_texIndices << t[(i+2)%4];
                    }
                }
            }
        }
        else if ((id == "vn") && (p_copyNormals))
        {
            // EAM 05/03/2021 : this is used by DataConverter to copy obj normals into the mesh.bin instead of recomputing them.
            // Helps to resolve face rendering issues in TestGLWidget
            double nx, ny, nz;
            ts >> nx >> ny >> nz;

            m_normals << geom::Vector(geom::Point::ORIGIN(), geom::Point(nx, p_flipY ? -ny : ny, nz));

            hasNormals = true;
        }

    }


    if (!p_copyNormals || !hasNormals) // re-computes normals by default, or if normals were supposed to be copied but there were none in input file
    {
        m_normals.resize(m_points.size());
        for (int i = 0; i < m_pointIndices.size(); i += 3)
        {
            const geom::Point a = m_points.at(m_pointIndices.at(i));
            const geom::Point b = m_points.at(m_pointIndices.at(i+1));
            const geom::Point c = m_points.at(m_pointIndices.at(i+2));

            const geom::Vector normal = (geom::Vector(b - a)^geom::Vector(c - a)).normalized();
      
            for (int j = 0; j < 3; ++j)
                m_normals[m_pointIndices.at(i + j)] += normal;
        }
    }

    file.close();
}

//-----------------------------------------------------------------------------
void MeshModel::loadObjPath(const QString& p_filePath, bool p_flipY)
//-----------------------------------------------------------------------------
{
    loadObjFile(p_filePath, p_flipY, false /*compute normals*/);
}
