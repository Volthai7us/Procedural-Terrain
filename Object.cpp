#include "Object.h"

void Object::Set(vec3 move, vec3 scale, vec3 rotate, int size, vec4 * points, vec3 * normals, vec4 * colors){
    this->move = move;
    this->scale = scale;
    this->rotate = rotate;
    this->size = size;
    this->points = points;
    this->normals = normals;
    this->colors = colors;
}
