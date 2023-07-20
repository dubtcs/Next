
#version 460

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 projectionViewMatrices[6];

out vec4 pWorldPosition;

void main()
{
    for(int i = 0; i < 6; i++)
    {
        gl_Layer = i;
        for (int vert = 0; vert < 3; vert++)
        {
            pWorldPosition = gl_in[vert].gl_Position; // HAHAHAA I HAD THIS SET TO [i] INSTEAD OF [vert] ISNT THAT SO FUNNY AND NOT AT ALL FRUSTRATING
            gl_Position = projectionViewMatrices[i] * pWorldPosition;
            EmitVertex();
        }
        EndPrimitive();
    }
}
