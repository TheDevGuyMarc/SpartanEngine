/*
Copyright(c) 2016 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//= INCLUDES ==========================
#include "Socket.h"
#include "Scene.h"
#include "Timer.h"
#include "../Pools/GameObjectPool.h"
#include "../IO/Log.h"
#include "../Components/MeshRenderer.h"
#include "../Graphics/Renderer.h"
#include "../Loading/ModelLoader.h"
//=====================================

//= NAMESPACES =====
using namespace std;
//==================

Socket::Socket(Scene* scene, Renderer* renderer, Input* input, Timer* timer, ModelLoader* modelLoader, PhysicsEngine* physics, TexturePool* texturePool, GraphicsDevice* graphicsDevice)
{
	m_scene = scene;
	m_renderer = renderer;
	m_timer = timer;
	m_modelLoader = modelLoader;
	m_physics = physics;
	m_texturePool = texturePool;
	m_graphicsDevice = graphicsDevice;
	m_input = input;
}

Socket::~Socket()
{
}

//= IO ========================================================================
void Socket::SetLogger(ILogger* logger)
{
	Log::SetLogger(logger);
}

void Socket::LoadModel(string path)
{
	m_modelLoader->Load(path, new GameObject());
}

ImageLoader* Socket::GetImageLoader()
{
	return &ImageLoader::GetInstance();
}
//==============================================================================

//= GRAPHICS ===================================================================
void Socket::SetViewport(int width, int height) const
{
	m_renderer->SetResolution(width, height);
}
//==============================================================================

//= MISC =======================================================================
void Socket::Update()
{
	// update time
	m_timer->Update();

	// update input
	m_input->Update();

	// update physics
	m_physics->Update();

	// update gaemeobjects
	GameObjectPool::GetInstance().Update();

	// update scene
	m_scene->Update();
}

void Socket::Render()
{
	m_renderer->Render();
}

void Socket::SetPhysicsDebugDraw(bool enable)
{
	m_renderer->SetPhysicsDebugDraw(enable);
}

PhysicsDebugDraw* Socket::GetPhysicsDebugDraw()
{
	return m_physics->GetPhysicsDebugDraw();
}
//==============================================================================

//= GAMEOBJECTS ================================================================
vector<GameObject*> Socket::GetAllGameObjects()
{
	return GameObjectPool::GetInstance().GetAllGameObjects();
}

std::vector<GameObject*> Socket::GetRootGameObjects()
{
	return GameObjectPool::GetInstance().GetRootGameObjects();
}

GameObject* Socket::GetGameObjectByID(string gameObjectID)
{
	return GameObjectPool::GetInstance().GetGameObjectByID(gameObjectID);
}

int Socket::GetGameObjectCount()
{
	return GameObjectPool::GetInstance().GetGameObjectCount();
}

void Socket::DestroyGameObject(GameObject* gameObject)
{
	if (!gameObject)
		return;

	GameObjectPool::GetInstance().RemoveGameObject(gameObject);
}

bool Socket::GameObjectExists(GameObject* gameObject)
{
	if (!gameObject) 
		return false;

	bool exists = GameObjectPool::GetInstance().GameObjectExists(gameObject);

	return exists;
}
//==============================================================================

//= SCENE ======================================================================
bool Socket::SaveSceneToFile(string path)
{
	return m_scene->SaveToFile(path);
}

bool Socket::LoadSceneFromFile(string path)
{
	return m_scene->LoadFromFile(path);
}

void Socket::ClearScene()
{
	m_scene->Clear();
}
//==============================================================================

//= STATS ======================================================================
float Socket::GetFPS()
{
	return m_renderer->GetFPS();
}

int Socket::GetRenderedMeshesCount()
{
	return m_renderer->GetRenderedMeshesCount();
}

float Socket::GetDeltaTime()
{
	return m_timer->GetDeltaTimeMs();
}

float Socket::GetRenderTime()
{
	return m_renderer->GetRenderTimeMs();
}
//==============================================================================

void Socket::SetMaterialTexture(GameObject* gameObject, TextureType type, string texturePath)
{
	if (gameObject == nullptr)
	{
		LOG("Can't update material texture, the GameObject is null", Log::Warning);
		return;
	}

	if (!gameObject->HasComponent<MeshRenderer>())
	{
		LOG("Can't update material texture, the GameObject has no mesh renderer", Log::Warning);
		return;
	}

	Material* material = gameObject->GetComponent<MeshRenderer>()->GetMaterial();
	if (material)
	{
		// Get the texture from the texture pool
		Texture* texture = m_texturePool->GetTextureByPath(texturePath);

		// If it's not loaded yet, load it
		if (!texture)
			texture = m_texturePool->AddFromFile(texturePath, type);

		// Set it to the material
		material->SetTexture(texture->GetID());
	}
}
