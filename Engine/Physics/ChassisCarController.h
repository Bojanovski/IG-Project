
#ifndef EN_CHASSIS_CAR_CONTROLLER_H
#define EN_CHASSIS_CAR_CONTROLLER_H

#include "CarRigidBody.h"
#include <vector>

namespace engine_physics
{
	class ChassisCarController
	{
		friend class World;

	public:
		ChassisCarController(CarRigidBody *car);
		~ChassisCarController();

		const glm::mat4 &GetWheelTransform_frontLeft() const { return mM_frontLeft; }
		const glm::mat4 &GetWheelTransform_frontRight() const { return mM_frontRight; }
		const glm::mat4 &GetWheelTransform_backLeft() const { return mM_backLeft; }
		const glm::mat4 &GetWheelTransform_backRight() const { return mM_backRight; }

	private:
		void UpdateTransformationMatrices();
		void Update();
		void AddToPosition(glm::vec3 v);
		float GetYRot() { return mYRot; }
		void  AddToYRot(float yRot) { mYRot += yRot; }
		void SetSteering(float yRot) { mYSteeringRot = yRot; }

		CarRigidBody *mCar;
		glm::vec3 mWheelDist;
		float mWheelsRadius;
		float mWheelsRotation;

		// springs
		std::vector<float> mRestingLengths;
		float mSpringCoefficient;

		// chassis position and rotation
		glm::vec3 mPos;
		float mYRot;
		float mYSteeringRot;

		// transformation matrices of the wheels
		glm::mat4 mM_frontLeft;
		glm::mat4 mM_frontRight;
		glm::mat4 mM_backLeft;
		glm::mat4 mM_backRight;
		glm::mat4 mM_frontLeft_prev;
		glm::mat4 mM_frontRight_prev;
		glm::mat4 mM_backLeft_prev;
		glm::mat4 mM_backRight_prev;

		// connection points on car
		glm::vec4 mCP_frontLeft;
		glm::vec4 mCP_frontRight;
		glm::vec4 mCP_backLeft;
		glm::vec4 mCP_backRight;
	};
}

#endif
