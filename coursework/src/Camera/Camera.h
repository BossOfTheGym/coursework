#pragma once

#include "../Common.h"


class Camera
{
public:
    static const int DEFAULT_SENSITIVITY = 200;


    Camera(const Camera& camera) = default;

    Camera(Camera&& camera) = default;

    Camera(
          const Mat4& mat = Mat4(1.0f)
        , const Vec3& pos = Vec3(0.0f)
        , int sensitivity = DEFAULT_SENSITIVITY
    );


    Camera& operator = (const Camera& camera) = default;

    Camera& operator = (Camera&& camera) = default;


    //TODO
    void lookAt();


    void rotate(float dHorisont, float dVertical);

    void travelView(float distance);



    void setPos(const Vec3& pos);

    void setMat(const Mat4& mat);


    const Mat4& mat() const;

    const Vec3& pos() const;


private:
    Mat4 m_mat;

    Vec3 m_pos;

    int m_sensivity;


    void updateMat();
};