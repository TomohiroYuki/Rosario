#pragma once
#include <map>
#include <string>
#include <memory>
#include "Utility.h"
#include "Object\3D\WirePrimitive.h"

class StaticMesh;
class PrimitiveBase;
class BOX;
class ResourceManager
{

	ResourceManager() = default;
	~ResourceManager() = default;
public:
	// Singleton
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	void Initialize()
	{
		box = std::make_shared<BOX>();
		sphere = std::make_shared<SPHERE>();
	}
	
	std::map<std::string, std::shared_ptr<StaticMesh> > staticmesh_map;

	std::map<std::string, std::shared_ptr<PrimitiveBase> > primitive_map;


public:
	std::shared_ptr<BOX> box;
	std::shared_ptr<SPHERE> sphere;


	F_INLINE bool AddToResourceMap(std::string name, std::shared_ptr<StaticMesh>& in_staticmesh)
	{
		if (staticmesh_map.count(name) == 0)
		{
			staticmesh_map.insert(std::make_pair(name, in_staticmesh));
			return true;
		}
		return false;
	}
	F_INLINE std::shared_ptr<StaticMesh> FindStaticMesh(std::string name)
	{
		if (staticmesh_map.count(name) != 0)
		{
			return staticmesh_map[name];
		}

		return nullptr;
	}

	F_INLINE bool AddToResourceMap(std::string name, std::shared_ptr<PrimitiveBase>& in_primitive)
	{
		if (staticmesh_map.count(name) == 0)
		{
			primitive_map.insert(std::make_pair(name, in_primitive));
			return true;
		}
		return false;
	}
	F_INLINE std::shared_ptr<PrimitiveBase> FindPrimitive(std::string name)
	{
		if (primitive_map.count(name) != 0)
		{
			return primitive_map[name];
		}

		return nullptr;
	}



	static ResourceManager* getInstance()
	{
		static ResourceManager inst;			// privateなコンストラクタを呼び出す
		return &inst;
	}
};
#define RESOURCEM (ResourceManager::getInstance())