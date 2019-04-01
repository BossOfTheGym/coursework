#pragma once

#include <Common.h>


class View
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
	View(const View& camera) = default;

	View(View&& camera) = default;

	View(
		const Mat4& view = Mat4(1.0f)
		, const Mat4& proj = Mat4(1.0f)
		, const Vec3& pos = Vec3(0.0f)
		, int sensitivity = DEFAULT_SENSITIVITY
	);


	View& operator = (const View& camera) = default;

	View& operator = (View&& camera) = default;


	//TODO
	void lookAt();


	void rotate(float delta, Axis axis);

	void travelView(float distance, Axis axis);



	void setPos(const Vec3& pos);

	void setView(const Mat4& mat);

	void setProj(const Mat4& mat);


	const Mat4& view() const;

	const Mat4& proj() const;

	const Vec3& pos() const;


	int& sensivity();

	const int& sensivity() const;


private:
	void updateView();


private:
	Mat4 mView;
	Mat4 mProj;

	Vec3 mPos;

	int mSensivity;
};