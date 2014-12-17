
#ifndef EN_PHYSICS_WORLD_H
#define EN_PHYSICS_WORLD_H

#include "CarRigidBody.h"
#include "ChassisCarController.h"
#include <Engine/Core/EventListener.h>
#include <Engine/Core/Updateable.h>

namespace engine_physics
{
	struct StraightRoad
	{
		static const float mHalfLength;
		static const float mWidth;
		glm::vec2 mDir;
		glm::vec2 mPos;
	};

	struct TurnRoad
	{
		static const float mRadius;
		static const float mWidth;
		glm::vec2 mDir;
		glm::vec2 mPos;
	};

	class World : public engine::EventListener, public engine::Updateable
	{
	public:
		World();
		~World();

		const CarRigidBody &GetCar() { return mCar; }
		const ChassisCarController &GetChassis() { return mChassis; }

		void Initialize(const glm::vec2 &carPos, float carYRot);
		void AddStraightRoads(std::vector<glm::mat4> &sRoads);
		void AddTurnRoads(std::vector<glm::mat4> &tRoads);
		float GetCarSpeed() { return mCarSpeed; }
		float getSpeedLimit() { return mCarSpeedLimit; }
		glm::vec3 getCarPosition() { return mChassis.mPos; }
		bool isTurning() { return mGoRight || mGoLeft; }

	private:
		float CalcualteElevationAtPoint(float x, float z);
		void Update(float dt);
		void HandleEvent(const SDL_Event &e);
		void KeyPress(const SDL_KeyboardEvent &e);

		CarRigidBody mCar;
		ChassisCarController mChassis;
		bool mGoForward, mGoRight, mGoLeft;
		float mCarSteering, mCarSpeed, mCarSpeedLimit;
		glm::vec3 mGravitiy;
		std::vector<StraightRoad> mSRoads;
		std::vector<TurnRoad> mTRoads;
	};
}

#endif
