#pragma once

#include "Saffron/Scene/Scene.h"

namespace Se
{
class RuntimeScene : public Scene
{
public:
	explicit RuntimeScene(String name, Shared<Scene> copyFrom);

	void OnUpdate() override;
	void OnRender() override;
	void OnGuiRender() override;
	void OnStart();
	void OnStop();

	void Escape();
	void Return();

	const Shared<EditorCamera> &GetDebugCamera() const { return m_DebugCamera; }
	const Shared<SceneRenderer::Target> &GetTarget() const override { return SceneRenderer::GetMainTarget(); }

private:
	Shared<EditorCamera> m_DebugCamera;
	bool m_DebugCameraActivated = false;
};
}

