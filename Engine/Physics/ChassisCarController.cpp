
#include "ChassisCarController.h"
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;
using namespace engine_physics;
using namespace std;

ChassisCarController::ChassisCarController(CarRigidBody *car)
: mCar(car),
mPos(vec3(0.0f)),
mYRot(0.0f),
mSpringCoefficient(200.0f),
mWheelsRadius(0.3f),
mWheelsRotation(0.0f),
mYSteeringRot(0.0f),
mElevation_frontLeft(0.0f),
mElevation_frontRight(0.0f),
mElevation_backLeft(0.0f),
mElevation_backRight(0.0f)
{
	mWheelDist = vec3(0.8f, 0.6f, 1.3f);
	mCP_frontLeft = vec4(mWheelDist.x, -0.2f, mWheelDist.z, 1.0f);
	mCP_frontRight = vec4(-mWheelDist.x, -0.2f, mWheelDist.z, 1.0f);
	mCP_backLeft = vec4(mWheelDist.x, -0.2f, -mWheelDist.z, 1.0f);
	mCP_backRight = vec4(-mWheelDist.x, -0.2f, -mWheelDist.z, 1.0f);

	mM_frontLeft = mM_frontRight = mM_backLeft = mM_backRight = mat4(1.0f); // for previous in UpdateTransformationMatrices
	UpdateTransformationMatrices();

	// calcualte resting lengths
	vec4 wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_frontLeft;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_frontRight;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_backLeft;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_backRight;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	// inner cross resting lengths
	wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_backRight;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_frontLeft;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_backLeft;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_frontRight;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	// outer cross resting lengths (front and back separately connected)
	wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_frontRight;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_frontLeft;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_backLeft;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));

	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - car->GetTransform() * mCP_backRight;
	mRestingLengths.push_back(glm::sqrt(dot(wheelRelPos, wheelRelPos)));
}

ChassisCarController::~ChassisCarController()
{

}

void ChassisCarController::UpdateTransformationMatrices()
{
	// set current to previous before update
	mM_frontLeft_prev = mM_frontLeft;
	mM_frontRight_prev = mM_frontRight;
	mM_backLeft_prev = mM_backLeft;
	mM_backRight_prev = mM_backRight;

	// wheels (rotate the right wheels)
	mM_frontLeft = translate(mat4(1.0f), vec3(mWheelDist.x, -mWheelDist.y + mElevation_frontLeft, mWheelDist.z)) * toMat4(quat(vec3(mWheelsRotation, mYSteeringRot, 0.0f)));
	mM_frontRight = translate(mat4(1.0f), vec3(-mWheelDist.x, -mWheelDist.y + mElevation_frontRight, mWheelDist.z)) * toMat4(quat(vec3(-mWheelsRotation, 3.14f + mYSteeringRot, 0.0f)));
	mM_backLeft = translate(mat4(1.0f), vec3(mWheelDist.x, -mWheelDist.y + mElevation_backLeft, -mWheelDist.z)) * toMat4(quat(vec3(mWheelsRotation, 0.0f, 0.0f)));
	mM_backRight = translate(mat4(1.0f), vec3(-mWheelDist.x, -mWheelDist.y + mElevation_backRight, -mWheelDist.z)) * toMat4(quat(vec3(-mWheelsRotation, 3.14f, 0.0f)));

	// whole chassis
	mat4 translationMatrix = translate(mat4(), mPos);
	mat4 rotationMatrix = toMat4(quat(vec3(0.0f, mYRot, 0.0f)));
	mat4 chassisM = translationMatrix * rotationMatrix;

	// go to world coordinates
	mM_frontLeft = chassisM * mM_frontLeft;
	mM_frontRight = chassisM * mM_frontRight;
	mM_backLeft = chassisM * mM_backLeft;
	mM_backRight = chassisM * mM_backRight;
}

vec3 Damping(const vec4 &pos1, const vec4 &pos1_old, const vec4&pos2_rel, const vec4 &pos2_rel_old, float dt)
{
	vec4 vel1 = (pos1 - pos1_old)/dt;
	vec4 vel2 = (pos2_rel - pos2_rel_old)/dt;
	vec4 dirN = normalize(pos2_rel);
	vec3 damping = vec3(dirN * dot(dirN, vel2 - vel1));
	return damping;
}

void ChassisCarController::Update(float dt)
{
	UpdateTransformationMatrices();
	float mDampingC = 3.0f;
	float mDampingC_innerCrossSprings = 50.0f;
	float mDampingC_outerCrossSprings = 30.0f;

	vec4 cpWorld = mCar->mM * mCP_frontLeft;
	vec4 cpWorld_prev = mCar->mM_previous * mCP_frontLeft;
	vec4 wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	vec4 wheelRelPos_prev = mM_frontLeft_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	wheelRelPos_prev.x = wheelRelPos_prev.z = 0.0f;
	float d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[0];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	vec3 damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_frontRight;
	cpWorld_prev = mCar->mM_previous * mCP_frontRight;
	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_frontRight_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	wheelRelPos_prev.x = wheelRelPos_prev.z = 0.0f;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[1];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_backLeft;
	cpWorld_prev = mCar->mM_previous * mCP_backLeft;
	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_backLeft_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	wheelRelPos_prev.x = wheelRelPos_prev.z = 0.0f;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[2];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_backRight;
	cpWorld_prev = mCar->mM_previous * mCP_backRight;
	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_backRight_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	wheelRelPos_prev.x = wheelRelPos_prev.z = 0.0f;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[3];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC, vec3(cpWorld));

	// inner cross springs
	cpWorld = mCar->mM * mCP_backRight;
	cpWorld_prev = mCar->mM_previous * mCP_backRight;
	wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_frontLeft_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[4];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_innerCrossSprings, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_frontLeft;
	cpWorld_prev = mCar->mM_previous * mCP_frontLeft;
	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_backRight_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[5];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_innerCrossSprings, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_backLeft;
	cpWorld_prev = mCar->mM_previous * mCP_backLeft;
	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_frontRight_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[6];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_innerCrossSprings, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_frontRight;
	cpWorld_prev = mCar->mM_previous * mCP_frontRight;
	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_backLeft_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[7];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_innerCrossSprings, vec3(cpWorld));

	// outer cross springs
	cpWorld = mCar->mM * mCP_frontRight;
	cpWorld_prev = mCar->mM_previous * mCP_frontRight;
	wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_frontLeft_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[8];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_outerCrossSprings, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_frontLeft;
	cpWorld_prev = mCar->mM_previous * mCP_frontLeft;
	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_frontRight_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[9];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_outerCrossSprings, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_backLeft;
	cpWorld_prev = mCar->mM_previous * mCP_backLeft;
	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_backRight_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[10];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_outerCrossSprings, vec3(cpWorld));

	cpWorld = mCar->mM * mCP_backRight;
	cpWorld_prev = mCar->mM_previous * mCP_backRight;
	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos_prev = mM_backLeft_prev * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[11];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
	damping = Damping(cpWorld, cpWorld_prev, wheelRelPos, wheelRelPos_prev, dt);
	mCar->AddForceAtPoint(damping*mDampingC_outerCrossSprings, vec3(cpWorld));
}

void ChassisCarController::AddToPosition(glm::vec3 v)
{
	float length = glm::sqrt(dot(v, v));
	mWheelsRotation += length / mWheelsRadius;
	mPos += v;
}