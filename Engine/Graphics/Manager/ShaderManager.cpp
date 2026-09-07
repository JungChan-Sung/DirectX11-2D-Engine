#include "ShaderManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Exception/COMException.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Config/ResourceConfig.h"
#include "Engine/Message/Error/ResourceManagerErrorMessage.h"
#include "Engine/Graphics/Constant/ShaderJsonConstants.h"
#include "Engine/Graphics/Factory/Shader/ShaderFactoryManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			ShaderManager::ShaderManager()
			{
			}

			ShaderManager::~ShaderManager()
			{
				Release();
			}

			bool ShaderManager::Initialize(ID3D11Device* device, BufferManager* bufferManager)
			{
				// 셰이더 데이터 파일 열기
				std::ifstream shaderDataFile(ResourceConfig::Instance()->ShaderDataFilePath());
				if (!shaderDataFile.is_open()) // shaderDataFile.is_open == false
				{
					CommonUtility::Log::ErrorMessage(Message::Error::SHADER_DATA_FILE_OPEN_FAILURE);
					return false;
				}

				// 셰이더 데이터 파일 Json 파싱
				nlohmann::json shaderDataJson;
				shaderDataJson = nlohmann::json::parse(shaderDataFile);

				Data::ShaderData* shaderData;
				for (const auto& [shaderGroupName, shaderGroup] : shaderDataJson.items())
				{
					for (const auto& [shaderName, shader] : shaderGroup.items())
					{
						// 스프라이트 데이터 생성
						shaderData = new Data::ShaderData;

						// 스프라이트 데이터 구조화
						shaderData->path = shader[Constant::SHADER_PATH].get<std::string>();

						// 스프라이트 데이터 삽입
						shaderDataGroupTable[shaderGroupName][shaderName] = shaderData;
					}
				}

				shaderDataJson.clear();
				shaderDataFile.close();

				try
				{
					// ==== 셰이더 생성 ====
					HRESULT hr = CreateShaders(device, bufferManager);
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_SHADERS_FAILURE);
					// ========================
				}
				catch (CommonUtility::COMException& exception)
				{
					CommonUtility::Log::ErrorMessage(exception);
					return false;
				}

				return true;
			}

			void ShaderManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(shaders);
				CommonUtility::SafeMapContainerRelease(pixelShaders);
				CommonUtility::SafeMapContainerRelease(vertexShaders);

				CommonUtility::SafeNestedMapContainerRelease(shaderDataGroupTable);
			}

			Resource::Shader* ShaderManager::GetShader(Type::ShaderType shaderType) const
			{
				return shaders.at(shaderType);
			}

			HRESULT ShaderManager::CreateShaders(ID3D11Device* device, BufferManager* bufferManager)
			{
				Type::ShaderType shaderType;
				Factory::ShaderFactoryManager shaderFactoryManager;
				Resource::VertexShader* vertexShader;
				Resource::PixelShader* pixelShader;
				Resource::Shader* shader;
				for (int i = 0; i < static_cast<int>(Type::ShaderType::END); ++i)
				{
					shaderType = static_cast<Type::ShaderType>(i);

					// 정점 셰이더 생성
					vertexShader = shaderFactoryManager.CreateVertexShader(device, bufferManager, shaderDataGroupTable, shaderType);
					if (!vertexShader) // vertexShader == nullptr
						return E_FAIL;

					vertexShaders.insert(std::make_pair(shaderType, vertexShader));

					// 픽셀 셰이더 생성
					pixelShader = shaderFactoryManager.CreatePixelShader(device, bufferManager, shaderDataGroupTable, shaderType);
					if (!pixelShader) // pixelShader == nullptr
						return E_FAIL;

					pixelShaders.insert(std::make_pair(shaderType, pixelShader));

					// 셰이더 통합 및 생성
					shader = new Resource::Shader(vertexShader, pixelShader);
					if (!shader) // shader == nullptr
						return E_FAIL;

					shaders.insert(std::make_pair(shaderType, shader));
				}

				return S_OK;
			}
		} // Manager
	} // Graphics
} // Engine