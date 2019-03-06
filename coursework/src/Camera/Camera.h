#pragma once

#include "../Common.h"


class Camera
{
public:
    static const int DEFAULT_SENSITIVITY = 1000;


    Camera(const Camera& camera) = default;

    Camera(Camera&& camera) = default;

    Camera(
          const Mat4& mat = Mat4(1.0f)
        , const Vec3& pos = Vec3(0.0f)
        , int sensitivity = DEFAULT_SENSITIVITY
    ) : 
        m_mat(mat)
        , m_matTransposed(glm::transpose(mat))
        , m_pos(pos)
        , m_sensivity(sensitivity)
    {}


    Camera& operator = (const Camera& camera) = default;

    Camera& operator = (Camera&& camera) = default;



    void rotate(float dx, float dy)
    {

    }


    void travelView(float distance)
    {
        m_pos[0] -= m_mat[3][0] * distance;
        m_pos[1] -= m_mat[3][1] * distance;
        m_pos[2] -= m_mat[3][2] * distance;

        updateMat();
    }



    void setPos(const Vec3& pos)
    {
        m_pos = pos;
        
        updateMat();
    }


    const Mat4& mat() const
    {
        return m_mat;
    }

    const Vec3& pos() const
    {
        return m_pos;
    }


private:
    Mat4 m_mat;
    Mat4 m_matTransposed;

    Vec3 m_pos;

    int m_sensivity;


    void updateMat()
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
};