#include "Mesh.h"
#include <time.h>
#define EPSILON 0.000001

float Mesh::randfrom(float min, float max)
{
    float range = (max - min);
    float div = RAND_MAX / range;
    return min + (rand() / div);
}

vec4 Mesh::Lerp(vec4 start, vec4 end, float curr)
{
    return start + (end - start) * curr;
}

void Mesh::DivideTriangles(vec4 v1, vec4 v2, vec4 v3, int step, float min, float max)
{
    if(step > 0)
    {      
        vec4 a = v1/2 + v2/2;
        vec4 b = v2/2 + v3/2;
        vec4 c = v3/2 + v1/2;

        AddHeight(&a, min, max);
        AddHeight(&b, min, max);
        AddHeight(&c, min, max);
        
        DivideTriangles(v1, a, c, step-1, min/2, max/2);
        DivideTriangles(a, v2, b, step-1, min/2, max/2);
        DivideTriangles(a, b, c, step-1, min/2, max/2);
        DivideTriangles(c, b, v3, step-1, min/2, max/2);
    }
    else
    {
        Triangle(v1, v2, v3);
    }
}


void Mesh::Triangle(vec4 v1, vec4 v2, vec4 v3)
{
    points[numberOfPoints] = v1;
    SetColor();

    points[numberOfPoints] = v2;
    SetColor();
    
    points[numberOfPoints] = v3;
    SetColor();

    vec4 edge1 = points[numberOfPoints-3] - points[numberOfPoints-2];
    vec4 edge2 = points[numberOfPoints-1] - points[numberOfPoints-2];
    vec3 triNormal = normalize(cross(edge2, edge1));

    AddNormal(v1, triNormal);
    AddNormal(v2, triNormal);
    AddNormal(v3, triNormal);

    normals[numberOfPoints-3] += triNormal;
    normals[numberOfPoints-2] += triNormal;
    normals[numberOfPoints-1] += triNormal;
}

void Mesh::SetColor()
{
    //printf("%f\n", points[numberOfPoints].y);
    if(points[numberOfPoints].y > terrainSize * max)
    {
        colors[numberOfPoints] = Lerp(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.9, 0.9, 0.9, 1.0), points[numberOfPoints].y/(terrainSize * .3));
    }
    else if(points[numberOfPoints].y > 0)
    {
        colors[numberOfPoints] = Lerp(vec4(0.800, 0.800, 0.105, 1.0), vec4(0.600, 0.078, 0.003, 1.0), points[numberOfPoints].y/(terrainSize * .45));
    }
    else
    {
        colors[numberOfPoints] = Lerp(vec4(0.0672, 0.841, 0.960, 1.0), vec4(0.00, 0.0758, 0.910, 1.0), -points[numberOfPoints].y/(terrainSize *.5));
        points[numberOfPoints].y = 0;
    }
    numberOfPoints++;
}

void Mesh::CreateMesh(vec4 * initialVertices, int step, float _min, float _max, int seed, bool _arrangeBorders, int _terrainSize)
{
    arrangeBorders = _arrangeBorders;
    terrainSize = _terrainSize;
    max = _max;
    min = _min;
    min *= terrainSize;
    max *= terrainSize;
    int verticesNumber = pow(2, step+1) + 1;
    if(seed == -1)
    {
        seed = time(NULL);
    }
    printf("Seed: %d\n", seed);
    srand(seed);

    numberOfVertices = verticesNumber;
    points = new vec4[ (verticesNumber - 1) * (verticesNumber - 1) * 6];
    normals = new vec3[ (verticesNumber - 1) * (verticesNumber - 1) * 6];
    verNormals = new vec3[ (verticesNumber - 1) * (verticesNumber - 1) * 6];
    colors = new vec4[ (verticesNumber - 1) * (verticesNumber - 1) * 6];
    heights = new vec4*[verticesNumber];
    for(int i=0; i<verticesNumber; i++)
    {
        heights[i] = new vec4[verticesNumber];
    }

    vertexNormals = new vec3[verticesNumber * verticesNumber];

    DivideTriangles(initialVertices[0], initialVertices[3], initialVertices[4], step, min, max);
    DivideTriangles(initialVertices[0], initialVertices[4], initialVertices[1], step, min, max);

    DivideTriangles(initialVertices[1], initialVertices[4], initialVertices[5], step, min, max);
    DivideTriangles(initialVertices[1], initialVertices[5], initialVertices[2], step, min, max);

    DivideTriangles(initialVertices[3], initialVertices[6], initialVertices[7], step, min, max);
    DivideTriangles(initialVertices[3], initialVertices[7], initialVertices[4], step, min, max);

    DivideTriangles(initialVertices[4], initialVertices[7], initialVertices[8], step, min, max);
    DivideTriangles(initialVertices[4], initialVertices[8], initialVertices[5], step, min, max);
    
    for(int i=0; i<numberOfPoints; i++)
    {
        int x = (points[i].x + terrainSize) / (terrainSize * 2) * (numberOfVertices - 1);
        int z = (points[i].z + terrainSize) / (terrainSize * 2) * (numberOfVertices - 1);
        verNormals[i] = normalize(vertexNormals[x + verticesNumber * z]);
    }
}

void Mesh::AddHeight(vec4* v, float min, float max)
{
    int x = (v->x + terrainSize) / (terrainSize * 2) * (numberOfVertices - 1);
    int z = (v->z + terrainSize) / (terrainSize * 2) * (numberOfVertices - 1);

    if(arrangeBorders && x == numberOfVertices - 1 || x == 0 || z == 0 || z == numberOfVertices - 1)
    {
        heights[x][z] = vec4(v->z, 0, v->z, 1.0);
    }

    else if(heights[x][z].w == 0)
    {
        heights[x][z] = vec4(v->z, v->y + randfrom(min, max), v->z, 1.0);
    }
    
    v->y = heights[x][z].y;
}

void Mesh::AddNormal(vec4 v, vec3 n)
{
    int x = (v.x + terrainSize) / (terrainSize * 2) * (numberOfVertices - 1);
    int z = (v.z + terrainSize) / (terrainSize * 2) * (numberOfVertices - 1);

    vertexNormals[x + z * numberOfVertices] += n;
}

vec3 Mesh::RandomLandPoint()
{
    vec4 randPoint = vec4(0.0, -1.0, 0.0, 0.0);
    printf("Number: %d\n", numberOfPoints);
    while(randPoint.y <= 0)
    {
        int index = rand()%numberOfPoints;
        printf("Index: %d\n", index);
        randPoint = points[index];
    }

    printf("%f %f %f\n", randPoint.x, randPoint.y, randPoint.z);


    return (vec3)(randPoint.x, randPoint.y, randPoint.z);
}
