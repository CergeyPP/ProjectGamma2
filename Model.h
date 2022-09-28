#pragma once
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model :
    public IAsset, public IDrawable
{
public:

    Model();
    ~Model() override;

    void load(const std::string& filename) override;
    void draw(Shader* shader, const glm::mat4& ProjectionView = glm::mat4(1), const Transform& transform = Transform()) override;

    Mesh* getMesh(int index) {
        return m_meshes[index];
    }

private:

    std::string m_directory;

    std::vector<Mesh*> m_meshes;
    std::vector<Texture*> m_textures;

    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

    bool loadMaterialTexture(Material* material, aiMaterial* aiMat, aiTextureType texType, const std::string& uniformName);
};

