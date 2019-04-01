#include "View.h"


//statics
const float View::DELTA_THRESHOLD = 0.0001f;

const int View::DEFAULT_SENSITIVITY = 200;


//constructor
View::View(
	const Mat4& view
	, const Mat4& proj
	, const Vec3& pos
	, int sensitivity
) 
	: mView(view)
	, mProj(proj)
	, mPos(pos)
	, mSensivity(sensitivity)
{}


//core


void View::rotate(float delta, Axis axis)
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

		newAxis1[0] = cosDelta * mView[0][axis1] - sinDelta * mView[0][axis2];
		newAxis1[1] = cosDelta * mView[1][axis1] - sinDelta * mView[1][axis2];
		newAxis1[2] = cosDelta * mView[2][axis1] - sinDelta * mView[2][axis2];

		newAxis2[0] = sinDelta * mView[0][axis1] + cosDelta * mView[0][axis2];
		newAxis2[1] = sinDelta * mView[1][axis1] + cosDelta * mView[1][axis2];
		newAxis2[2] = sinDelta * mView[2][axis1] + cosDelta * mView[2][axis2];

		mView[0][axis1] = newAxis1[0];
		mView[1][axis1] = newAxis1[1];
		mView[2][axis1] = newAxis1[2];

		mView[0][axis2] = newAxis2[0];
		mView[1][axis2] = newAxis2[1];
		mView[2][axis2] = newAxis2[2];

		updateView();
	}
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


int& View::sensivity()
{
	return mSensivity;
}

const int& View::sensivity() const
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
