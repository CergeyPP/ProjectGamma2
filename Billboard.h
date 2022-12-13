#pragma once
#include "Asset.h"
#include "Mesh.h"
#include "Texture.h"

#include <glm/glm.hpp>

class Billboard :
    public IAsset, public IDrawable
{
public:

    Billboard();
    ~Billboard();

    void load(const std::string& filename) override;

    void draw(Shader* shader = nullptr, const glm::mat4& ProjectionView = glm::mat4(1), const Transform& transform = Transform()) override;

    void setTexture(Texture* texture);

    glm::vec2 size;

private:

    Texture* m_renderTexture;
    Shader* m_shader;
};

