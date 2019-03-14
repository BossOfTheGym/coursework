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
        //rot(alpha) * vec
        //[1, 0] & [0, 1]
        //[cos(a) -sin(a)]
        //[sin(a)  cos(a)]
        float cosAlpha;
        float sinAlpha;
        Vec3 newX;
        Vec3 newY;
        Vec3 newZ;

        //x, z
        cosAlpha = cos(dHorisont);
        sinAlpha = sin(dHorisont);
        newX[0] = +cosAlpha * m_mat[0][0] + sinAlpha * m_mat[0][2];
        newX[1] = +cosAlpha * m_mat[1][0] + sinAlpha * m_mat[1][2]; 
        newX[2] = +cosAlpha * m_mat[2][0] + sinAlpha * m_mat[2][2];

        newZ[0] = -sinAlpha * m_mat[0][0] + cosAlpha * m_mat[0][2];
        newZ[1] = -sinAlpha * m_mat[1][0] + cosAlpha * m_mat[1][2];
        newZ[2] = -sinAlpha * m_mat[2][0] + cosAlpha * m_mat[2][2];

        m_mat[0][0] = newX[0];
        m_mat[1][0] = newX[1];
        m_mat[2][0] = newX[2];

        m_mat[0][2] = newZ[0];
        m_mat[1][2] = newZ[1];
        m_mat[2][2] = newZ[2];
        
        //y, z
        cosAlpha = cos(-dVertical);
        sinAlpha = sin(-dVertical);
        newY[0] = +cosAlpha * m_mat[0][1] + sinAlpha * m_mat[0][2];
        newY[1] = +cosAlpha * m_mat[1][1] + sinAlpha * m_mat[1][2];
        newY[2] = +cosAlpha * m_mat[2][1] + sinAlpha * m_mat[2][2];

        newZ[0] = -sinAlpha * m_mat[0][1] + cosAlpha * m_mat[0][2];
        newZ[1] = -sinAlpha * m_mat[1][1] + cosAlpha * m_mat[1][2];
        newZ[2] = -sinAlpha * m_mat[2][1] + cosAlpha * m_mat[2][2];

        m_mat[0][1] = newY[0];
        m_mat[1][1] = newY[1];
        m_mat[2][1] = newY[2];

        m_mat[0][2] = newZ[0];
        m_mat[1][2] = newZ[1];
        m_mat[2][2] = newZ[2];

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
