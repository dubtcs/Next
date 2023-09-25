
#version 460

// bitmasks
#define CHECKBIT(var, index) ((var >> index) & 1U) == 1

#define PRIM_BIT_NORMALS 0
#define PRIM_BIT_TANGENTS 1

// lighting
#define MAX_LIGHTS 10

#define LIGHT_POINT 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_SPOT 2
#define LIGHT_AMBIENT 3
