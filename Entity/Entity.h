//
// Created by blakey on 01/12/23.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

class Entity
{

  private:
    double posX, posY, posZ, yaw, pitch; // Position of the entity
    unsigned int id;      // ID for selection & verification later on
    double height, width; // Hitbox param

  public:
    glm::vec3 getPosition();
    unsigned int getId();
    void setPosition(glm::vec3);
    void setSize(double height, double width);
    void setX(double x);
    void setY(double y);
    void setZ(double z);
};

#endif // ENTITY_H
