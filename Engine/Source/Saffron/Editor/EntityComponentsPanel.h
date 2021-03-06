#pragma once

#include "Saffron/Editor/ScriptPanel.h"
#include "Saffron/Entity/Entity.h"
#include "Saffron/Rendering/Mesh.h"
#include "Saffron/Scene/Scene.h"

namespace Se
{
class EntityComponentsPanel : public Managed
{
public:
	explicit EntityComponentsPanel(const Shared<Scene>& context);

	void OnGuiRender();

	void SetContext(const Shared<Scene>& context);
	void SetSelectedEntity(Entity entity);

private:
	void OnGuiRenderProperties();
	void OnGuiRenderMaterial();
	void OnGuiRenderMeshDebug();

	void DrawComponents(Entity entity);
	static bool DrawVec3Control(const String& label, Vector3& value, float resetValue = 0.0f,
	                            float columnWidth = 100.0f);

private:
	Shared<Scene> _context;
	Entity _selectionContext;

	TreeMap<String, Shared<Texture2D>> _texStore;
};
}
