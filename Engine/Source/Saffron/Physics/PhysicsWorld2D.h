#pragma once

#include "Saffron/Base.h"

class b2World;

namespace Se
{
class Scene;

class PhysicsWorld2D
{
public:
	explicit PhysicsWorld2D(Scene& scene);
	~PhysicsWorld2D();
	PhysicsWorld2D(const PhysicsWorld2D& world);
	PhysicsWorld2D(PhysicsWorld2D&& world) noexcept;
	PhysicsWorld2D& operator=(const PhysicsWorld2D& world);

	void OnUpdate();
	void OnGuiRender();
	void OnStart();
	void OnStop();

	Vector2 GetGravity() const;
	void SetGravity(const Vector2& gravity);

private:
	b2World* _nativeWorld;
	Scene* _scene;
	class Entity* _physicsBodyEntityBuffer;
	bool _filledWorld = false;
};
}
