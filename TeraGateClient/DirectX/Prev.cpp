#include"Prev.h"
int RANDOM_E(int exceptSize, int size)
{
    int loopSeed = 0;
    int result   = RANDOM_S(0, size);
    if(abs(result) < exceptSize)
    {
        int extra = exceptSize - abs(result);
        if(result<0)
            result -= abs(extra);
        else if(0<result)
            result += abs(extra);
    }
    return result+RANDOM_S(0, 100);
}
void infoD3DXMATRIX(D3DXMATRIX mtx)
{
    printf("¦£%.2f %.2f %.2f %.2f\n", mtx._11, mtx._12, mtx._13, mtx._14);
    printf("¦¢%.2f %.2f %.2f %.2f\n", mtx._21, mtx._22, mtx._23, mtx._24);
    printf("¦¢%.2f %.2f %.2f %.2f\n", mtx._31, mtx._32, mtx._33, mtx._34);
    printf("¦¦%.2f %.2f %.2f %.2f\n", mtx._41, mtx._42, mtx._43, mtx._44);
}
void infoD3DXMATRIX(D3DXMATRIX mtx, char* desc)
{
    printf("¦£ %.2f %.2f %.2f %.2f %s\n", mtx._11, mtx._12, mtx._13, mtx._14, desc);
    printf("¦¢ %.2f %.2f %.2f %.2f\n", mtx._21, mtx._22, mtx._23, mtx._24);
    printf("¦¢ %.2f %.2f %.2f %.2f\n", mtx._31, mtx._32, mtx._33, mtx._34);
    printf("¦¦ %.2f %.2f %.2f %.2f\n", mtx._41, mtx._42, mtx._43, mtx._44);
}
void infoD3DXVECTOR3(D3DXVECTOR3 v)
{
    printf("[ %f    %f    %f\n", v.x, v.y, v.z);
}
void infoD3DXVECTOR3(D3DXVECTOR3 v, char* desc)
{
    printf("[ %f    %f    %f %s\n", v.x, v.y, v.z, desc);
}