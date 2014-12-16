
#include "CarRigidBody.h"
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace engine_physics;
using namespace glm;

CarRigidBody::CarRigidBody(float width, float height, float length, float density)
: mWidth(width),
mHeight(height),
mLength(length),
mDensity(density),
mPos(0.0f, 0.0f, 0.0f),
mLinVel(0.0f, 0.0f, 0.0f),
mLinAcc(0.0f, 0.0f, 0.0f),
mForceAccumulator(0.0f, 0.0f, 0.0f),
mAngVel(0.0f, 0.0f, 0.0f),
mAngAcc(0.0f, 0.0f, 0.0f),
mTorqueAccumulator(0.0f, 0.0f, 0.0f)
{
	vec3 EulerAngles(0, 0, 0);
	mOri = quat(EulerAngles);
	mat4 translationMatrix = translate(mat4(), mPos);
	mat4 rotationMatrix = toMat4(mOri);
	mM_previous = mM = translationMatrix * rotationMatrix;

	mMass = mWidth * mHeight * mLength * mDensity;
	mInvMass = 1.0f / mMass;

	mInertiaTensor = mat3x3(mMass*(height*height + length*length) / 12.0f, 0.0f, 0.0f,
		0.0f, mMass*(width*width + length*length) / 12.0f, 0.0f,
		0.0f, 0.0f, mMass*(width*width + height*height) / 12.0f);

	mInverseInertiaTensorBody = inverse(mInertiaTensor);
}

CarRigidBody::~CarRigidBody()
{

}

void CarRigidBody::Integrate(float dt)
{
	// linear
	mLinAcc = mInvMass * mForceAccumulator;
	mLinVel += mLinAcc * dt;

	// angular
	mAngAcc = mInverseInertiaTensorWorld * mTorqueAccumulator;
	mAngVel += mAngAcc * dt;

	// Impose drag.
	mLinVel *= pow(0.4f, dt);
	mAngVel *= pow(0.4f, dt);

	// apply change in position
	mPos += mLinVel * dt;

	// and rotation
	quat angVelChange = quat(0.0f, mAngVel.x, mAngVel.y, mAngVel.z) * dt * 0.5f;
	mOri += angVelChange * mOri;
	mOri = normalize(mOri);

	// update matrices and clear forces accumulators
	UpdateTransformationMatrix();
	ClearAccumulators();
}

void CarRigidBody::UpdateTransformationMatrix()
{
	mM_previous = mM;
	mat4 translationMatrix = translate(mat4(), mPos);
	mat4 rotationMatrix = toMat4(mOri);
	mM = translationMatrix * rotationMatrix;

	// inverse inertia tensor in world space
	mat3 R = toMat3(mOri);
	mat3 iR = transpose(R);
	mInverseInertiaTensorWorld = R * mInverseInertiaTensorBody * iR;
}

void CarRigidBody::ClearAccumulators()
{
	mForceAccumulator = vec3(0.0f, 0.0f, 0.0f);
	mTorqueAccumulator = vec3(0.0f, 0.0f, 0.0f);
}

void CarRigidBody::AddForceAtPoint(const vec3 &force, const vec3 &pointPos)
{
	vec3 relToBody = pointPos - mPos;

	// torque
	vec3 t = cross(relToBody, force);
	if (t.x != t.x || t.y != t.y || t.z != t.z) 
		t = vec3(0.0f);

	AddForce(force);
	AddTorque(t);
}