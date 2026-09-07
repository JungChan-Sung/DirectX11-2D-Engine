#include "Scene.h"
#include "CommonUtility/Utility/StringUtility.h"

namespace Engine
{
	namespace Scene
	{
		Scene::Scene(const std::wstring& sceneName) : name(sceneName)
		{
		}

		Scene::Scene(const std::string& sceneName)
			: name(CommonUtility::StringUtility::ConvertToWideString(sceneName))
		{
		}

		const std::wstring& Scene::GetSceneName() const
		{
			return name;
		}
	} // Scene
} // Engine