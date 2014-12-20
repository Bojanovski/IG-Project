
#include "World.h"
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\vector_angle.hpp>

#define PI_DIV2 1.57079632f
#define PI_DIV4 0.78539816f

using namespace engine_physics;
using namespace glm;
using namespace std;

const float StraightRoad::mHalfLength = 8.0f;
const float StraightRoad::mWidth = 4.0f;
const float TurnRoad::mRadius = 8.0f;
const float TurnRoad::mWidth = 4.0f;

World::World()
: mCar(1.6f, 1.2f, 3.0f, 1.0f),
mGravitiy(0.0f, -9.81f, 0.0f),
mChassis(&mCar),
mGoForward(false),
mGoBackwards(false),
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
	float elevation = 0.8f;
	mCar.mPos = vec3(carPos.x, elevation, carPos.y);
	mCar.UpdateTransformationMatrix();
	mCar.UpdateTransformationMatrix(); // two times for "_previous" data
	mChassis.AddToPosition(vec3(carPos.x, elevation, carPos.y), false);
	mChassis.UpdateTransformationMatrices();
	mChassis.UpdateTransformationMatrices(); // two times for "_previous" data
}

void World::AddStraightRoads(vector<mat4> &sRoads)
{
	StraightRoad road;
	for (unsigned int i = 0; i < sRoads.size(); ++i)
	{
		vec4 dir = sRoads[i] * vec4(1.0f, 0.0f, 0.0f, 0.0f);
		dir = normalize(dir);
		vec4 pos = sRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		road.mDir = vec2(dir.x, dir.z);
		road.mPos = vec2(pos.x, pos.z);
		mSRoads.push_back(road);
	}
}

void World::AddTurnRoads(std::vector<glm::mat4> &tRoads)
{
	TurnRoad road;
	for (unsigned int i = 0; i < tRoads.size(); ++i)
	{
		vec4 dir = tRoads[i] * vec4(1.0f, 0.0f, 1.0f, 0.0f);
		dir = normalize(dir);
		vec4 pos = tRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road.mDir = vec2(dir.x, dir.z);
		road.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road);
	}
}

void World::AddTRoads(std::vector<glm::mat4> &TRoads)
{
	StraightRoad road_1;
	TurnRoad road_2, road_3;
	for (unsigned int i = 0; i < TRoads.size(); ++i)
	{
		// first straight road
		vec4 dir = TRoads[i] * vec4(1.0f, 0.0f, 0.0f, 0.0f);
		dir = normalize(dir);
		vec4 pos = TRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		road_1.mDir = vec2(dir.x, dir.z);
		road_1.mPos = vec2(pos.x, pos.z);
		mSRoads.push_back(road_1);

		// now the first curved road
		dir = TRoads[i] * vec4(1.0f, 0.0f, 1.0f, 0.0f);
		dir = normalize(dir);
		pos = TRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road_2.mDir = vec2(dir.x, dir.z);
		road_2.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road_2);

		// now the second curved road
		dir = TRoads[i] * vec4(-1.0f, 0.0f, 1.0f, 0.0f);
		dir = normalize(dir);
		pos = TRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road_3.mDir = vec2(dir.x, dir.z);
		road_3.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road_3);
	}
}

void World::AddCrossRoads(std::vector<glm::mat4> &cRoads)
{
	TurnRoad road_1, road_2, road_3, road_4;
	for (unsigned int i = 0; i < cRoads.size(); ++i)
	{
		// the first curved road
		vec4 dir = cRoads[i] * vec4(1.0f, 0.0f, 1.0f, 0.0f);
		dir = normalize(dir);
		vec4 pos = cRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road_1.mDir = vec2(dir.x, dir.z);
		road_1.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road_1);

		// now the second curved road
		dir = cRoads[i] * vec4(-1.0f, 0.0f, 1.0f, 0.0f);
		dir = normalize(dir);
		pos = cRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road_2.mDir = vec2(dir.x, dir.z);
		road_2.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road_2);

		// now the third curved road
		dir = cRoads[i] * vec4(1.0f, 0.0f, -1.0f, 0.0f);
		dir = normalize(dir);
		pos = cRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road_3.mDir = vec2(dir.x, dir.z);
		road_3.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road_3);

		// now the fourth curved road
		dir = cRoads[i] * vec4(-1.0f, 0.0f, -1.0f, 0.0f);
		dir = normalize(dir);
		pos = cRoads[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f) - dir*glm::sqrt(StraightRoad::mHalfLength*StraightRoad::mHalfLength*2.0f);
		road_4.mDir = vec2(dir.x, dir.z);
		road_4.mPos = vec2(pos.x, pos.z);
		mTurnRoads.push_back(road_4);
	}
}

float World::CalcualteElevationAtPoint(float x, float z)
{
	vec3 p4 = vec3(x, 0.0f, z);
	float elevation = 0.25f;
	float slopeWidth = 1.0f;
	// straight roads
	for (unsigned int i = 0; i < mSRoads.size(); ++i)
	{
		vec3 pDir = vec3(mSRoads[i].mPos.x, 0.0f, mSRoads[i].mPos.y) - p4;
		vec3 roadDir = normalize(vec3(mSRoads[i].mDir.x, 0.0f, mSRoads[i].mDir.y));
		vec3 perp = cross(roadDir, pDir);
		vec3 towardsP = normalize(cross(perp, roadDir));
		float proj = dot(pDir, towardsP); // this is also the distance from line that goes through road segment
		float projOnRoad = dot(pDir, roadDir);
		if (abs(projOnRoad) > StraightRoad::mHalfLength) continue;
		if (abs(proj) > (StraightRoad::mWidth + slopeWidth)) continue;
		float tempElevation = 1.0f - (abs(proj) - StraightRoad::mWidth) / (slopeWidth);
		if (tempElevation > 1.0f) tempElevation = 1.0f;
		if (tempElevation < 0.0f) tempElevation = 0.0f;
		float newElevation = 0.5f * tempElevation;
		if (newElevation > elevation)
			elevation = newElevation;
	}

	// turn roads
	for (unsigned int i = 0; i < mTurnRoads.size(); ++i)
	{
		vec3 pRel = p4 - vec3(mTurnRoads[i].mPos.x, 0.0f, mTurnRoads[i].mPos.y);
		float dist = abs(glm::sqrt(dot(pRel, pRel)) - TurnRoad::mRadius);
		vec3 roadQuadrantDir = normalize(vec3(mTurnRoads[i].mDir.x, 0.0f, mTurnRoads[i].mDir.y));
		float angle = dot(roadQuadrantDir, normalize(pRel));
		if (angle < cos(PI_DIV4)) continue;
		if (dist > (TurnRoad::mWidth + slopeWidth)) continue;
		float tempElevation = 1.0f - (dist - StraightRoad::mWidth) / (slopeWidth);
		if (tempElevation > 1.0f) tempElevation = 1.0f;
		if (tempElevation < 0.0f) tempElevation = 0.0f;
		float newElevation = 0.5f * tempElevation;
		if (newElevation > elevation)
			elevation = newElevation;
	}

	return elevation;
}

void World::Update(float dt)
{
	float backwardsSpeedLimit = -3.0f;
	// must be F, not B, must be below speed limit and not be going in reverse
	if (mGoForward && !mGoBackwards && mCarSpeed < mCarSpeedLimit && mCarSpeed >= 0.0f)
	{
		float carAcceleration = 3.0f;
		mCarSpeed += carAcceleration * dt;
	}
	// must be B, not F, must be below reverse speed limit and not be going in positive direction
	else if (mGoBackwards && !mGoForward && mCarSpeed > backwardsSpeedLimit && mCarSpeed <= 0.0f)
	{
		float carAcceleration = -3.0f;
		mCarSpeed += carAcceleration * dt;
	}
	else
	{
		float carDeacceleration = -6.0f*sign(mCarSpeed);
		if (abs(mCarSpeed) > abs(carDeacceleration) * dt) mCarSpeed += carDeacceleration * dt;
		else mCarSpeed = 0.0f;
	}

	if (mGoLeft != mGoRight)
	{
		float coeff = (3.0f / abs(mCarSpeed));
		if (coeff > 1.0f) coeff = 1.0f;
		float currentMaxSteering = 3.14f / 4.0f * coeff;
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
		float s = 0.0f;
		bool isOk = false;
		if (mCarSpeed > 0.0f)
		{
			s = s1;
			if (s2 < s && s2 >= 0.0f) s = s2; // we need the smaller one (but positive)
			if (s > 0.0f && s < length) isOk = true;
		}
		else if (mCarSpeed < 0.0f)
		{
			s = s1;
			if (s > 0.0f) s = s2;
			if (s < 0.0f && s > -length) isOk = true;
		}

		if (isOk)
		{
			vec4 newBack = back + s*dirN;
			vec4 newDir = newFront - newBack;
			vec4 newDirN = normalize(newDir);
			vec4 newCPos = (newFront + newBack) * 0.5f;
			vec4 offsetPos = newCPos - cPos;
			offsetPos.y = 0.0f;
			mChassis.AddToPosition(vec3(offsetPos), (mCarSpeed < 0.0f));
			float angle = radians(glm::angle(dirN, newDirN));
			mChassis.AddToYRot(sign(mCarSteering) * abs(angle) * sign(mCarSpeed));
		}
	}

	// wheels elevation
	mChassis.mElevation_frontLeft = CalcualteElevationAtPoint(frontLeft.x, frontLeft.z);
	mChassis.mElevation_frontRight = CalcualteElevationAtPoint(frontRight.x, frontRight.z);
	mChassis.mElevation_backLeft = CalcualteElevationAtPoint(backLeft.x, backLeft.z);
	mChassis.mElevation_backRight = CalcualteElevationAtPoint(backRight.x, backRight.z);

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
	case SDLK_k:
		mGoBackwards = (e.type == SDL_KEYDOWN);
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
	default:
		break;
	}
}
