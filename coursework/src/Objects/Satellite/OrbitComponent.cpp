#include "OrbitComponent.h"

#include <limits>

#include <Numerics/Equ/Neuton.h>


OrbitComponent::OrbitComponent(
	IComponent* parent
	, const PhysicsComponentWeak& planet
	, const PhysicsComponentWeak& satellite
)
	: IComponent(parent)

	, mPlanetPhys(planet)
	, mSatellitePhys(satellite)

	, mC(0.0f)
	, mI(0.0f)
	, mRA(0.0f)
	, mE(0.0f)
	, mAP(0.0f)
	, mTA(0.0f)

	, mP(0.0f)
	, mMu(0.0f)

	, mOP(0.0f)
	, mEA(0.0f)
	, mT(0.0f)
{
	updateOrbit();
}


void OrbitComponent::setOrbit(double h, double e, double i, double Omega, double omega, double theta)
{
	using glm::transpose;
	using glm::cross;
	using glm::length;


	double cost = cos(theta);
	double sint = sin(theta);

	double coso = cos(omega);
	double sino = sin(omega);
	double cosO = cos(Omega);
	double sinO = sin(Omega);
	double cosi = cos(i);
	double sini = sin(i);

	auto planet = mPlanetPhys.lock();
	auto sat = mSatellitePhys.lock();
	if (!sat || !planet)
	{
		return;
	}

	//hardcode - result of bad strtucture
	double mu = MU;

	Vec3 rx = h * h / mu / (1.0 + e * cost) * Vec3(cost, sint, 0.0);
	Vec3 vx = mu / h * Vec3(-sint, e + cost, 0.0);

	Mat3 Qxx = transpose(
		Mat3(
			cosO * coso - sinO * sino * cosi, -cosO * sino - sinO * cosi * coso, sinO * sini
			, sinO * coso + cosO * cosi * sino, -sinO * sino + cosO * cosi * coso, -cosO * sini
			, sini * sino, sini * coso, cosi
		)
	);

	//setting parameters
	sat->mPosition = Qxx * rx;
	sat->mMat[3] = Vec4(sat->mPosition, 1.0);
	sat->mVelocity = Qxx * vx;

	mC = h;
	mH = h;
	mE = e;
	mI = i;
	mP = h * h / mu;
	mAP = omega;
	mRA = Omega;
	mTA = theta;
	mMu = mu;

	mCv = cross(sat->mPosition, sat->mVelocity);
	mNv = cross(Vec3(0.0, 0.0, 1.0), mCv);
	mEv = cross(sat->mVelocity, mCv) / mMu - sat->mPosition / length(sat->mPosition);

	updateSpecificParams();
}

//TODO
OrbitComponent::R_V OrbitComponent::orbitStateAngle(double theta) const
{
	//TODO

	return {};
}

OrbitComponent::R_V OrbitComponent::orbitStateTime(const Time& dt) const
{
	//TODO

	return {};
}


void OrbitComponent::updateOrbit()
{
	using glm::cross;
	using glm::dot;
	using glm::length;


	auto planet = mPlanetPhys.lock();
	auto satellite = mSatellitePhys.lock();
	if (!planet || !satellite)
	{
		return;
	}

	//algorithm
	mMu = MU;//G * planet->mMass;

	//1.
	Vec3& r0 = satellite->mPosition;
	Vec3& v0 = satellite->mVelocity;
	Vec3& center = planet->mPosition;


	Vec3  rv = r0 - center;
	double r = length(rv);

	//2.
	Vec3  vv = v0;
	double v = length(v0);
	mH = dot(vv, vv) - 2.0 * mMu / r;

	//3.
	double vr = dot(rv / r, vv);

	//4, 5
	mCv = cross(rv, vv);//
	mC = length(mCv);
	mP = mC * mC / mMu;

	//6.
	mI = acos(mCv.z / mC);

	//7, 8
	mNv = cross(Vec3(0.0, 0.0, 1.0), mCv);
	double N = length(mNv);

	//9.
	mRA = acos(mNv.x / N);
	if (mNv.y <= std::numeric_limits<double>::epsilon())
	{
		mRA = PI_2 - mRA;
	}

	//10, 11
	mEv = cross(vv, mCv) / mMu - rv / r;
	mE = length(mEv);

	Vec3 uev = (mE > std::numeric_limits<double>::epsilon() ? mEv / mE : Vec3(0.0));

	//12.
	mAP = acos(dot((mNv / N), uev));
	if (mEv.z <= std::numeric_limits<double>::epsilon())
	{
		mAP = PI_2 - mAP;
	}

	//13.
	mTA = acos(dot(rv / r, uev));
	if (vr <= std::numeric_limits<double>::epsilon())
	{
		mTA = PI_2 - mTA;
	}

	updateSpecificParams();
}

void OrbitComponent::updateSpecificParams()
{
	if (mE < 1.0) //elliptic
	{
		mA = mP / (1.0 - mE * mE);
		mEA = 2.0 * atan(sqrt((1.0 - mE) / (1.0 + mE)) * tan(mTA / 2.0));
		if (mTA > PI)
		{
			mEA = PI_2 + mEA;
		}

		mN = sqrt(mMu) / pow(mA, 1.5);

		mT = 1.0 / mN * (mEA - mE * sin(mEA));
		mOP = 1.0 / mN * PI_2;
	}
	else if (mE > 1.0) // hyperbolic
	{
		mA = mP / (mE * mE - 1.0);
		mEA = 2 * atanh(sqrt((mE - 1.0) / (mE + 1.0)) * tanh(mTA / 2));

		mN = sqrt(mMu) / pow(mA, 1.5);

		mT = 1.0 / mN * (mE * sinh(mEA) - mEA); //something strange happens here
		mOP = std::numeric_limits<double>::infinity();
	}
	else
	{
		mA = 0.0;
		mT = 0.0;
		mOP = 0.0;
		mEA = 0.0;
	}
}


//#include "OrbitComponent.h"
//
//#include <limits>
//
//#include <Numerics/Equ/Neuton.h>
//
//
//OrbitComponent::OrbitComponent(IComponent* parent)
//	: IComponent(parent)
//
//	, m_h(0.0f)
//	, m_i(0.0f)
//	, m_ra(0.0f)
//	, m_e(0.0f)
//	, m_ap(0.0f)
//	, m_ta(0.0f)
//
//	, m_p(0.0f)
//
//	, m_op(0.0f)
//	, m_ea(0.0f)
//	, m_t(0.0f)
//{}
//
//
//void OrbitComponent::setOrbitFromParams(double mu, double h, double e, double i, double Omega, double omega, double theta)
//{
//	using glm::transpose;
//	using glm::cross;
//	using glm::length;
//
//
//	double cost = cos(theta);
//	double sint = sin(theta);
//
//	double coso = cos(omega);
//	double sino = sin(omega);
//	double cosO = cos(Omega);
//	double sinO = sin(Omega);
//	double cosi = cos(i);
//	double sini = sin(i);
//
//	Vec3 rx = h * h / mu / (1.0 + e * cost) * Vec3(cost, sint, 0.0);
//	Vec3 vx = mu / h * Vec3(-sint, e + cost, 0.0);
//
//	Mat3 Qxx = transpose(
//		Mat3(
//			  cosO * coso - sinO * sino * cosi, -cosO * sino - sinO * cosi * coso, sinO * sini
//			, sinO * coso + cosO * cosi * sino, -sinO * sino + cosO * cosi * coso, -cosO * sini
//			,        sini * sino              ,         sini * coso              ,      cosi
//		)
//	);
//
//	//setting parameters
//	m_r = Qxx * rx;
//	m_v = Qxx * vx;
//
//	m_h = h;
//	m_en = h;
//	m_e = e;
//	m_i = i;
//	m_p = h * h / mu;
//	m_ap = omega;
//	m_ra = Omega;
//	m_ta = theta;
//
//	m_hv = cross(m_r, m_v);
//	m_nv = cross(Vec3(0.0, 0.0, 1.0), m_hv);
//	m_ev = cross(m_v, m_hv) / mu - m_r / length(m_r);
//
//	updateSpecificParams(mu);
//}
//
////TODO
//OrbitComponent::R_V OrbitComponent::orbitStateAngle(double theta) const
//{
//	//TODO
//
//	return {};
//}
//
//OrbitComponent::R_V OrbitComponent::orbitStateTime(const Time& dt) const
//{
//	//TODO
//
//	return {};
//}
//
//
//void OrbitComponent::setOrbitFromStateVec(double mu, const Vec3& center, const Vec3& rVec, const Vec3& vVec)
//{
//	using glm::cross;
//	using glm::dot;
//	using glm::length;
//
//	//algorithm
//
//	//1.
//	Vec3  rv = rVec - center;
//	double r = length(rv);
//
//	//2.
//	Vec3  vv = vVec;
//	double v = length(vVec);
//	m_en = dot(vv, vv) - 2.0 * mu / r;
//
//	//3.
//	double vr = dot(rv / r, vv);
//
//	//4, 5
//	m_hv = cross(rv, vv);//
//	m_h  = length(m_hv);	
//	m_p  = m_h * m_h / mu;
//
//	//6.
//	m_i = acos(m_hv.z / m_h);
//
//	//7, 8
//	m_nv = cross(Vec3(0.0, 0.0, 1.0), m_hv);
//	double N = length(m_nv);
//	
//	//9.
//	m_ra = acos(m_nv.x / N);
//	if(m_nv.y <= std::numeric_limits<double>::epsilon())
//	{
//		m_ra = PI_2 - m_ra;
//	}	
//
//	//10, 11
//	m_ev = cross(vv, m_hv) / mu - rv / r;
//	m_e  = length(m_ev);
//
//	Vec3 uev = (m_e > std::numeric_limits<double>::epsilon() ? m_ev / m_e : Vec3(0.0));
//
//	//12.
//	m_ap = acos(dot((m_nv / N), uev));
//	if (m_ev.z <= std::numeric_limits<double>::epsilon())
//	{
//		m_ap = PI_2 - m_ap;
//	}
//
//	//13.
//	m_ta = acos(dot(rv / r, uev));
//	if(vr <= std::numeric_limits<double>::epsilon())
//	{
//		m_ta = PI_2 - m_ta;
//	}	
//
//	updateSpecificParams(mu);
//}
//
//void OrbitComponent::updateSpecificParams(double mu)
//{
//	if (m_e < 1.0) //elliptic
//	{
//		m_a  = m_p / (1.0 - m_e * m_e);
//		m_ea = 2.0 * atan(sqrt((1.0 - m_e) / (1.0 + m_e)) * tan(m_ta / 2.0));
//		if (m_ta > PI)
//		{
//			m_ea = PI_2 + m_ea;
//		}
//
//		m_n = sqrt(mu) / pow(m_a, 1.5);
//
//		m_t  = 1.0 / m_n * (m_ea - m_e * sin(m_ea));
//		m_op = 1.0 / m_n * PI_2;
//	}
//	else if (m_e > 1.0) // hyperbolic
//	{
//		m_a  = m_p / (m_e * m_e - 1.0);
//		m_ea = 2 * atanh(sqrt((m_e - 1.0) / (m_e + 1.0)) * tanh(m_ta / 2));
//
//		m_n = sqrt(mu) / pow(m_a, 1.5);
//
//		m_t  = 1.0 / m_n * (m_e * sinh(m_ea) - m_ea); //something strange happens here
//		m_op = std::numeric_limits<double>::infinity();
//	}
//	else
//	{
//		m_a = 0.0;
//		m_t = 0.0;
//		m_op = 0.0;
//		m_ea = 0.0;
//	}
//}