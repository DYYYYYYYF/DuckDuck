#pragma once

#include "Resources/Resource.hpp"
#include <string>

class IResourceLoader {
public:
	IResourceLoader() : Id(INVALID_ID), CustomType(nullptr){}
	virtual ~IResourceLoader() {
		Id = INVALID_ID;
		CustomType = nullptr;
	}

public:
	virtual bool Load(const std::string& name, void* params, Resource* resource) = 0;
	virtual void Unload(Resource* resouce) = 0;

public:
	uint32_t Id;
	ResourceType Type;
	const char* CustomType;
	std::string TypePath;

};
