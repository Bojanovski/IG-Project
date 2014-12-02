
#include "World.h"
#include <glm\glm.hpp>

using namespace engine_physics;

World::World()
: mCar(2.0f, 1.0f, 3.0f, 1.0f)
{
	glm::vec3 v = glm::vec3(1.0f, 0.0f, 0.0f);
}

World::~World()
{

}

void World::Update(float dt)
{
	int a = 0;
	a = 3;
}
