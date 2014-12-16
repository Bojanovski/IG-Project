
#include "World.h"
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\vector_angle.hpp>

using namespace engine_physics;
using namespace glm;
using namespace std;

const float StraightRoad::mLength = 4.0f;
const float StraightRoad::mWidth = 4.0f;

World::World()
: mCar(1.6f, 1.2f, 3.0f, 1.0f),
mGravitiy(0.0f, -9.81f, 0.0f),
mChassis(&mCar),
mGoForward(false),
mGoRight(false),
mGoLeft(false),
mCarSteering(0.0f),
mCarSpeed(0.0f),
mCarSpeedLimit(5.0f)
{

}

World::~World()
{

}

void World::Initialize(const vec2 &carPos, float carYRot)
{
	float elevation = 0.7f;
	mCar.mPos = vec3(carPos.x, elevation, carPos.y);
	mCar.UpdateTransformationMatrix();
	mCar.UpdateTransformationMatrix(); // two times for "_previous" data
	mChassis.AddToPosition(vec3(carPos.x, elevation, carPos.y));
	mChassis.UpdateTransformationMatrices();
	mChassis.UpdateTransformationMatrices(); // two times for "_previous" data
}

void World::AddStraightRoads(vector<mat4> &sRoads)
{
	StraightRoad road;
	for (unsigned int i = 0; i < sRoads.size(); ++i)
	{
		vec4 dir = sRoads[i] * vec4(0.0f, 0.0f, 1.0f, 0.0f);
		dir = normalize(dir);
		vec4 pos = sRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		road.mDir = vec2(dir.x, dir.z);
		road.mPos = vec2(pos.x, pos.z);
		mSRoads.push_back(road);
	}
}

void World::CalcualteElevationAtPoint(const vec2 &p)
{
	//vec4 p4 = vec4(p.x, 0.0f, p.y, 1.0f);
	//// straight roads
	//for (unsigned int i = 0; i < mSRoads.size(); ++i)
	//{
	//	vec3 perp = 
	//}
}

void World::Update(float dt)
{
	if (mGoForward && mCarSpeed < mCarSpeedLimit)
	{
		float carAcceleration = 3.0f;
		mCarSpeed += carAcceleration * dt;
	}
	else
	{
		float carDeacceleration = -6.0f;
		mCarSpeed += carDeacceleration * dt;
		if (mCarSpeed < 0.0f) mCarSpeed = 0.0f;
	}
	if (mGoLeft != mGoRight)
	{
		float coeff = (3.0f / abs(mCarSpeed));
		if (coeff > 1.0f) coeff = 1.0f;
		float currentMaxSteering = 3.14f / 4.0f * coeff;
		//if (mCarSpeed > 1.0f) currentMaxSteering /= mCarSpeed * 0.01f;
		mCarSteering += ((mGoLeft) ? 1.0f : -1.0f) * 2.0f * dt;
		if (mCarSteering > currentMaxSteering) mCarSteering = currentMaxSteering;
		if (mCarSteering < -currentMaxSteering) mCarSteering = -currentMaxSteering;
	}
	else if (mCarSteering != 0.0f)
	{
		float signCarSteering = sign(mCarSteering);
		mCarSteering -= signCarSteering * 2.0f * dt;
		if (sign(mCarSteering) != signCarSteering) mCarSteering = 0.0f; // we have crossed over 0.0f
	}
	mChassis.SetSteering(mCarSteering);

	vec4 frontLeft = mChassis.GetWheelTransform_frontLeft() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 frontRight = mChassis.GetWheelTransform_frontRight() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 backLeft = mChassis.GetWheelTransform_backLeft() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 backRight = mChassis.GetWheelTransform_backRight() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 front = (frontLeft + frontRight)* 0.5f;
	vec4 back = (backLeft + backRight)* 0.5f;
	vec4 cPos = (front + back)* 0.5f;
	vec4 dir = front - back;
	float length = glm::sqrt(dot(dir, dir));
	vec4 dirN = normalize(dir);
	vec4 newFront = front +
		toMat4(quat(vec3(0.0f, mChassis.GetYRot(), 0.0f))) *
		toMat4(quat(vec3(0.0f, mCarSteering, 0.0f))) * vec4(0.0f, 0.0f, mCarSpeed, 0.0f) * dt;

	float a = dirN.x*dirN.x + dirN.y*dirN.y + dirN.z*dirN.z;
	vec4 back_minus_newFront = back - newFront;
	float b = 2.0f*(dirN.x*back_minus_newFront.x + dirN.y*back_minus_newFront.y + dirN.z*back_minus_newFront.z);
	float c = back_minus_newFront.x*back_minus_newFront.x +
		back_minus_newFront.y*back_minus_newFront.y +
		back_minus_newFront.z*back_minus_newFront.z - length*length;
	float D = b*b - 4.0f*a*c;
	if (D >= 0.0f)
	{
		float s1 = (-b + glm::sqrt(D)) / (2.0f*a);
		float s2 = (-b - glm::sqrt(D)) / (2.0f*a);
		float s = -1.0f;
		if (s1 >= 0.0f) s = s1;
		if (s2 < s && s2 >= 0.0f) s = s2; // we need the smaller one (but positive)

		if (s > 0.0f && s < length)
		{
			vec4 newBack = back + s*dirN;
			vec4 newDir = newFront - newBack;
			vec4 newDirN = normalize(newDir);
			vec4 newCPos = (newFront + newBack) * 0.5f;
			vec4 offsetPos = newCPos - cPos;
			mChassis.AddToPosition(vec3(offsetPos));
			float angle = radians(glm::angle(dirN, newDirN));
			mChassis.AddToYRot(sign(mCarSteering) * abs(angle));

			//static float jo = 3.0f;
			//jo += dt;
			//mChassis.mElevation_backLeft = 0.5f*sin(jo);
		}
	}

	mChassis.Update(dt);
	//mCar.AddForceAtPoint(-mGravitiy, vec3(1.0f, 0.0f, 0.0f));
	//mCar.AddTorque(vec3(0.0f, 1.0f, 0.0f));
	mCar.AddForce(mGravitiy);
	mCar.Integrate(dt);
}

void World::HandleEvent(const SDL_Event &e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
	case SDL_KEYDOWN:
		KeyPress(e.key);
		break;
	}
}

void World::KeyPress(const SDL_KeyboardEvent &e)
{
	if (e.repeat)
		return;

	const int mod = e.type == SDL_KEYDOWN ? 1 : -1;
	switch (e.keysym.sym)
	{
	case SDLK_i:
		mGoForward = (e.type == SDL_KEYDOWN);
		break;
	case SDLK_j:
		mGoLeft = (e.type == SDL_KEYDOWN);
		break;
	case SDLK_l:
		mGoRight = (e.type == SDL_KEYDOWN);
		break;
	case SDLK_1:
		mCarSpeedLimit = 5.0f * 1.0f;
		break;
	case SDLK_2:
		mCarSpeedLimit = 5.0f * 2.0f;
		break;
	case SDLK_3:
		mCarSpeedLimit = 5.0f * 3.0f;
		break;
	case SDLK_4:
		mCarSpeedLimit = 5.0f * 4.0f;
		break;
	default:
		break;
	}
}
