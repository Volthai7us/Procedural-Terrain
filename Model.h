#ifndef MODEL_H
#define MODEL_H
#include "Angel.h"

class Model
{
    
    public:

        Model(vec4* _vertices, vec4* _faces, int _numberOfVertices, int _numberOfFaces);
        Model()=default;
        vec4* vertices;
        vec4* faces;
        vec4* points;
        vec3* normals;
        vec4* colors;
        vec3 color;

        int numberOfVertices;
        int numberOfFaces;

        vec4 middlePoint;
        vec4 max;
        vec4 min;

        float scale;

        void Normalized();
        void SetPoints();
        void Reset();
        void ReadModel(char* fileName, vec3 color);
    //private:

        
};

#endif