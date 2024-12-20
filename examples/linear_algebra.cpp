#include "linear_algebra.h"
/*
* This transforms a matrix based on the input translation [a b c]
*/
mat4 create_matrix_transform(vec3 translation)
{
    mat4 matrix;
    //r^4 identity matrix
    // {1 0 0 a} {x} goes | ^->              {x + a}
    // {0 1 0 b}*{y}      | |      gives us  {y + b}
    // {0 0 1 c} {z}       v->               {z + c}
    // {0 0 0 1} {1}                           {1}
    matrix.entries[0] = 1.0f; 
    matrix.entries[1] = 0.0f; 
    matrix.entries[2] = 0.0f; 
    matrix.entries[3] = 0.0f; 

    matrix.entries[4] = 0.0f; 
    matrix.entries[5] = 1.0f;
    matrix.entries[6] = 0.0f;
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = 0.0f;
    matrix.entries[9] = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = translation.entries[0];
    matrix.entries[13] = translation.entries[1];
    matrix.entries[14] = translation.entries[2];
    matrix.entries[15] = 1.0f;
    return matrix;
}
/*
* This rotates a matrix around the Z axis.
*/
mat4 create_z_rotation(float angle)
{
    //degrees to radians
    angle = angle * PI / 180.0f;
    float cos = cosf(angle);
    float sin = sinf(angle);
    mat4 matrix;
    matrix.entries[0] = cos;
    matrix.entries[1] = sin;
    matrix.entries[2] = 0.0f;
    matrix.entries[3] = 0.0f;

    matrix.entries[4] = -sin;
    matrix.entries[5] = cos;
    matrix.entries[6] = 0.0f;
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = 0.0f;
    matrix.entries[9] = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = 0.0f;
    matrix.entries[13] = 0.0f;
    matrix.entries[14] = 0.0f;
    matrix.entries[15] = 1.0f;
    return matrix;
}

mat4 create_model_transform(vec3 pos, float angle)
{
    //degrees to radians
    angle = angle * PI / 180.0f;
    float cos = cosf(angle);
    float sin = sinf(angle);
    mat4 matrix;
    matrix.entries[0] = cos;
    matrix.entries[1] = sin;
    matrix.entries[2] = 0.0f;
    matrix.entries[3] = 0.0f;

    matrix.entries[4] = -sin;
    matrix.entries[5] = cos;
    matrix.entries[6] = 0.0f;
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = 0.0f;
    matrix.entries[9] = 0.0f;
    matrix.entries[10] = 1.0f;
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = pos.entries[0];
    matrix.entries[13] = pos.entries[1];
    matrix.entries[14] = pos.entries[2];
    matrix.entries[15] = 1.0f;
    return matrix;
}

mat4 create_look_at(vec3 from, vec3 to)
{
    vec3 global_up = {0.0f, 0.0f, 1.0f};

    vec3 f = {
        to.entries[0] - from.entries[0],
        to.entries[1] - from.entries[1],
        to.entries[2] - from.entries[2],
    };
    
    f = normalize(f);

    vec3 r = normalize(cross(f, global_up));
    vec3 u = normalize(cross(r, f));

    mat4 matrix;
    matrix.entries[0] = r.entries[0];
    matrix.entries[1] = u.entries[0];
    matrix.entries[2] = -f.entries[0];
    matrix.entries[3] = 0.0f;

    matrix.entries[4] = r.entries[1];
    matrix.entries[5] = u.entries[1];
    matrix.entries[6] = -f.entries[1];
    matrix.entries[7] = 0.0f;

    matrix.entries[8] = r.entries[2];
    matrix.entries[9] = u.entries[2];
    matrix.entries[10] = -f.entries[2];
    matrix.entries[11] = 0.0f;

    matrix.entries[12] = -dot(r, from);
    matrix.entries[13] = -dot(u, from);
    matrix.entries[14] = dot(f, from);
    matrix.entries[15] = 1.0f;
    return matrix;

}

float dot(vec3 u, vec3 v)
{
    return u.entries[0] * v.entries[0] + 
           u.entries[1] * v.entries[1] + 
           u.entries[2] * v.entries[2];
}

vec3 normalize(vec3 v)
{
    vec3 u;
    float magnitude = sqrtf(dot(v,v));

    u.entries[0] = v.entries[0] / magnitude;
    u.entries[1] = v.entries[1] / magnitude;
    u.entries[2] = v.entries[2] / magnitude;
    return u;
}

vec3 cross(vec3 u, vec3 v)
{
    vec3 w;
    w.entries[0] = u.entries[1] * v.entries[2] - u.entries[2] * v.entries[1];    
    w.entries[1] = -(u.entries[0] * v.entries[2] - u.entries[2] * v.entries[0]);
    w.entries[2] = u.entries[0] * v.entries[1] - u.entries[1] * v.entries[0];
    return w;    
}

mat4 create_perspective_projection(float FOVy, float aspect, float near, float far)
{
    FOVy = FOVy * PI / 360.0f; //half of FOV
    float tan = tanf(FOVy);
    float n = -near;
    float f = -far;

    mat4 matrix;
    for (int i = 0; i < 16; i++)
    {
        matrix.entries[i] = 0.0f;
    }

    matrix.entries[0] = 1.0f / (aspect * tan);
    matrix.entries[5] = 1.0f / tan;
    matrix.entries[10] = -(n + f) / (n - f);
    matrix.entries[11] = -1.0f;
    matrix.entries[14] = 2 * n * f / (n - f);

    return matrix;
}