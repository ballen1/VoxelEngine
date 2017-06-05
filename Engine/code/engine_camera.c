#include "engine_camera.h"
#include "engine_math.h"

mat4f_t GetPerspectiveProjection(PerspectiveProjection* Perspective)
{
    mat4f_t ProjectionMatrix;

    float AspectRatio = Perspective->Width / Perspective->Height;
    float TanFactor = tanf((DEG2RAD*Perspective->VerticalFov) / 2.0);

    ProjectionMatrix.mat[0] = 1.0 / (AspectRatio * TanFactor);
    ProjectionMatrix.mat[1] = 0.0f;
    ProjectionMatrix.mat[2] = 0.0f;
    ProjectionMatrix.mat[3] = 0.0f;

    ProjectionMatrix.mat[4] = 0.0f;
    ProjectionMatrix.mat[5] = (1.0 / TanFactor);
    ProjectionMatrix.mat[6] = 0.0f;
    ProjectionMatrix.mat[7] = 0.0f;

    ProjectionMatrix.mat[8] = 0.0f;
    ProjectionMatrix.mat[9] = 0.0f;
    ProjectionMatrix.mat[10] = (Perspective->Near + Perspective->Far) /
				 (Perspective->Near - Perspective->Far);
    ProjectionMatrix.mat[11] = -1.0f;

    ProjectionMatrix.mat[12] = 0.0f;
    ProjectionMatrix.mat[13] = 0.0f;
    ProjectionMatrix.mat[14] = (2*Perspective->Far*Perspective->Near) /
	                       (Perspective->Near - Perspective->Far);
    ProjectionMatrix.mat[15] = 0.0f;

    return ProjectionMatrix;
}

mat4f_t GetGeneralView(GeneralCamera* Cam)
{
    mat4f_t ViewMat;

    vec3f_t w = NormalizeVec3f(SubtractVec3f(Cam->LookAt, Cam->Origin));
    vec3f_t u = NormalizeVec3f(CrossProductVec3f(Cam->Up, w));
    vec3f_t v = CrossProductVec3f(w, u);

    ViewMat.mat[0] = u.x;
    ViewMat.mat[1] = v.x;
    ViewMat.mat[2] = w.x;
    ViewMat.mat[3] = 0.0f;

    ViewMat.mat[4] = u.y;
    ViewMat.mat[5] = v.y;
    ViewMat.mat[6] = w.y;
    ViewMat.mat[7] = 0.0f;

    ViewMat.mat[8] = u.z;
    ViewMat.mat[9] = v.z;
    ViewMat.mat[10] = w.z;
    ViewMat.mat[11] = 0.0f;

    ViewMat.mat[12] = -DotProductVec3f(u, Cam->Origin);
    ViewMat.mat[13] = -DotProductVec3f(v, Cam->Origin);
    ViewMat.mat[14] = -DotProductVec3f(w, Cam->Origin);
    ViewMat.mat[15] = 1.0f;

    return ViewMat;

}

mat4f_t GetFirstPersonView(FirstPersonCamera* Cam)
{
    mat4f_t ViewMat;

    float PitchCos = cos(DEG2RAD * Cam->Pitch);
    float PitchSin = sin(DEG2RAD * Cam->Pitch);
    float YawCos = cos(DEG2RAD * Cam->Yaw);
    float YawSin = sin(DEG2RAD * Cam->Yaw);

    vec3f_t u = Vec3f(YawCos, 0.0f, YawSin);
    vec3f_t v = Vec3f(PitchSin * YawSin, PitchCos, -(PitchSin * YawCos));
    vec3f_t w = Vec3f(-(PitchCos * YawSin), PitchSin, PitchCos * YawCos);

    ViewMat.mat[0] = u.x;
    ViewMat.mat[1] = v.x;
    ViewMat.mat[2] = w.x;
    ViewMat.mat[3] = 0.0f;

    ViewMat.mat[4] = u.y;
    ViewMat.mat[5] = v.y;
    ViewMat.mat[6] = w.y;
    ViewMat.mat[7] = 0.0f;

    ViewMat.mat[8] = u.z;
    ViewMat.mat[9] = v.z;
    ViewMat.mat[10] = w.z;
    ViewMat.mat[11] = 0.0f;

    ViewMat.mat[12] = -DotProductVec3f(u, Cam->Position);
    ViewMat.mat[13] = -DotProductVec3f(v, Cam->Position);
    ViewMat.mat[14] = -DotProductVec3f(w, Cam->Position);
    ViewMat.mat[15] = 1.0f;

    return ViewMat;
}

vec3f_t FPCamFront(FirstPersonCamera* Cam)
{
    vec3f_t Front;

    Front.x = (cos(DEG2RAD * Cam->Pitch)*sin(DEG2RAD * Cam->Yaw));
    Front.y = -sin(DEG2RAD * Cam->Pitch);
    Front.z = -(cos(DEG2RAD * Cam->Pitch) * cos(DEG2RAD * Cam->Yaw));

    return NormalizeVec3f(Front);

}

vec3f_t FPCamRight(vec3f_t Front)
{
    vec3f_t Up = Vec3f(0.0f, 1.0f, 0.0f);

    return CrossProductVec3f(NormalizeVec3f(Front), Up);
}
