#pragma once

#include "Geom/Point.h"

#include <QtCore/QString>
#include <QtCore/QVector>

class MeshModel
{
public:
    MeshModel();
    MeshModel(const QString &p_filePath, bool p_flipY = false, bool p_copyNormals = false);
    virtual ~MeshModel();

    void clear();

    inline const QString& fileName() const { return m_fileName; }
    inline int faceCount() const { return (m_pointIndices.size() / 3); }
    inline int pointCount() const { return m_points.size(); }

    inline const QVector<geom::Point>&  vertices() const { return m_points; }
    
    inline const QVector<geom::Vector>& normals() const { return m_normals; }
    
    inline const QVector<int>& vtxIndices() const { return m_pointIndices; }

    /// \brief Call \c loadObjFile but open the file given by the path \c p_filePath before
    void loadObjPath(const QString& p_filePath, bool p_flipY);

protected:

    /// \brief Load data from file \c p_filePath.
    /// Normals are computed, not read, expected if p_copyNormals is set to true
    void loadObjFile(const QString& p_filePath, bool p_flipY, bool p_copyNormals);

private:
    QString m_fileName;

    QVector<geom::Point>  m_points;
    QVector<geom::Vector> m_normals;

    QVector<int> m_texIndices;
    QVector<int> m_pointIndices; 
}; 
