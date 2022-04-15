#include "Model.h"
#include <string.h>
#include <math.h>

Model::Model(vec4* _vertices, vec4* _faces, int _numberOfVertices, int _numberOfFaces)
{
    vertices = _vertices;
    faces = _faces;
    numberOfVertices = _numberOfVertices;
    numberOfFaces = _numberOfFaces;
    min = vec4(10000, 10000, 10000, 1.0);
    max = vec4(-10000, -10000, -10000, 1.0);
}

void Model::Normalized()
{
    middlePoint /= numberOfVertices;
    if(max.x > max.y && max.x > max.z) scale = max.x;
    else if(max.y > max.z && max.y > max.z) scale = max.y;
    else if(max.z > max.y && max.z > max.x) scale = max.z;
}

void Model::SetPoints()
{
    points = new vec4[numberOfFaces*3];
    normals = new vec3[numberOfFaces*3];
    colors = new vec4[numberOfFaces*3];
    
    for(int i=0; i<numberOfFaces; i++)
    {
        vec4 face = faces[i];
        vec4 vertexX = vertices[(int) face.x];
        vec4 vertexY = vertices[(int) face.y];
        vec4 vertexZ = vertices[(int) face.z]; 
        points[i*3] = vertexX;
        points[i*3+1] = vertexY;
        points[i*3+2] = vertexZ;

        normals[i*3] = normalize(cross(vertexY-vertexX, vertexZ-vertexY));
        normals[i*3+1] = normalize(cross(vertexY-vertexX, vertexZ-vertexY));
        normals[i*3+2] = normalize(cross(vertexY-vertexX, vertexZ-vertexY));

        colors[i*3] = vec4(color, 1.0);
        colors[i*3+1] = vec4(color, 1.0);
        colors[i*3+2] = vec4(color, 1.0);
    }
}

void Model::Reset()
{
    delete[] faces;
    delete[] vertices;
    delete[] points;
    delete[] normals;

    middlePoint -= middlePoint;
    max -= max;
    min -= min;
}

void Model::ReadModel(char* fileName, vec3 color)
{
    this->color = color;
    FILE *p;
    char line[50];
    middlePoint -= middlePoint;
    max -= max;
    min -= min;

    p = fopen(fileName, "r");
    if(p != NULL)
    {
        if(fgets(line, 50, p) != NULL);
        if(fgets(line, 50, p) != NULL)
        {
            char *token = strtok(line, " ");
            numberOfVertices = atoi(token);
            token = strtok(NULL, " ");
            numberOfFaces = atoi(token);
        }
    
        vertices = new vec4[numberOfVertices];
        faces = new vec4[numberOfFaces];

        for(int i=0; i<numberOfVertices; i++){
            fgets(line, 50, p);
            char *token = strtok(line, " ");
            float x = atof(token);
            token = strtok(NULL, " ");
            float y = atof(token);
            token = strtok(NULL, " ");
            float z = atof(token);

            if(min.x > x) min.x = x;
            if(min.x > y) min.y = y;
            if(min.x > z) min.z = z;
            if(max.x < x) max.x = x;
            if(max.y < y) max.y = y;
            if(max.z < z) max.z = z;
            
            middlePoint += vec4(x, y, z, 0.0);
            
            vertices[i] = vec4(x, y, z, 1);
        }

        

        for(int i=0; i<numberOfFaces; i++){
            fgets(line, 50, p);
            char *token = strtok(line, " ");
            token = strtok(NULL, " ");
            float x = atof(token);
            token = strtok(NULL, " ");
            float y = atof(token);
            token = strtok(NULL, " ");
            float z = atof(token);

            faces[i] = vec4(x, y, z, 1);
        }
        fclose(p);

        Normalized();
        SetPoints();
    }
}