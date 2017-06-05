#ifndef H_ENGINE_MATH
#define H_ENGINE_MATH

#include <math.h>

#define PI 3.14159265359
#define DEG2RAD 0.0174533

typedef struct
vec2f_t
{
    float x;
    float y;
} vec2f_t;

typedef struct
vec3f_t
{
    float x;
    float y;
    float z;
} vec3f_t;

typedef struct
vec4f_t
{
    float x;
    float y;
    float z;
    float w;
} vec4f_t;

typedef struct
mat3f_t
{

    float mat[9];

} mat3f_t;

typedef struct
mat4f_t
{

    float mat[16];

} mat4f_t;

typedef struct
quad_t
{
    vec3f_t x;
    vec3f_t y;
    vec3f_t w;
    vec3f_t h;
} quad_t;

inline
quad_t Quad(vec3f_t X, vec3f_t Y, vec3f_t W, vec3f_t H)
{
    quad_t quad;
    quad.x = X;
    quad.y = Y;
    quad.w = W;
    quad.h = H;

    return (quad);
}

inline
vec2f_t Vec2f(float x, float y)
{
    vec2f_t v2;
    v2.x = x;
    v2.y = y;
    return v2;
}

inline
vec3f_t Vec3f(float x, float y, float z)
{
    vec3f_t v3;
    
    v3.x = x;
    v3.y = y;
    v3.z = z;

    return v3;
}

inline
vec4f_t Vec4f(float x, float y, float z, float w)
{
    vec4f_t v4;
    v4.x = x;
    v4.y = y;
    v4.z = z;
    v4.w = w;

    return (v4);
}

inline
int Vec3f_Equals(vec3f_t V1, vec3f_t V2)
{
    if (V1.x == V2.x &&
	V1.y == V2.y &&
	V1.z == V2.z)
    {
	return (1);
    }

    return (0);
}

inline
mat3f_t Mat3f_Identity()
{
    mat3f_t mat;

    mat.mat[0] = 1.0f;
    mat.mat[1] = 0.0f;
    mat.mat[2] = 0.0f;

    mat.mat[3] = 0.0f;
    mat.mat[4] = 1.0f;
    mat.mat[5] = 0.0f;

    mat.mat[6] = 0.0f;
    mat.mat[7] = 0.0f;
    mat.mat[8] = 1.0f;

    return mat;
}

inline
mat4f_t Mat4f_Identity()
{
    mat4f_t mat;

    mat.mat[0] = 1.0f;
    mat.mat[1] = 0.0f;
    mat.mat[2] = 0.0f;
    mat.mat[3] = 0.0f;

    mat.mat[4] = 0.0f;
    mat.mat[5] = 1.0f;
    mat.mat[6] = 0.0f;
    mat.mat[7] = 0.0f;

    mat.mat[8] = 0.0f;
    mat.mat[9] = 0.0f;
    mat.mat[10] = 1.0f;
    mat.mat[11] = 0.0f;

    mat.mat[12] = 0.0f;
    mat.mat[13] = 0.0f;
    mat.mat[14] = 0.0f;
    mat.mat[15] = 1.0f;

    return mat;
}

inline
vec3f_t NegativeVec3f(vec3f_t v)
{
    v.x = -v.x;
    v.y = -v.y;
    v.z = -v.z;

    return v;
}

inline
float DotProductVec2f(vec2f_t v1, vec2f_t v2)
{
    return (v1.x*v2.x + v1.y*v2.y);
}

inline
float DotProductVec3f(vec3f_t v1, vec3f_t v2)
{
    return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

inline
vec3f_t AddVec3f(vec3f_t v1, vec3f_t v2)
{

    vec3f_t v3;

    v3.x = v2.x + v1.x;
    v3.y = v2.y + v1.y;
    v3.z = v2.z + v1.z;

    return v3;
}

inline
vec3f_t SubtractVec3f(vec3f_t v1, vec3f_t v2)
{
    vec3f_t v3;

    v3.x = v2.x - v1.x;
    v3.y = v2.y - v1.y;
    v3.z = v2.z - v1.z;

    return v3;
}

inline
vec3f_t MultiplyVec3f(vec3f_t v, float s)
{
    v.x *= s;
    v.y *= s;
    v.z *= s;

    return v;
}

inline
float MagnitudeVec3f(vec3f_t v)
{
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline
vec3f_t NormalizeVec3f(vec3f_t v)
{
    float vMag = MagnitudeVec3f(v);
    
    v.x /= vMag;
    v.y /= vMag;
    v.z /= vMag;

    return v;
}

inline
vec3f_t CrossProductVec3f(vec3f_t v1, vec3f_t v2)
{
    vec3f_t v3;

    v3.x = (v1.y * v2.z) - (v1.z * v2.y);
    v3.y = (v1.z * v2.x) - (v1.x * v2.z);
    v3.z = (v1.x * v2.y) - (v1.y * v2.x);

    return v3;
}

inline
mat4f_t TranslationMat4f(float x, float y, float z)
{
    mat4f_t trans = Mat4f_Identity();

    trans.mat[12] = x;
    trans.mat[13] = y;
    trans.mat[14] = z;

    return trans;
}

inline
mat4f_t ScalingMat4f(float x, float y, float z)
{
    mat4f_t scale = Mat4f_Identity();

    scale.mat[0] *= x;
    scale.mat[5] *= y;
    scale.mat[10] *= z;

    return scale;
}

inline
mat4f_t RotationMat4f(float deg, int x, int y, int z)
{
    
    mat4f_t rot = Mat4f_Identity();
    
    float rad = deg * DEG2RAD;

    if (x)
    {
	rot.mat[5] = cos(rad);
	rot.mat[6] = sin(rad);
	rot.mat[9] = -sin(rad);
	rot.mat[10] = cos(rad);
    }
    else if (y)
    {
	rot.mat[0] = cos(rad);
	rot.mat[2]= -sin(rad);
	rot.mat[8] = sin(rad);
	rot.mat[10] = cos(rad);
    }
    else if (z)
    {
	rot.mat[0] = cos(rad);
	rot.mat[1] = sin(rad);
	rot.mat[4] = -sin(rad);
	rot.mat[5] = cos(rad);
    }

    return rot;

}

inline
vec4f_t MultiplyVec4fByMat4f(vec4f_t V, mat4f_t M)
{
    vec4f_t Result;

    Result.x = M.mat[0]*V.x + M.mat[4]*V.y + M.mat[8]*V.z + M.mat[12]*V.w;
    Result.y = M.mat[1]*V.x + M.mat[5]*V.y + M.mat[9]*V.z + M.mat[13]*V.w;
    Result.z = M.mat[2]*V.x + M.mat[6]*V.y + M.mat[10]*V.z + M.mat[14]*V.w;
    Result.w = M.mat[3]*V.x + M.mat[7]*V.y + M.mat[11]*V.z + M.mat[15]*V.w;

    return (Result);
}


inline
mat4f_t MultiplyMat4f(mat4f_t mat1, mat4f_t mat2)
{
    mat4f_t mat3;

    mat3.mat[0] = mat1.mat[0]*mat2.mat[0] + mat1.mat[4]*mat2.mat[1] + 
	          mat1.mat[8]*mat2.mat[2] + mat1.mat[12]*mat2.mat[3];

    mat3.mat[4] = mat1.mat[0]*mat2.mat[4] + mat1.mat[4]*mat2.mat[5] + 
	          mat1.mat[8]*mat2.mat[6] + mat1.mat[12]*mat2.mat[7];

    mat3.mat[8] = mat1.mat[0]*mat2.mat[8] + mat1.mat[4]*mat2.mat[9] + 
	          mat1.mat[8]*mat2.mat[10] + mat1.mat[12]*mat2.mat[11];

    mat3.mat[12] = mat1.mat[0]*mat2.mat[12] + mat1.mat[4]*mat2.mat[13] + 
	          mat1.mat[8]*mat2.mat[14] + mat1.mat[12]*mat2.mat[15];

    mat3.mat[1] = mat1.mat[1]*mat2.mat[0] + mat1.mat[5]*mat2.mat[1] + 
	          mat1.mat[9]*mat2.mat[2] + mat1.mat[13]*mat2.mat[3];

    mat3.mat[5] = mat1.mat[1]*mat2.mat[4] + mat1.mat[5]*mat2.mat[5] + 
	          mat1.mat[9]*mat2.mat[6] + mat1.mat[13]*mat2.mat[7];

    mat3.mat[9] = mat1.mat[1]*mat2.mat[8] + mat1.mat[5]*mat2.mat[9] + 
	          mat1.mat[9]*mat2.mat[10] + mat1.mat[13]*mat2.mat[11];

    mat3.mat[13] = mat1.mat[1]*mat2.mat[12] + mat1.mat[5]*mat2.mat[13] + 
	          mat1.mat[9]*mat2.mat[14] + mat1.mat[13]*mat2.mat[15];

    mat3.mat[2] = mat1.mat[2]*mat2.mat[0] + mat1.mat[6]*mat2.mat[1] + 
	          mat1.mat[10]*mat2.mat[2] + mat1.mat[14]*mat2.mat[3];

    mat3.mat[6] = mat1.mat[2]*mat2.mat[4] + mat1.mat[6]*mat2.mat[5] + 
	          mat1.mat[10]*mat2.mat[6] + mat1.mat[14]*mat2.mat[7];

    mat3.mat[10] = mat1.mat[2]*mat2.mat[8] + mat1.mat[6]*mat2.mat[9] + 
	           mat1.mat[10]*mat2.mat[10] + mat1.mat[14]*mat2.mat[11];

    mat3.mat[14] = mat1.mat[2]*mat2.mat[12] + mat1.mat[6]*mat2.mat[13] + 
	           mat1.mat[10]*mat2.mat[14] + mat1.mat[14]*mat2.mat[15];

    mat3.mat[3] = mat1.mat[3]*mat2.mat[0] + mat1.mat[7]*mat2.mat[1] + 
	           mat1.mat[11]*mat2.mat[2] + mat1.mat[115]*mat2.mat[3];

    mat3.mat[7] = mat1.mat[3]*mat2.mat[4] + mat1.mat[7]*mat2.mat[5] + 
	           mat1.mat[11]*mat2.mat[6] + mat1.mat[115]*mat2.mat[7];

    mat3.mat[11] = mat1.mat[3]*mat2.mat[8] + mat1.mat[7]*mat2.mat[9] + 
	           mat1.mat[11]*mat2.mat[10] + mat1.mat[115]*mat2.mat[11];

    mat3.mat[15] = mat1.mat[3]*mat2.mat[12] + mat1.mat[7]*mat2.mat[13] + 
	           mat1.mat[11]*mat2.mat[14] + mat1.mat[15]*mat2.mat[15];

    return mat3;

}

inline
float VE_Fabs(float Num)
{
    return (Num >= 0 ? Num : -(Num)); 
}

inline
vec3f_t Vec3f_Fabs(vec3f_t Vec)
{
    Vec.x = VE_Fabs(Vec.x);
    Vec.y = VE_Fabs(Vec.y);
    Vec.z = VE_Fabs(Vec.z);

    return (Vec);
}

#endif
