//
// Created by blakey on 01/12/23.
//

#include "Entity.h"


unsigned Entity::getId() {
    return this->id;
}

glm::vec3 Entity::getPosition() {
    glm::vec3 position = glm::vec3(this->posX, this->posY, this->posZ);
    return position;
}

void Entity::setPosition(glm::vec3 pos) {
    this->posX = pos.x;
    this->posY = pos.y;
    this->posZ = pos.z;
}

void Entity::setSize(double height, double width) {
    this->height = height;
    this->width = width;
}

void Entity::setX(double x) {
    this->posX = x;
}

void Entity::setY(double y) {
    this->posY = y;
}

void Entity::setZ(double z) {
    this->posZ = z;
}




