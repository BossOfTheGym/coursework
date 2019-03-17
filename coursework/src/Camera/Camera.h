#pragma once

#include "../Common.h"


class Camera
{
public:
    static const int DEFAULT_SENSITIVITY;

	static const float DELTA_THRESHOLD;

	enum Axis : int
	{
		  X = 0
		, Y = 1
		, Z = 2
	};


public:
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


    void rotate(float delta, Axis axis);

    void travelView(float distance, Axis axis);



    void setPos(const Vec3& pos);

    void setMat(const Mat4& mat);


    const Mat4& mat() const;

    const Vec3& pos() const;


private:
    Mat4 mMat;

    Vec3 mPos;

    int mSensivity;


    void updateMat();
};