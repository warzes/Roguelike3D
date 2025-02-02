#include "stdafx.h"
//=============================================================================
#if defined(_MSC_VER)
#	pragma comment( lib, "3rdparty.lib" )
#	pragma comment( lib, "Engine.lib" )
#endif
//=============================================================================
void RequestExit() {}
//=============================================================================
#define TEXTURE_WIDTH  2
#define TEXTURE_HEIGHT 2

UINT texturedata[] = // 2x2 pixel checkerboard pattern, 0xAARRGGBB
{
0xffffffff, 0xff7f7f7f,
0xff7f7f7f, 0xffffffff,
};

float vertexdata[] = // pos.x, pos.y, pos.z, nor.x, nor.y, nor.z, tex.u, tex.v, col.r, col.g, col.b,
{
-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  0.0f,  0.973f,  0.480f,  0.002f,
1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 10.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f,  0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,  0.973f,  0.480f,  0.002f,
0.6f,  0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  2.0f,  0.973f,  0.480f,  0.002f,
-0.6f, -0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  8.0f,  0.973f,  0.480f,  0.002f,
0.6f, -0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  8.0f,  0.973f,  0.480f,  0.002f,
-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.973f,  0.480f,  0.002f,
-0.6f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f, 10.0f,  0.973f,  0.480f,  0.002f,
0.6f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f, 10.0f,  0.973f,  0.480f,  0.002f,
1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 10.0f, 10.0f,  0.973f,  0.480f,  0.002f,
1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 10.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.897f,  0.163f,  0.011f,
1.0f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f,  2.0f,  0.897f,  0.163f,  0.011f,
1.0f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f,  8.0f,  0.897f,  0.163f,  0.011f,
1.0f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f,  8.0f,  0.897f,  0.163f,  0.011f,
1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 10.0f,  0.897f,  0.163f,  0.011f,
1.0f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f, 10.0f,  0.897f,  0.163f,  0.011f,
1.0f, -1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f, 10.0f,  0.897f,  0.163f,  0.011f,
1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 10.0f, 10.0f,  0.897f,  0.163f,  0.011f,
1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 10.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,  0.612f,  0.000f,  0.069f,
-0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  2.0f,  0.612f,  0.000f,  0.069f,
0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  8.0f,  0.612f,  0.000f,  0.069f,
-0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  8.0f,  0.612f,  0.000f,  0.069f,
1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 10.0f,  0.612f,  0.000f,  0.069f,
0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f, 10.0f,  0.612f,  0.000f,  0.069f,
-0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f, 10.0f,  0.612f,  0.000f,  0.069f,
-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 10.0f, 10.0f,  0.612f,  0.000f,  0.069f,
-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 10.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f,  2.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f,  8.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f,  8.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 10.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f, 10.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f, 10.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 10.0f, 10.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f,  0.0f,  0.000f,  0.254f,  0.637f,
1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 10.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f,  0.6f,  0.0f,  1.0f,  0.0f,  2.0f,  2.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f,  0.6f,  0.0f,  1.0f,  0.0f,  8.0f,  2.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f, -0.6f,  0.0f,  1.0f,  0.0f,  2.0f,  8.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f, -0.6f,  0.0f,  1.0f,  0.0f,  8.0f,  8.0f,  0.000f,  0.254f,  0.637f,
-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  2.0f, 10.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  8.0f, 10.0f,  0.000f,  0.254f,  0.637f,
1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 10.0f, 10.0f,  0.000f,  0.254f,  0.637f,
-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  2.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  8.0f,  0.0f,  0.001f,  0.447f,  0.067f,
1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f, -0.6f,  0.0f, -1.0f,  0.0f,  2.0f,  2.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f, -0.6f,  0.0f, -1.0f,  0.0f,  8.0f,  2.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f,  0.6f,  0.0f, -1.0f,  0.0f,  2.0f,  8.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f,  0.6f,  0.0f, -1.0f,  0.0f,  8.0f,  8.0f,  0.001f,  0.447f,  0.067f,
-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  2.0f, 10.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  8.0f, 10.0f,  0.001f,  0.447f,  0.067f,
1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 10.0f, 10.0f,  0.001f,  0.447f,  0.067f,
-0.6f,  0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f, -0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f,  0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f,  0.6f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f, -0.6f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f, -0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f, -0.6f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f, -0.6f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
-0.6f,  0.6f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f,  0.6f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
1.0f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
1.0f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f,  0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f, -0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
-1.0f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-1.0f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
-0.6f,  1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f,  1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
-0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
-0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
0.6f, -1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
};

UINT indexdata[] =
{
0,   1,   9,   9,   8,   0,   1,   2,   5,   5,   4,   1,   6,   7,  10,  10,   9,   6,   2,   3,  11,  11,  10,   2,
12,  13,  21,  21,  20,  12,  13,  14,  17,  17,  16,  13,  18,  19,  22,  22,  21,  18,  14,  15,  23,  23,  22,  14,
24,  25,  33,  33,  32,  24,  25,  26,  29,  29,  28,  25,  30,  31,  34,  34,  33,  30,  26,  27,  35,  35,  34,  26,
36,  37,  45,  45,  44,  36,  37,  38,  41,  41,  40,  37,  42,  43,  46,  46,  45,  42,  38,  39,  47,  47,  46,  38,
48,  49,  57,  57,  56,  48,  49,  50,  53,  53,  52,  49,  54,  55,  58,  58,  57,  54,  50,  51,  59,  59,  58,  50,
60,  61,  69,  69,  68,  60,  61,  62,  65,  65,  64,  61,  66,  67,  70,  70,  69,  66,  62,  63,  71,  71,  70,  62,
72,  73,  74,  74,  75,  72,  76,  77,  78,  78,  79,  76,  80,  81,  82,  82,  83,  80,  84,  85,  86,  86,  87,  84,
88,  89,  90,  90,  91,  88,  92,  93,  94,  94,  95,  92,  96,  97,  98,  98,  99,  96, 100, 101, 102, 102, 103, 100,
104, 105, 106, 106, 107, 104, 108, 109, 110, 110, 111, 108, 112, 113, 114, 114, 115, 112, 116, 117, 118, 118, 119, 116,
120, 121, 122, 122, 123, 120, 124, 125, 126, 126, 127, 124, 128, 129, 130, 130, 131, 128, 132, 133, 134, 134, 135, 132,
136, 137, 138, 138, 139, 136, 140, 141, 142, 142, 143, 140, 144, 145, 146, 146, 147, 144, 148, 149, 150, 150, 151, 148,
152, 153, 154, 154, 155, 152, 156, 157, 158, 158, 159, 156, 160, 161, 162, 162, 163, 160, 164, 165, 166, 166, 167, 164,
};

struct float3 { float x, y, z; };
struct matrix { float m[4][4]; };

matrix operator*(const matrix& m1, const matrix& m2)
{
	return
	{
		m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
		m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
		m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
		m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],
		m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
		m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
		m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
		m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],
		m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
		m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
		m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
		m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
		m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
		m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
		m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
		m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3],
	};
}
//=============================================================================
int main()
{
	if (!DirectX::XMVerifyCPUSupport())
		return 1;

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return 1;

	LogSystemCreateInfo logCI;
	LogSystem log;
	log.Create(logCI);

	WindowSystemCreateInfo windowCI;
	WindowSystem window;
	window.Create(windowCI);

	RHISystemCreateInfo rhiCI;
	rhiCI.context.hwnd = window.GetHWND();
	rhiCI.context.width = window.GetWidth();
	rhiCI.context.height = window.GetHeight();
	RHISystem rhi;
	rhi.Create(rhiCI);

	auto device = rhi.GetContext().GetD3DDevice();
	auto devicecontext = rhi.GetContext().GetD3DContext();

	ID3DBlob* vertexshaderCSO;
	D3DCompileFromFile(L"gpu.hlsl", nullptr, nullptr, "VertexShaderMain", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
	ID3D11VertexShader* vertexshader;
	device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &vertexshader);

	ID3DBlob* pixelshaderCSO;
	D3DCompileFromFile(L"gpu.hlsl", nullptr, nullptr, "PixelShaderMain", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
	ID3D11PixelShader* pixelshader;
	device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &pixelshader);

	D3D11_INPUT_ELEMENT_DESC inputelementdesc[] = // maps to vertexdesc struct in gpu.hlsl via semantic names ("POS", "NOR", "TEX", "COL")
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // float3 position
		{ "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // float3 normal
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // float2 texcoord
		{ "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // float3 color
	};

	ID3D11InputLayout* inputlayout;
	device->CreateInputLayout(inputelementdesc, ARRAYSIZE(inputelementdesc), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &inputlayout);

	D3D11_RASTERIZER_DESC rasterizerdesc = {};
	rasterizerdesc.FillMode = D3D11_FILL_SOLID;
	rasterizerdesc.CullMode = D3D11_CULL_BACK;
	ID3D11RasterizerState* rasterizerstate;
	device->CreateRasterizerState(&rasterizerdesc, &rasterizerstate);

	D3D11_SAMPLER_DESC samplerdesc = {};
	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	ID3D11SamplerState* samplerstate;
	device->CreateSamplerState(&samplerdesc, &samplerstate);

	D3D11_DEPTH_STENCIL_DESC depthstencildesc = {};
	depthstencildesc.DepthEnable = TRUE;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS;
	ID3D11DepthStencilState* depthstencilstate;
	device->CreateDepthStencilState(&depthstencildesc, &depthstencilstate);

	struct Constants { matrix transform, projection; float3 lightvector; };

	D3D11_BUFFER_DESC constantbufferdesc = {};
	constantbufferdesc.ByteWidth = sizeof(Constants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ID3D11Buffer* constantbuffer;
	device->CreateBuffer(&constantbufferdesc, nullptr, &constantbuffer);

	D3D11_TEXTURE2D_DESC texturedesc = {};
	texturedesc.Width = TEXTURE_WIDTH;  // in xube.h
	texturedesc.Height = TEXTURE_HEIGHT; // in xube.h
	texturedesc.MipLevels = 1;
	texturedesc.ArraySize = 1;
	texturedesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // same as framebuffer(view)
	texturedesc.SampleDesc.Count = 1;
	texturedesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated
	texturedesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureSRD = {};
	textureSRD.pSysMem = texturedata; // in xube.h
	textureSRD.SysMemPitch = TEXTURE_WIDTH * sizeof(UINT); // 1 UINT = 4 bytes per pixel, 0xAARRGGBB

	ID3D11Texture2D* texture;
	device->CreateTexture2D(&texturedesc, &textureSRD, &texture);

	ID3D11ShaderResourceView* textureSRV;
	device->CreateShaderResourceView(texture, nullptr, &textureSRV);


	D3D11_BUFFER_DESC vertexbufferdesc = {};
	vertexbufferdesc.ByteWidth = sizeof(vertexdata);
	vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
	vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertexdata }; // in xube.h

	ID3D11Buffer* vertexbuffer;
	device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexbuffer);

	D3D11_BUFFER_DESC indexbufferdesc = {};
	indexbufferdesc.ByteWidth = sizeof(indexdata);
	indexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated
	indexbufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexbufferSRD = { indexdata }; // in xube.h

	ID3D11Buffer* indexbuffer;
	device->CreateBuffer(&indexbufferdesc, &indexbufferSRD, &indexbuffer);

	UINT stride = 11 * sizeof(float); // vertex size (11 floats: float3 position, float3 normal, float2 texcoord, float3 color)
	UINT offset = 0;


	float w = (float)window.GetWidth() / (float)window.GetHeight(); // width (aspect ratio)
	float h = 1.0f;                             // height
	float n = 1.0f;                             // near
	float f = 9.0f;                             // far

	float3 modelrotation = { 0.0f, 0.0f, 0.0f };
	float3 modelscale = { 1.0f, 1.0f, 1.0f };
	float3 modeltranslation = { 0.0f, 0.0f, 4.0f };

	while (!window.IsShouldClose())
	{
		window.PollEvent();
		rhi.Resize(window.GetWidth(), window.GetHeight());

		// update
		{
			matrix rotatex = { 1, 0, 0, 0, 0, (float)cos(modelrotation.x), -(float)sin(modelrotation.x), 0, 0, (float)sin(modelrotation.x), (float)cos(modelrotation.x), 0, 0, 0, 0, 1 };
			matrix rotatey = { (float)cos(modelrotation.y), 0, (float)sin(modelrotation.y), 0, 0, 1, 0, 0, -(float)sin(modelrotation.y), 0, (float)cos(modelrotation.y), 0, 0, 0, 0, 1 };
			matrix rotatez = { (float)cos(modelrotation.z), -(float)sin(modelrotation.z), 0, 0, (float)sin(modelrotation.z), (float)cos(modelrotation.z), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
			matrix scale = { modelscale.x, 0, 0, 0, 0, modelscale.y, 0, 0, 0, 0, modelscale.z, 0, 0, 0, 0, 1 };
			matrix translate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, modeltranslation.x, modeltranslation.y, modeltranslation.z, 1 };

			modelrotation.x += 0.005f;
			modelrotation.y += 0.009f;
			modelrotation.z += 0.001f;

			D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

			devicecontext->Map(constantbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
			{
				Constants* constants = (Constants*)constantbufferMSR.pData;

				constants->transform = rotatex * rotatey * rotatez * scale * translate;
				constants->projection = { 2 * n / w, 0, 0, 0, 0, 2 * n / h, 0, 0, 0, 0, f / (f - n), 1, 0, 0, n * f / (n - f), 0 };
				constants->lightvector = { 1.0f, -1.0f, 1.0f };
			}
			devicecontext->Unmap(constantbuffer, 0);
		}

		rhi.BeginFrame();

		devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devicecontext->IASetInputLayout(inputlayout);
		devicecontext->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
		devicecontext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);

		devicecontext->VSSetShader(vertexshader, nullptr, 0);
		devicecontext->VSSetConstantBuffers(0, 1, &constantbuffer);

		devicecontext->RSSetState(rasterizerstate);

		devicecontext->PSSetShader(pixelshader, nullptr, 0);
		devicecontext->PSSetShaderResources(0, 1, &textureSRV);
		devicecontext->PSSetSamplers(0, 1, &samplerstate);

		devicecontext->OMSetDepthStencilState(depthstencilstate, 0);
		devicecontext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		devicecontext->DrawIndexed(ARRAYSIZE(indexdata), 0, 0);

		rhi.EndFrame();
	}

	rhi.Destroy();
	window.Destroy();
	log.Destroy();
}
//=============================================================================S