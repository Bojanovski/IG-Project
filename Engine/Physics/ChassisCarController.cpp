
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
mYSteeringRot(0.0f)
{
	mWheelDist = vec3(0.8f, 0.6f, 1.3f);
	mCP_frontLeft = vec4(mWheelDist.x, -0.2f, mWheelDist.z, 1.0f);
	mCP_frontRight = vec4(-mWheelDist.x, -0.2f, mWheelDist.z, 1.0f);
	mCP_backLeft = vec4(mWheelDist.x, -0.2f, -mWheelDist.z, 1.0f);
	mCP_backRight = vec4(-mWheelDist.x, -0.2f, -mWheelDist.z, 1.0f);

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
	// wheels (rotate the right wheels)
	mM_frontLeft = translate(mat4(1.0f), vec3(mWheelDist.x, -mWheelDist.y, mWheelDist.z)) * toMat4(quat(vec3(mWheelsRotation, mYSteeringRot, 0.0f)));
	mM_frontRight = translate(mat4(1.0f), vec3(-mWheelDist.x, -mWheelDist.y, mWheelDist.z)) * toMat4(quat(vec3(-mWheelsRotation, 3.14f + mYSteeringRot, 0.0f)));
	mM_backLeft = translate(mat4(1.0f), vec3(mWheelDist.x, -mWheelDist.y, -mWheelDist.z)) * toMat4(quat(vec3(mWheelsRotation, 0.0f, 0.0f)));
	mM_backRight = translate(mat4(1.0f), vec3(-mWheelDist.x, -mWheelDist.y, -mWheelDist.z)) * toMat4(quat(vec3(-mWheelsRotation, 3.14f, 0.0f)));

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

void ChassisCarController::Update()
{
	UpdateTransformationMatrices();

	vec4 cpWorld = mCar->GetTransform() * mCP_frontLeft;
	vec4 wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	float d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[0];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_frontRight;
	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[1];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_backLeft;
	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[2];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_backRight;
	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	wheelRelPos.x = wheelRelPos.z = 0.0f;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[3];
	mCar->AddForceAtPoint(-sign(wheelRelPos.y) * vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	// inner cross springs
	cpWorld = mCar->GetTransform() * mCP_backRight;
	wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[4];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_frontLeft;
	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[5];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_backLeft;
	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[6];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_frontRight;
	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[7];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	// outer cross springs
	cpWorld = mCar->GetTransform() * mCP_frontRight;
	wheelRelPos = mM_frontLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[8];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_frontLeft;
	wheelRelPos = mM_frontRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[9];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_backLeft;
	wheelRelPos = mM_backRight * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[10];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));

	cpWorld = mCar->GetTransform() * mCP_backRight;
	wheelRelPos = mM_backLeft * vec4(0.0f, 0.0f, 0.0f, 1.0f) - cpWorld;
	d = glm::sqrt(dot(wheelRelPos, wheelRelPos)) - mRestingLengths[11];
	mCar->AddForceAtPoint(vec3(normalize(wheelRelPos) * d * mSpringCoefficient), vec3(cpWorld));
}

void ChassisCarController::AddToPosition(glm::vec3 v)
{
	float length = glm::sqrt(dot(v, v));
	mWheelsRotation += length / mWheelsRadius;
	mPos += v;
}