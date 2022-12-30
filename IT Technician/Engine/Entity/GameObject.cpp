#include "GameObject.h"

#include "../ResourceManager.h"

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, const std::string& texture, glm::vec3 colour, glm::vec2 velocity, bool solid) : isSolid(solid), _position(pos), _size(size), _velocity(velocity),  _colour(colour), _textureName(texture) {
}

void GameObject::Draw(SpriteRenderer& renderer, glm::vec2 offset)
{
    Texture2D texture = ResourceManager::GetTexture(this->_textureName);
    renderer.DrawSprite(texture, this->_position - offset, this->_size, 0.0f, this->_colour);
}