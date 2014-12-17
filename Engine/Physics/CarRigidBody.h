
#ifndef EN_PHYSICS_CAR_RIGIDBODY_H
#define EN_PHYSICS_CAR_RIGIDBODY_H

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace engine_physics
{
	class CarRigidBody
	{
		friend class World;
		friend class ChassisCarController;

	public:
		CarRigidBody(float width, float height, float length, float density);
		~CarRigidBody();
		
        const glm::mat4 &GetTransform() const { return mM; }
        const glm::vec3 &GetPos() const { return mPos; }
        const glm::vec3 &GetLinVel() const { return mLinVel; }
        const glm::vec3 &GetLinAcc() const { return mLinAcc; }

	private:
		void Integrate(float dt);
		void UpdateTransformationMatrix();
		void ClearAccumulators();
		void AddForce(const glm::vec3 &force) { mForceAccumulator += force; }
		void AddTorque(const glm::vec3 &torque) { mTorqueAccumulator += torque; }
		// calculates force and torque for when we apply a force on a point in the body in world coordinates
		void AddForceAtPoint(const glm::vec3 &force, const glm::vec3 &pointPos);

		float mWidth;
		float mHeight;
		float mLength;
		float mDensity;

		float mMass;
		float mInvMass; // inverse mass

		// linear stuff
		glm::vec3 mPos;
		glm::vec3 mLinVel;
		glm::vec3 mLinAcc;
		glm::vec3 mForceAccumulator;

		// angular stuff
		glm::quat mOri; // orientation
		glm::vec3 mAngVel;
		glm::vec3 mAngAcc;
		glm::vec3 mTorqueAccumulator;

		// transformation matrix
		glm::mat4 mM;
		glm::mat4 mM_previous;

		glm::mat3x3 mInertiaTensor;
		glm::mat3x3 mInverseInertiaTensorBody;
		glm::mat3x3 mInverseInertiaTensorWorld;
	};
}

#endif
