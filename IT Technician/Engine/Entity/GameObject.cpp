#include "GameObject.h"

#include "../ResourceManager.h"

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, std::string texture, glm::vec3 colour, glm::vec2 velocity, bool solid) : isSolid(solid), _position(pos), _size(size), _velocity(velocity),  _colour(colour), _textureName(texture) {
}

void GameObject::Draw(SpriteRenderer& renderer)
{
    Texture2D texture = ResourceManager::GetTexture(this->_textureName);
    renderer.DrawSprite(texture, this->_position, this->_size, 0.0f, this->_colour);
}