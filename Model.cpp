#include "Model.h"
#include "Application.h"

Model::Model()
{
    m_directory = "";
}

Model::~Model()
{
    if (!m_meshes.empty()) {
        for (auto& elem : m_meshes)
            delete elem;
        m_meshes.clear();
    }
    if (!m_textures.empty()) {
        for (auto& elem : m_textures)
            delete elem;
        m_textures.clear();
    }
}

void Model::load(const std::string& filename)
{
    m_directory = filename.substr(0, filename.find_last_of('/'));

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::draw(Shader* shader, const glm::mat4& ProjectionView, const Transform& transform)
{
    for (auto& mesh : m_meshes) {
        mesh->draw(shader, ProjectionView, transform);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* aimesh, const aiScene* scene)
{
    Mesh* mesh = new Mesh();
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (int i = 0; i < aimesh->mNumVertices; i++) {
        glm::vec3 pos(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z);
        glm::vec3 normal(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z);
        glm::vec2 texcoord(0, 0);
        if (aimesh->mTextureCoords[0]) {
            texcoord = glm::vec2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y);
        }
        vertices.push_back(Vertex(pos, normal, texcoord));
    }
    for (int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    mesh->init(vertices, indices);
    vertices.clear();
    indices.clear();
    m_meshes.push_back(mesh);

    if (aimesh->mMaterialIndex >= 0) {
        Material* material = new Material(Loader().getAsset<Shader>("DefaultLit.shader"));


        aiMaterial* mat = scene->mMaterials[aimesh->mMaterialIndex];
        if (!loadMaterialTexture(material, mat, aiTextureType_DIFFUSE, "material.Albedo")) {
            aiColor4D albedo;
            mat->Get(AI_MATKEY_COLOR_DIFFUSE, albedo);

            Texture* texture = new Texture();
            const unsigned char data[4] = {albedo.r * 255, albedo.g * 255, albedo.b * 255, albedo.a * 255};
            texture->generateTextureFromData(1, 1, data);

            material->texParam["material.Albedo0"] = texture;
            m_textures.push_back(texture);
        }
        if (!loadMaterialTexture(material, mat, aiTextureType_SPECULAR, "material.Specular")) {
            aiColor4D spec;
            mat->Get(AI_MATKEY_COLOR_SPECULAR, spec);

            Texture* texture = new Texture();
            const unsigned char data[4] = { spec.r * 255, spec.g * 255, spec.b * 255, spec.a * 255 };
            texture->generateTextureFromData(1, 1, data);

            material->texParam["material.Specular0"] = texture;
            m_textures.push_back(texture);
        }

        float shininess;
        mat->Get(AI_MATKEY_SHININESS, shininess);
        material->floatParam["material.shininess"] = shininess;
        std::cout << "Shininess : " << shininess << std::endl;

        mesh->setMaterial(material);
    }

    
}

bool Model::loadMaterialTexture(Material* material, aiMaterial* aiMat, aiTextureType texType, const std::string& uniformName)
{
    if (aiMat->GetTextureCount(texType) == 0) return false;
    for (unsigned int i = 0; i < aiMat->GetTextureCount(texType); i++)
    {
        aiString str;
        aiMat->GetTexture(texType, i, &str);
        material->texParam[uniformName + std::to_string(i)] = 
            Loader().getAsset<Texture>(m_directory.substr(m_directory.find_first_of('/') + 1, m_directory.find_last_of('/')) + "/" + str.C_Str());
    }
    return true;
}
