#ifndef MESH_H
#define MESH_H

#include "Angel.h"

class Mesh
{
    public:
        Mesh(vec4 * _vertices);
        Mesh()=default;
        void DivideTriangles(vec4 v1, vec4 v2, vec4 v3, int step, float min, float max);
        void Triangle(vec4 v1, vec4 v2, vec4 v3);
        void CreateMesh(vec4 * initialVertices, int step, float min, float max, int seed, bool arrangeBorders, int terrainSize);
        void SetColor();
        void AddHeight(vec4* v, float min, float max);
        void AddNormal(vec4 v, vec3 n);
        vec3 RandomLandPoint();
        static vec4 Lerp(vec4 start, vec4 end, float curr);
        static float randfrom(float min, float max);

        

        //Methods




        //Variables
        vec4 * points;
        vec4 ** heights;
        vec3 * normals;
        vec3 * vertexNormals;
        vec3 * verNormals;
        vec4 * colors;
        struct Material * materials;
        
        int numberOfPoints = 0;
        int numberOfVertices;
        int numberOfHeights = 0;
        int terrainSize;
        float min;
        float max;
        bool arrangeBorders;
        

};
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

#endif