
#ifndef EN_PHYSICS_CAR_H
#define EN_PHYSICS_CAR_H

#include <glm\glm.hpp>

namespace engine_physics
{
	class Car
	{
	public:
		Car(float width, float height, float length, float density);
		~Car();

		void Integrate(float dt);

	private:
		float mWidth;
		float mHeight;
		float mLength;
		float mDensity;

		float mMass;
		float mInvMass; // inverse mass

		glm::mat3x3 mInertiaTensor;
		glm::mat3x3 mInverseInertiaTensor;
	};
}

#endif
