
#include "Car.h"

using namespace engine_physics;

Car::Car(float width, float height, float length, float density)
: mWidth(width),
mHeight(height),
mLength(length),
mDensity(density)
{
	mMass = mWidth * mHeight * mLength * mDensity;
	mInvMass = 1.0f / mMass;

	mInertiaTensor = glm::mat3x3(mMass*(height*height + length*length) / 12.0f, 0.0f, 0.0f,
		0.0f, mMass*(width*width + length*length) / 12.0f, 0.0f,
		0.0f, 0.0f, mMass*(width*width + height*height) / 12.0f);

	mInverseInertiaTensor = glm::inverse(mInertiaTensor);
}

Car::~Car()
{

}

void Car::Integrate(float dt)
{

}