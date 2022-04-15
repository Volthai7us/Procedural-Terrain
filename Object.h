#ifndef OBJECT_H
#define OBJECT_H

#include "Angel.h"


class Object
{
    public:
        Object()=default;

        //Methods
        void Set(vec3 move, vec3 scale, vec3 rotate, int size, vec4 * points, vec3 * normals, vec4 * colors);

        //Variables
        Object * parent;
        vec3 move;
        vec3 scale;
        vec3 rotate;
        vec4 * points;
        vec3 * normals;
        vec4 * colors;
        int size;
};

#endif