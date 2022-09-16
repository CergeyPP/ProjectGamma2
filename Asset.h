#pragma once
#include <string>
#include <map>
#include <functional>

class IAsset
{
public:

	virtual ~IAsset() {}
	virtual void load(const std::string& filePath) = 0;

};

class AssetLoader
{
public:

	AssetLoader();
	~AssetLoader();

	template<class Type>
	Type* getAsset(const std::string& filename) {
		std::string filePath = "Assets/" + filename;
		auto it = std::find_if(m_loadedAssets.cbegin(), m_loadedAssets.cend(), [&filePath](const std::pair<const std::string, IAsset*> elem) {
			Type* asset = dynamic_cast<Type*>(elem.second);

			return (asset != nullptr && elem.first == filePath);
			});
		if (it == m_loadedAssets.end()) {
			Type* newAsset = new Type;
			newAsset->load(filePath);
			m_loadedAssets[filePath] = newAsset;
		}

		return (Type*)m_loadedAssets[filePath];
	}

	template<class Type>
	void forEach(std::function<void(Type*)> func) {
		for (auto& elem : m_loadedAssets) {
			Type* asset = dynamic_cast<Type*>(elem.second);
			if (asset != nullptr) {
				func(asset);
			}
		}
	}

	void addRawAsset(const std::string& filePath, IAsset* asset) {
		m_loadedAssets[filePath] = asset;
	}

	void clear();

private:

	std::map<const std::string, IAsset*> m_loadedAssets;
	
};