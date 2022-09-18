#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

#include "Asset.h"
#include "Texture.h"

class Shader : public IAsset 
{
public:

	void load(const std::string& filePath) override;
	GLuint getProgram();

	void setUniform(const std::string name, bool value);
	void setUniform(const std::string name, int value);
	void setUniform(const std::string name, float value);
	void setUniform(const std::string name, const glm::vec3& value);
	void setUniform(const std::string name, const glm::vec2& value);
	void setUniform(const std::string name, const glm::vec4& value);
	void setUniform(const std::string name, const glm::mat3& value);
	void setUniform(const std::string name, const glm::mat4& value);
	void setTexture(const std::string name, Texture& texture);

private:

	void releaseProgram();

	void loadVertex(const std::string shaderPath);
	void loadFragment(const std::string shaderPath);
	void loadGeometry(const std::string shaderPath);

	void linkProgram();

	struct TextureUniform {
		int textureIndex;
		GLuint offset;
	};

	GLint program_ = -1;
	std::map<const std::string, GLuint> uniformOffset_;
	std::map<const std::string, TextureUniform> textureUniformOffset_;

	GLint vertexShaderID_ = -1;
	GLint fragmentShaderID_ = -1;
	GLint geometryShaderID_ = -1;

	int textureCount = 0;
};

