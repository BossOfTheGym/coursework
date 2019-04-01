#include "View.h"


//statics
const float View::DELTA_THRESHOLD = 0.0001f;

const float View::DEFAULT_SENSITIVITY = 0.25f;

const float View::PITCH_MAX = 2 * PI;
const float View::PITCH_MIN = 0;

const float View::ROLL_MAX = +PI / 2;
const float View::ROLL_MIN = -PI / 2;


//constructor
View::View(
	  const Mat4& axes
	, const Mat4& proj
	, const Vec3& pos
	, float sensitivity
) 
	: mView(axes)
	, mAxes(axes)
	, mProj(proj)
	, mPos(pos)
	, mPitch(0.0)
	, mRoll(0.0)
	, mSensivity(sensitivity)
{}



//core
void View::lookAround(float pitch, float roll)
{
	mPitch += pitch / mSensivity;
	mRoll  += roll / mSensivity;

	if (mPitch > PITCH_MAX)
	{
		mPitch = PITCH_MIN;
	}
	if (mPitch < PITCH_MIN)
	{
		mPitch = PITCH_MAX;
	}
	if (mRoll > ROLL_MAX)
	{
		mRoll = ROLL_MIN;
	}
	if (mRoll < ROLL_MIN)
	{
		mRoll = ROLL_MAX;
	}

	mView = mAxes;
	rotateView(mPitch, Y);
	rotateView(mRoll,  X);
}


namespace
{
	void rotateMat(Mat4& mat, float delta, View::Axis axis)
	{
		static View::Axis cycled[5] = {View::X, View::Y, View::Z, View::X, View::Y};

		if (abs(delta) > View::DELTA_THRESHOLD)
		{
			//rot(alpha) * vec
			//[1, 0] & [0, 1]
			//[cos(a) -sin(a)]
			//[sin(a)  cos(a)]
			float cosDelta = cos(delta);
			float sinDelta = sin(delta);

			View::Axis axis1 = cycled[axis + 1];
			View::Axis axis2 = cycled[axis + 2];

			Vec3 newAxis1;
			Vec3 newAxis2;

			newAxis1[0] = cosDelta * mat[0][axis1] - sinDelta * mat[0][axis2];
			newAxis1[1] = cosDelta * mat[1][axis1] - sinDelta * mat[1][axis2];
			newAxis1[2] = cosDelta * mat[2][axis1] - sinDelta * mat[2][axis2];

			newAxis2[0] = sinDelta * mat[0][axis1] + cosDelta * mat[0][axis2];
			newAxis2[1] = sinDelta * mat[1][axis1] + cosDelta * mat[1][axis2];
			newAxis2[2] = sinDelta * mat[2][axis1] + cosDelta * mat[2][axis2];

			mat[0][axis1] = newAxis1[0];
			mat[1][axis1] = newAxis1[1];
			mat[2][axis1] = newAxis1[2];

			mat[0][axis2] = newAxis2[0];
			mat[1][axis2] = newAxis2[1];
			mat[2][axis2] = newAxis2[2];
		}
	}
}

void View::rotateAxes(float angle, Axis axis)
{
	mAxes = mView;

	rotateMat(mAxes, angle, axis);

	updateAxes();
}

void View::rotateView(float angle, Axis axis)
{
	rotateMat(mView, angle, axis);

	updateView();
}


void View::travelView(float distance, Axis axis)
{
	mPos[0] -= mView[0][axis] * distance;
	mPos[1] -= mView[1][axis] * distance;
	mPos[2] -= mView[2][axis] * distance;

	updateView();
}



void View::setView(const Mat4& mat)
{
	mView = mat;
}

void View::setProj(const Mat4& mat)
{
	mProj = mat;
}

void View::setPos(const Vec3& pos)
{
	mPos = pos;

	updateView();
}


const Mat4& View::axes() const
{
	return mAxes;
}

const Mat4& View::view() const
{
	return mView;
}

const Mat4& View::proj() const
{
	return mProj;
}

const Vec3& View::pos() const
{
	return mPos;
}


float& View::sensivity()
{
	return mSensivity;
}

const float& View::sensivity() const
{
	return mSensivity;
}


void View::updateView()
{
	mView[3] = Vec4();
	mView[3] -= mView[0] * mPos[0];
	mView[3] -= mView[1] * mPos[1];
	mView[3] -= mView[2] * mPos[2];
	mView[3][3] = 1.0f;
}

void View::updateAxes()
{
	mPitch = 0.0f;
	mRoll  = 0.0f;

	mView = mAxes;

	updateView();
}
