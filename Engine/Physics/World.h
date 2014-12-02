
#ifndef EN_PHYSICS_WORLD_H
#define EN_PHYSICS_WORLD_H

#include "Car.h"
#include <Engine/Core/EventListener.h>
#include <Engine/Core/Updateable.h>

namespace engine_physics
{
	class World : public engine::EventListener, public engine::Updateable
	{
	public:
		World();
		~World();

		void Update(float dt);

	private:
		Car mCar;
	};
}

#endif
