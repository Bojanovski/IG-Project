
#include "Car.h"
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace engine_physics;
using namespace glm;

Car::Car(float width, float height, float length, float density)
: mWidth(width),
mHeight(height),
mLength(length),
mDensity(density),
mPos(0.0f, 0.0f, 0.0f),
mLinVel(0.0f, 0.0f, 0.0f),
mLinAcc(0.0f, 0.0f, 0.0f),
mAngVel(0.0f, 0.0f, 0.0f),
mAngAcc(0.0f, 0.0f, 0.0f)
{
	vec3 EulerAngles(0, 0, 0);
	mOri = quat(EulerAngles);

	mMass = mWidth * mHeight * mLength * mDensity;
	mInvMass = 1.0f / mMass;

	mInertiaTensor = mat3x3(mMass*(height*height + length*length) / 12.0f, 0.0f, 0.0f,
		0.0f, mMass*(width*width + length*length) / 12.0f, 0.0f,
		0.0f, 0.0f, mMass*(width*width + height*height) / 12.0f);

	mInverseInertiaTensor = inverse(mInertiaTensor);
}

Car::~Car()
{

}

void Car::Integrate(float dt)
{

}

void Car::UpdateTransformationMatrix()
{
	mat4 translationMatrix;
	translate(translationMatrix, mPos);
	mat4 rotationMatrix = toMat4(mOri);
	mM = translationMatrix * rotationMatrix;
}