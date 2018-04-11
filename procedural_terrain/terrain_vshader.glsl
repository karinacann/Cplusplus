R"(
#version 330 core
uniform sampler2D noiseTex;

in vec3 vposition;
in vec2 vtexcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;
out vec3 fragPos;

///6
void main() {
    /// TODO: Get height h at uv
    float h = 0.0;

    uv = vtexcoord;//uv that gives the position of each vertex in texture space (i.e. one corner of the grid is [0,0] and the opposite corner is [1,1])

    //from github
    h=(texture(noiseTex,uv).r + 1.0f) / 4.0f;

    fragPos = vposition.xyz + vec3(0,0,h);

    gl_Position = P*V*M*vec4(vposition.x, vposition.y, vposition.z + h, 1.0);
}
)"
