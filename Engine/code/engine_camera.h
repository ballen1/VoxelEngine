#ifndef H_ENGINE_CAMERA
#define H_ENGINE_CAMERA

#include "engine_math.h"

typedef struct
GeneralCamera
{

    vec3f_t Origin;
    vec3f_t LookAt;
    vec3f_t Up;

} GeneralCamera;

typedef struct
FirstPersonCamera
{
    
    vec3f_t Position;
    float Yaw;
    float Pitch;

} FirstPersonCamera;

typedef struct
PerspectiveProjection
{

    float Width;
    float Height;
    
    float VerticalFov;

    float Near;
    float Far;

} PerspectiveProjection;

mat4f_t GetPerspectiveProjection(PerspectiveProjection* Perspective); 

mat4f_t GetGeneralView(GeneralCamera* Cam);

mat4f_t GetFirstPersonView(FirstPersonCamera* Cam);

vec3f_t FPCamFront(FirstPersonCamera* Cam);

vec3f_t FPCamRight(vec3f_t Front);

#endif
