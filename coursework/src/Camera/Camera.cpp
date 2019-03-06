#include "Camera.h"


Camera::Camera(
      const Mat4& mat
    , const Vec3& pos
    , int sensitivity
) :
    m_mat(mat)
    , m_matTransposed(glm::transpose(mat))
    , m_pos(pos)
    , m_sensivity(sensitivity)
{}


void Camera::rotate(float dx, float dy)
{

}

void Camera::travelView(float distance)
{
    m_pos[0] += m_mat[3][0] * distance;
    m_pos[1] += m_mat[3][1] * distance;
    m_pos[2] += m_mat[3][2] * distance;

    updateMat();
}



void Camera::setPos(const Vec3& pos)
{
    m_pos = pos;

    updateMat();
}

void Camera::setMat(const Mat4& mat)
{
    m_mat = mat;
    m_matTransposed = glm::transpose(mat);
}


const Mat4& Camera::mat() const
{
    return m_mat;
}

const Vec3& Camera::pos() const
{
    return m_pos;
}



void Camera::updateMat()
{
    m_mat[3] = Vec4();
    m_mat[3] -= m_mat[0] * m_pos[0];
    m_mat[3] -= m_mat[1] * m_pos[1];
    m_mat[3] -= m_mat[2] * m_pos[2];
    m_mat[3][3] = 1.0f;


    m_matTransposed[0][3] = m_mat[3][0];
    m_matTransposed[1][3] = m_mat[3][1];
    m_matTransposed[2][3] = m_mat[3][2];
}
