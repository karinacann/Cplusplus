R"(
#version 330 core
uniform sampler2D noiseTex;

uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D water;

// The camera position
uniform vec3 viewPos;

in vec2 uv;
// Fragment position in world space coordinates
in vec3 fragPos;

out vec4 color;

///9.
void main() {

    // Directional light source
    vec3 lightDir = normalize(vec3(1,1,1));

    // Texture size in pixels
    ivec2 size = textureSize(noiseTex, 0);

    /// TODO: Calculate surface normal N
    /// HINT: Use textureOffset(,,) to read height at uv + pixelwise offset
    /// HINT: Account for texture x,y dimensions in world space coordinates (default f_width=f_height=5)
    vec3 A = vec3( uv.x + 1.0/size.x, uv.y, textureOffset(noiseTex, uv, ivec2(1,0)) );
    vec3 B = vec3( uv.x - 1.0/size.x, uv.y, textureOffset(noiseTex, uv, ivec2(-1,0)));
    vec3 C = vec3(  uv.x, uv.y + 1.0/size.y, textureOffset(noiseTex, uv, ivec2(0,1)));
    vec3 D = vec3( uv.x, uv.y - 1.0/size.y, textureOffset(noiseTex, uv, ivec2(0,-1)));
    vec3 N = normalize( cross(normalize(A-B), normalize(C-D)) );

    /// TODO: Texture according to height and slope
    /// HINT: Read noiseTex for height at uv
    vec3 c = vec3((texture(noiseTex, uv).r + 1.0f)/2.0f);
    /// TODO: Calculate ambient, diffuse, and specular lighting
    /// HINT: max(,) dot(,) reflect(,) normalize()

float maxNDotH = max(0.0,dot(N,lightDir));

 vec3 lambert = vec3(max(0.0, dot(N,lightDir)));
 vec3 specular = vec3(0.5,0.5,0.5)*(pow(maxNDotH,100));
 vec3 ambient = 0.5*vec3(0.6,0.3,0.3);

c += lambert+specular+ambient;


   // c = (N+ vec3(1.0)) / 2.0;
    color = vec4(c,1);
}
)"
