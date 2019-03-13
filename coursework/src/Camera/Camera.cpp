#include "Camera.h"


Camera::Camera(
      const Mat4& mat
    , const Vec3& pos
    , int sensitivity
) :
      m_mat(mat)
    , m_pos(pos)
    , m_sensivity(sensitivity)
{}


void Camera::rotate(float dHorisont, float dVertical)
{
    dHorisont /= m_sensivity;
    dVertical /= m_sensivity;

    if (abs(dHorisont) > 0.0001 || abs(dVertical) > 0.0001)
    {
        m_mat = glm::rotate(Mat4(1.0f), (abs(dHorisont) + abs(dVertical)) / 2, Vec3(dVertical, dHorisont, 0.0f)) * m_mat;

        updateMat();
    }
}

void Camera::travelView(float distance)
{
    m_pos[0] -= m_mat[0][2] * distance;
    m_pos[1] -= m_mat[1][2] * distance;
    m_pos[2] -= m_mat[2][2] * distance;

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
}
