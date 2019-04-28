#include "View.h"


//statics
const double View::DELTA_THRESHOLD = 0.00001;

const double View::DEFAULT_SENSITIVITY = 0.25;

const double View::PITCH_MAX = 2.0 * PI;
const double View::PITCH_MIN = 0.0;

const double View::ROLL_MAX = +PI / 2.0;
const double View::ROLL_MIN = -PI / 2.0;


//constructor
View::View(
	  const Mat4& axes
	, const Mat4& proj
	, const Vec3& pos
	, double sensitivity
	, const PhysicsComponentWeak& track
) 
	: IObject(nullptr)
	, mView(axes)
	, mAxes(axes)
	, mProj(proj)
	, mPos(pos)
	, mLastPos(pos)
	, mPitch(0.0)
	, mRoll(0.0)
	, mSensivity(sensitivity)
	, mTrack(track)
	, lastTrackNull(track.expired())
	, shouldUpdateAxes(false)
{
	updateAxes();
}



//core
void View::lookAround(double pitch, double roll)
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
		mRoll = ROLL_MAX;
	}
	if (mRoll < ROLL_MIN)
	{
		mRoll = ROLL_MIN;
	}

	mView = mAxes;
	rotateView(mPitch, Y);
	rotateView(mRoll,  X);
}


namespace
{
	void rotateMat(Mat4& mat, double delta, View::Axis axis)
	{
		static View::Axis cycled[5] = {View::X, View::Y, View::Z, View::X, View::Y};

		if (abs(delta) > View::DELTA_THRESHOLD)
		{
			//rot(alpha) * vec
			//[1, 0] & [0, 1]
			//[cos(a) -sin(a)]
			//[sin(a)  cos(a)]
			double cosDelta = cos(delta);
			double sinDelta = sin(delta);

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

void View::rotateAxes(double angle, Axis axis)
{
	mAxes = mView;

	rotateMat(mAxes, angle, axis);

	shouldUpdateAxes = true;
}

void View::rotateView(double angle, Axis axis)
{
	rotateMat(mView, angle, axis);
}


void View::travelView(double distance, Axis axis)
{
	mPos[0] -= mView[0][axis] * distance;
	mPos[1] -= mView[1][axis] * distance;
	mPos[2] -= mView[2][axis] * distance;
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

	//updateView();
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
	return mLastPos;
}

const Vec3& View::lastPos() const
{
	return mLastPos;
}


double& View::sensivity()
{
	return mSensivity;
}

const double& View::sensivity() const
{
	return mSensivity;
}


void View::updateView()
{
	mView[3] = Vec4();
	mView[3] -= mView[0] * mLastPos[0];
	mView[3] -= mView[1] * mLastPos[1];
	mView[3] -= mView[2] * mLastPos[2];
	mView[3][3] = 1.0;
}

void View::updateAxes()
{
	mPitch = 0.0;
	mRoll  = 0.0;

	mView = mAxes;

	updateView();
}


void View::update(const Time& t, const Time& dt)
{
	if (auto track = mTrack.lock())
	{
		lastTrackNull = false;

		mLastPos = track->mPosition + mPos;
	}
	else
	{
		if (!lastTrackNull)
		{
			lastTrackNull = true;

			mPos = mLastPos;
		}

		mLastPos = mPos;
	}

	if (shouldUpdateAxes)
	{
		shouldUpdateAxes = false;

		updateAxes();
	}
	
	updateView();
}

const IComponent::Type& View::componentType() const
{
	static Type type = "View";

	return type;
}