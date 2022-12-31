#include "SpriteRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../ResourceManager.h"

SpriteRenderer::SpriteRenderer() {
	this->_InitRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->_quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, glm::vec4* texturePos)
{
    glm::vec4 textureCoordinates;
    if (texturePos == nullptr) {
        textureCoordinates = texture.GetDefaultCoords();
    }
    else {
        textureCoordinates = *texturePos;
    }

    texture.CalculateTextureCoords(&textureCoordinates);

	Shader* shader = ResourceManager::GetShader("sprite");
	shader->Use();

    // prepare transformations
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader->SetMatrix4("model", model);
    shader->SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, textureCoordinates.x, textureCoordinates.w,
        1.0f, 0.0f, textureCoordinates.y, textureCoordinates.z,
        0.0f, 0.0f, textureCoordinates.x, textureCoordinates.z,

        0.0f, 1.0f, textureCoordinates.x, textureCoordinates.w,
        1.0f, 1.0f, textureCoordinates.y, textureCoordinates.w,
        1.0f, 0.0f, textureCoordinates.y, textureCoordinates.z
    };

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(this->_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::_InitRenderData() {
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->_quadVAO);
    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(this->_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}