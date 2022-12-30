#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../UI/SpriteRenderer.h"

class GameObject {
public:
    GameObject(glm::vec2 pos, glm::vec2 size, const std::string& texture, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool solid = false);

    bool isSolid;

    void Draw(SpriteRenderer& renderer, glm::vec2 offset = glm::vec2(0.0f));

private:
    glm::vec2 _position;
    glm::vec2 _size;
    glm::vec2 _velocity;

    glm::vec3 _colour;

    std::string _textureName;
};