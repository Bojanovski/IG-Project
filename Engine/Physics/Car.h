
#ifndef EN_PHYSICS_CAR_H
#define EN_PHYSICS_CAR_H

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

namespace engine_physics
{
	class Car
	{
	public:
		Car(float width, float height, float length, float density);
		~Car();

		void Integrate(float dt);

	private:
		void UpdateTransformationMatrix();

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

		// angular stuff
		glm::quat mOri; // orientation
		glm::vec3 mAngVel;
		glm::vec3 mAngAcc;

		// transformation matrix
		glm::mat4 mM;

		glm::mat3x3 mInertiaTensor;
		glm::mat3x3 mInverseInertiaTensor;
	};
}

#endif
