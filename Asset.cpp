#include "Asset.h"

AssetLoader::AssetLoader() 
{

}

AssetLoader::~AssetLoader()
{
	clear();
}

void AssetLoader::clear()
{
	for (auto& elem : m_loadedAssets) {
		delete elem.second;
	}

	m_loadedAssets.clear();
}
