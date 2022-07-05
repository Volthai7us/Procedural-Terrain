#version 150 

in  vec4 vPosition;
in vec3 vNormal;
in vec4 vColor;

out vec4 color;

uniform vec3 move;
uniform vec3 scale;
uniform vec3 rotate;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 lightPosition;
vec4 lightAmbient = vec4(0.1, 0.1, 0.1, 1.0);;
vec4 lightDiffuse = vec4(1, 1, 1, 1.0);;
vec4 lightSpecular = vec4(0.1, 0.1, 0.1, 1.0);;


mat4 Translate(float x, float y, float z)
{
    mat4 t = mat4
        (
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            x,   y,   z,   1.0
        );

    return t;
}
mat4 Scale(float x, float y, float z)
{
    mat4 s = mat4
        (
            x,   0.0, 0.0, 0.0,
            0.0, y,   0.0, 0.0,
            0.0, 0.0, z,   0.0,
            0.0, 0.0, 0.0, 1.0
        );

    return s;
}

mat4 RotateX(float theta)
{   
    mat4 rotate = mat4
        ( 
            1.0,  0.0,         0.0,        0.0,
		    0.0,  cos(theta),  sin(theta), 0.0,
		    0.0, -sin(theta),  cos(theta), 0.0,
		    0.0,  0.0,         0.0,        1.0 
        );

    return rotate;
}

mat4 RotateY(float theta)
{
    mat4 rotate = mat4
        ( 
            cos(theta),  0.0, -sin(theta), 0.0,
            0.0,         1.0,  0.0,        0.0,
            sin(theta),  0.0,  cos(theta), 0.0,
            0.0,         0.0,  0.0,        1.0 
        );

    return rotate;
}

mat4 RotateZ(float theta){
    mat4 rotate = mat4
        ( 
            cos(theta), sin(theta), 0.0, 0.0,
           -sin(theta), cos(theta), 0.0, 0.0,
            0.0,        0.0,        1.0, 0.0,
            0.0,        0.0,        0.0, 1.0 
        );

    return rotate;
}

void main() 
{
    // Transform vertex  position into eye coordinates
    vec4 pos = ModelView * vPosition;
	
    vec3 L = normalize((lightPosition - pos).xyz); //assuming light position provided in camera coordinates
    vec3 E = normalize(-pos.xyz);
    vec3 H = normalize(L + E);

    vec4 diffuseProduct = lightDiffuse * vColor;
    vec4 specularProduct = lightSpecular * vColor;
    // Transform vertex normal into eye coordinates
    vec3 N = normalize(vec3(ModelView * vec4(vNormal,0.0)));

    // Compute terms in the illumination equation
    vec4 ambient = lightAmbient * vColor;

    float Kd = max(dot(L, N), 0.0);
    vec4  diffuse = Kd * diffuseProduct;

    float Ks = pow(max(dot(N, H), 0.0), 250);
    vec4  specular = Ks * specularProduct;
    
    if( dot(L, N) < 0.0 ) 
    {
	    specular = vec4(0.0, 0.0, 0.0, 1.0);
    } 

    gl_Position = Projection
                * ModelView
                * Translate(move.x, move.y, move.z)
                * Scale(scale.x, scale.y, scale.z) 
                * RotateX(rotate.x) 
                * RotateY(rotate.y) 
                * RotateZ(rotate.z)
                * vPosition;

    color = ambient + diffuse + specular;
    color.a = 1.0;
}
