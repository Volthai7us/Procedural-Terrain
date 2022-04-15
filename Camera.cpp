#include "Camera.h"

Camera::Camera()
{
    eye = vec4(0.0, -radius, 0.0, 1.0);
    at = vec4(0.0, 0.0, 0.0, 1.0);
    up = vec4(0.0, 1.0, 0.0, 0.0);
    aspect = 1.0;
}

void Camera::setEye(float x, float y, float z){
    eye.x = x;
    eye.y = y;
    eye.z = z;
}

void Camera::setEye(vec4 _eye){
    eye = _eye;
}

void Camera::setEye(float theta, float phi){
    eye = vec4(radius * cos(phi * 3.14 / 180) * cos(theta * 3.14 / 180), 
               radius * sin(phi * 3.14 / 180),
               radius * cos(phi * 3.14 / 180) * sin(theta * 3.14 / 180), 
               1.0 );
}

void Camera::setAt(float theta, float phi, double distance){
    at = vec4(distance * cos(phi * 3.14 / 180) * cos(theta * 3.14 / 180), 
              distance * sin(phi * 3.14 / 180),
              distance * cos(phi * 3.14 / 180) * sin(theta * 3.14 / 180), 
               1.0 );
}

void Camera::setAt(float x, float y, float z){
    at.x = x;
    at.y = y;
    at.z = z;
}

void Camera::setAt(vec4 _at)
{
    at = _at;
}

void Camera::RotateAt(float theta)
{
    mat4 rotateY = mat4
        (
            cos(theta),  0.0, -sin(theta), 0.0,
            0.0,     1.0,  0.0,    0.0,
            sin(theta),  0.0,  cos(theta), 0.0,
            0.0,     0.0,  0.0,    1.0 
        );

    at = rotateY * at;
}

void Camera::RotateAtZ(float theta)
{
    mat4 rotateZ = mat4
        (
            1.0,  0.0,    0.0,    0.0,
		    0.0,  cos(theta), sin(theta), 0.0,
		    0.0,  -sin(theta), cos(theta), 0.0,
		    0.0,  0.0,    0.0,    1.0 
        );

    at = rotateZ * at;
}

void Camera::RotateX(float theta){
    mat4 rotateX = mat4
        ( 
            1.0,  0.0,    0.0,    0.0,
		    0.0,  cos(theta), sin(theta), 0.0,
		    0.0,  -sin(theta), cos(theta), 0.0,
		    0.0,  0.0,    0.0,    1.0 
        );

    eye = rotateX * eye;
}
    
void Camera::RotateY(float theta)
{
    mat4 rotateY = mat4
        (
            cos(theta),  0.0, -sin(theta), 0.0,
            0.0,     1.0,  0.0,    0.0,
            sin(theta),  0.0,  cos(theta), 0.0,
            0.0,     0.0,  0.0,    1.0 
        );

    eye = rotateY * eye;
}
    
void Camera::RotateZ(float theta)
{
    mat4 rotateZ = mat4
        (
            cos(theta), sin(theta), 0.0, 0.0,
            -sin(theta), cos(theta), 0.0, 0.0,
            0.0,    0.0,    1.0, 0.0,
            0.0,    0.0,    0.0, 1.0 
        );

    eye = rotateZ * eye;
}

void Camera::translate(float x, float y, float z){
    mat4 t = mat4
        (
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            x,   y,   z,   1.0
        );

    eye = t * eye;
}

void Camera::translate(vec4 direction)
{
    mat4 t = mat4
        (
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            direction.x,   direction.y,   direction.z,   1.0
        );

    eye = t * eye;
}


mat4 Camera::getLookAt(){

    vec4 z = normalize(eye-at);
    vec4 x = normalize(vec4(cross(up, z), 0.0));
    vec4 y = normalize(vec4(cross(z, x), 0.0));
    vec4 i = vec4(0.0, 0.0, 0.0, 1.0);
    mat4 c = mat4(x, y, z, i);
    mat4 t = Translate(-eye);
    mat4 r = c * t;
    
    return r;
}

mat4 Camera::getProjection(){
    float top = tan(fovy*3.14/180/2) * near;
    float right = top * aspect;

    mat4 projection = mat4
        (
            near/right, 0, 0, 0,
            0, near/top, 0, 0,
            0, 0, -(far + near)/(far - near), -1,
            0, 0, -(2*far*near)/(far - near), 0
        );

    return projection;
}

void Camera::setAspect(float _aspect)
{
    aspect = _aspect;
}
