
#ifndef EN_PHYSICS_WORLD_H
#define EN_PHYSICS_WORLD_H

#include "CarRigidBody.h"
#include "ChassisCarController.h"
#include <Engine/Core/EventListener.h>
#include <Engine/Core/Updateable.h>

namespace engine_physics
{
	class World : public engine::EventListener, public engine::Updateable
	{
	public:
		World(const glm::vec2 &carPos, float carYRot);
		~World();

		const CarRigidBody &GetCar() { return mCar; }
		const ChassisCarController &GetChassis() { return mChassis; }

		float GetCarSpeed() { return mCarSpeed; }
		float getSpeedLimit() { return mCarSpeedLimit; }
		glm::vec3 getCarPosition() { return mChassis.mPos; }
		bool isTurning() { return mGoRight || mGoLeft; }

	private:
		void Update(float dt);
		void HandleEvent(const SDL_Event &e);
		void KeyPress(const SDL_KeyboardEvent &e);

		CarRigidBody mCar;
		ChassisCarController mChassis;
		bool mGoForward, mGoRight, mGoLeft;
		float mCarSteering, mCarSpeed, mCarSpeedLimit;
		glm::vec3 mGravitiy;
	};
}

#endif
