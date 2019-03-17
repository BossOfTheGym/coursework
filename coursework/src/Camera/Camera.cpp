#include "Camera.h"


//statics
const float Camera::DELTA_THRESHOLD = 0.0001f;

const int Camera::DEFAULT_SENSITIVITY = 200;


//constructor
Camera::Camera(
      const Mat4& mat
    , const Vec3& pos
    , int sensitivity
) :
      mMat(mat)
    , mPos(pos)
    , mSensivity(sensitivity)
{}


//core
void Camera::rotate(float delta, Axis axis)
{
	static Axis cycled[5] = {X, Y, Z, X, Y};

    delta /= mSensivity;

    if (abs(delta) > DELTA_THRESHOLD)
    {
        //rot(alpha) * vec
        //[1, 0] & [0, 1]
        //[cos(a) -sin(a)]
        //[sin(a)  cos(a)]
        float cosDelta = cos(delta);
        float sinDelta = sin(delta);

		Axis axis1 = cycled[axis + 1];
		Axis axis2 = cycled[axis + 2];

		Vec3 newAxis1;
		Vec3 newAxis2;

		newAxis1[0] = cosDelta * mMat[0][axis1] - sinDelta * mMat[0][axis2];
		newAxis1[1] = cosDelta * mMat[1][axis1] - sinDelta * mMat[1][axis2];
		newAxis1[2] = cosDelta * mMat[2][axis1] - sinDelta * mMat[2][axis2];

		newAxis2[0] = sinDelta * mMat[0][axis1] + cosDelta * mMat[0][axis2];
		newAxis2[1] = sinDelta * mMat[1][axis1] + cosDelta * mMat[1][axis2];
		newAxis2[2] = sinDelta * mMat[2][axis1] + cosDelta * mMat[2][axis2];

		mMat[0][axis1] = newAxis1[0];
		mMat[1][axis1] = newAxis1[1];
		mMat[2][axis1] = newAxis1[2];

		mMat[0][axis2] = newAxis2[0];
		mMat[1][axis2] = newAxis2[1];
		mMat[2][axis2] = newAxis2[2];

        updateMat();
    }
}

void Camera::travelView(float distance, Axis axis)
{
    mPos[0] -= mMat[0][axis] * distance;
    mPos[1] -= mMat[1][axis] * distance;
    mPos[2] -= mMat[2][axis] * distance;

    updateMat();
}



void Camera::setPos(const Vec3& pos)
{
    mPos = pos;

    updateMat();
}

void Camera::setMat(const Mat4& mat)
{
    mMat = mat;
}


const Mat4& Camera::mat() const
{
    return mMat;
}

const Vec3& Camera::pos() const
{
    return mPos;
}



void Camera::updateMat()
{
    mMat[3] = Vec4();
    mMat[3] -= mMat[0] * mPos[0];
    mMat[3] -= mMat[1] * mPos[1];
    mMat[3] -= mMat[2] * mPos[2];
    mMat[3][3] = 1.0f;
}
