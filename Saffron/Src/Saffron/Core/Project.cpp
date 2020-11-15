#include "SaffronPCH.h"

#include "Saffron/Core/FileIOManager.h"
#include "Saffron/Core/Project.h"
#include "Saffron/Scene/EditorScene.h"
#include "Saffron/Serialize/ProjectSerializer.h"

namespace Se
{

Project::Project(Filepath filepath)
{
	const auto invalidateThis = [this]
	{
		m_UUID = 0ull;
		m_Name = "";
		m_ProjectFilepath = "";
		m_SceneFilepaths = {};
		m_SceneCache = {};
	};

	if ( IsValidFilepath(filepath) )
	{
		ProjectSerializer serializer(*this);
		if ( !serializer.Deserialize(filepath) )
		{
			invalidateThis();
		}
	}
	else
	{
		invalidateThis();
	}
}

Project::Project(String name, DateTime lastOpened, Filepath projectFilepath, ArrayList<Filepath> sceneFilepaths) :
	m_Name(std::move(name)),
	m_LastOpened(lastOpened),
	m_ProjectFilepath(std::move(projectFilepath)),
	m_SceneFilepaths(std::move(sceneFilepaths))
{
}

const Filepath &Project::AddScene(Filepath filepath)
{
	m_SceneFilepaths.push_back(Move(filepath));
	return m_SceneFilepaths.back();
}

const Shared<EditorScene> &Project::AddCachedScene(const Shared<EditorScene> &editorScene)
{
	m_SceneCache.push_back(editorScene);
	return m_SceneCache.back();
}

Project::Project(UUID mUuid, String mName, DateTime mLastOpened, Filepath mProjectFilepath, ArrayList<Filepath> mSceneFilepaths)
	:
	m_UUID(mUuid),
	m_Name(std::move(mName)),
	m_LastOpened(mLastOpened),
	m_ProjectFilepath(std::move(mProjectFilepath)),
	m_SceneFilepaths(std::move(mSceneFilepaths))
{
}

const ArrayList<Shared<EditorScene>> &Project::GetSceneCache() const
{
	return m_SceneCache;
}

bool Project::IsValid() const
{
	const bool correctData = !m_Name.empty() && !m_ProjectFilepath.empty() && !m_SceneFilepaths.empty() && !m_SceneFilepaths.front().empty(); // Correct on data level
	bool correctOnDisk = FileIOManager::FileExists(m_ProjectFilepath);
	for ( const auto &sceneFilepath : m_SceneFilepaths )
	{
		if ( !FileIOManager::FileExists(sceneFilepath) )
		{
			correctOnDisk = false;
			break;
		}
	}
	return correctData && correctOnDisk;
}

bool Project::IsValidFilepath(const Filepath &filepath)
{
	return !filepath.empty() && filepath.extension() == ".spr" && FileIOManager::FileExists(filepath);
}
}