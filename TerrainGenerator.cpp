#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Camera.h"
#include "Angel.h"
#include "Mesh.h"
#include "Object.h"
#include "Model.h"


void display(void);
void init(void);
void timer(int ms);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void autoBinder();
void autoDrawer();
void updateUniforms(int);

GLuint vao[2];
GLuint buffer[2];
Object objects[2];

GLuint modelViewAdress;
GLuint projectionAdress;
GLuint lightLocationAdress;
GLuint moveAdress;
GLuint scaleAdress;
GLuint rotateAdress;

vec4 light_position = vec4(0.0, -5.0, 0.0, 1.0);

enum Shading
{
    flat,
    gouraud
};

Shading shading = flat;

enum CameraMode
{
    c,
    t,
    w
};

CameraMode cameraMode = c;

int prevMouseX;
int prevMouseY;
bool first = true;

Camera mainCamera = Camera();
Camera planeCamera = Camera();
Camera * currentCamera = &mainCamera;
int terrainSize;
vec4 planePosition;
vec4 planeDirection;
float planeSpeed = 0.005;

Mesh mesh;
Model plane;

int main(int argc, char **argv )
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Terrain Generator");


    terrainSize = atoi(argv[6]);
    planePosition = vec4(0, terrainSize, -terrainSize, 1.0);
    planeDirection = vec4(0, 0, 1, 1.0);

    vec4 initialVertices[9] = {
        vec4(-terrainSize, 0.0, -terrainSize, 1.0),     vec4(0.0, 0.0, -terrainSize, 1.0),          vec4(terrainSize, 0.0, -terrainSize, 1.0),
        vec4(-terrainSize, 0.0, 0.0, 1.0),              vec4(0.0, 0.0, 0.0, 1.0),                   vec4(terrainSize, 0.0, 0.0, 1.0),
        vec4(-terrainSize, 0.0, terrainSize, 1.0),      vec4(0.0, 0.0, terrainSize, 1.0),           vec4(terrainSize, 0.0, terrainSize, 1.0)
    };


    mesh.CreateMesh(initialVertices, atoi(argv[1]), atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), terrainSize);
    plane.ReadModel((char *) "plane.off", vec3(0.300, 0.258, 0.258));

    objects[0].Set(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), mesh.numberOfPoints, mesh.points, mesh.normals, mesh.colors);
    objects[1].Set(vec3(planePosition.x, planePosition.y, planePosition.z), vec3(.005, .005, .005), vec3(-90 * 3.14 / 180, 0, 3.14), plane.numberOfFaces*3, plane.points, plane.normals, plane.colors);

    glewExperimental = GL_TRUE;
    glewInit();
    mainCamera.radius = terrainSize * 2;
    mainCamera.far = terrainSize * 30;
    mainCamera.setEye(mainCamera.theta, mainCamera.phi);
    planeCamera.fovy = 50;
    planeCamera.far = terrainSize * 10;
    planeCamera.phi = -20;
    planeCamera.theta = 90;

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutTimerFunc(1000.0/60.0, timer, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);  
    glCullFace(GL_BACK);

    init();

    glutMainLoop();

    return 0;
}

void autoDrawer(void)
{
    for(int i=0; i<sizeof(vao)/sizeof(GLuint); i++)
    {
        glBindVertexArray(vao[i]);
        updateUniforms(i);
        glDrawArrays(GL_TRIANGLES, 0, objects[i].size);
    }
}

void autoBinder(void)
{
    for(int i=0; i<sizeof(vao)/sizeof(GLuint); i++)
    {
        glGenVertexArrays(1, &vao[i]);
        glBindVertexArray(vao[i]);

        glGenBuffers(1, &buffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
        glBufferData(GL_ARRAY_BUFFER, objects[i].size * sizeof(vec4) * 2 + objects[i].size * sizeof(vec3), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, objects[i].size * sizeof(vec4), objects[i].points);
        glBufferSubData(GL_ARRAY_BUFFER, objects[i].size * sizeof(vec4), objects[i].size * sizeof(vec3), objects[i].normals);
        glBufferSubData(GL_ARRAY_BUFFER, objects[i].size * sizeof(vec4) + objects[i].size * sizeof(vec3), objects[i].size * sizeof(vec4), objects[i].colors);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3 , GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * objects[i].size));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4 , GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(vec4) + sizeof(vec3)) * objects[i].size) );
    }
}

void updateUniforms(int i)
{
    glUniformMatrix4fv(modelViewAdress, 1, GL_TRUE, (*currentCamera).getLookAt());
    glUniformMatrix4fv(projectionAdress, 1, GL_TRUE, (*currentCamera).getProjection());

    glUniform4fv(lightLocationAdress, 1, light_position);
    glUniform3fv(moveAdress, 1, objects[i].move);
    glUniform3fv(scaleAdress, 1, objects[i].scale);
    glUniform3fv(rotateAdress, 1, objects[i].rotate);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    planePosition += vec3(planeDirection.x, 0, planeDirection.z) * planeSpeed;
    objects[1].move = vec3(planePosition.x, planePosition.y, planePosition.z);

    if(cameraMode == c)
    {
        planeCamera.setEye(planePosition.x, planePosition.y, planePosition.z);
        planeCamera.setAt(planePosition.x + planeDirection.x * terrainSize, 0, planePosition.z + planeDirection.z * terrainSize);
    }
    else if(cameraMode == t)
    {
        float x1 = 0.3;
        float x2 = terrainSize;
        planeCamera.setEye(planePosition.x + planeDirection.x * -x1, planePosition.y + .5, planePosition.z + planeDirection.z * -x1);
        planeCamera.setAt(planePosition.x + planeDirection.x * x2 , 0, planePosition.z + planeDirection.z * x2);
    }

    autoDrawer();


    glClearColor(0.0672, 0.841, 0.960, 1.0);
    glutSwapBuffers();
}

void init(void){
    autoBinder();
    
    GLuint program = InitShader("vs.glsl", "fs.glsl");
    glUseProgram(program);

    modelViewAdress = glGetUniformLocation(program, "ModelView");
    projectionAdress = glGetUniformLocation(program, "Projection");
    lightLocationAdress = glGetUniformLocation(program, "lightPosition");
    moveAdress = glGetUniformLocation(program, "move");
    scaleAdress = glGetUniformLocation(program, "scale");
    rotateAdress = glGetUniformLocation(program, "rotate");

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void timer(int ms)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}

void motion(int x, int y){

    if(!first && currentCamera == &mainCamera)
    {
        int diffX = prevMouseX - x;
        int diffY = prevMouseY - y;
        float sens = 0.3;

        mainCamera.phi += diffY * sens;
        mainCamera.theta += diffX * sens;

        if(mainCamera.phi > 88.0)
        {
            mainCamera.phi = 88.0;
        }
        else if(mainCamera.phi < 0)
        {
            mainCamera.phi = 0;
        }

        mainCamera.setEye(mainCamera.theta, mainCamera.phi);
    }
    
    prevMouseX = x;
    prevMouseY = y;

    first = false;
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        first = true;
    }
    if(state == GLUT_UP && currentCamera == &mainCamera)
    {
        if(button == 4)
        {
            mainCamera.radius += mainCamera.radius/10;
        }
        else if(button == 3)
        {
            mainCamera.radius -= mainCamera.radius/10;
        }

        mainCamera.setEye(mainCamera.theta, mainCamera.phi);
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if(key == 'F' || key == 'f')
    {
        if(shading == flat)
        {
            shading = gouraud;
            objects[0].normals = mesh.verNormals;
            autoBinder();
        }
        else if(shading == gouraud)
        {
            shading = flat;
            objects[0].normals = mesh.normals;
            autoBinder();
        }
    }
    else if(key == 'C' || key == 'c')
    {
        cameraMode = c;
        currentCamera = &planeCamera;
    }
    else if(key == 'T' || key == 't')
    {
        cameraMode = t;
        currentCamera = &planeCamera;
    }
    else if(key == 'W' || key == 'w')
    {
        cameraMode = t;
        currentCamera = &mainCamera;
    }
    else if(key == 'A' || key == 'a')
    {
        planeSpeed += 0.001;
    }
    else if(key == 'D' || key == 'd')
    {
        planeSpeed += -.001;
    }
    else if(key == 'Q' || key == 'q')
    {
        exit(-1);
    }
}

void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            objects[1].rotate += vec3(0.0, 0.0, 0.03);
            planeDirection = RotateY(0.03 * 180 / 3.14) * planeDirection;
            break;
        case GLUT_KEY_RIGHT:
            objects[1].rotate += vec3(0.0, 0.0, -0.03);
            planeDirection = RotateY(-0.03 * 180 / 3.14) * planeDirection;
            break;
    }
}