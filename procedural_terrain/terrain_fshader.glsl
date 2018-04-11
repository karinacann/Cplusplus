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
    const float f_width = 5.0f;
    const float f_height = 5.0f;
    float h;


    vec3 A,B,C,D,N,co;

    A = vec3( fragPos.x + f_width/size.x, fragPos.y, textureOffset(noiseTex, uv, ivec2(f_width,0)) );
    B = vec3( fragPos.x - f_width/size.x, fragPos.y, textureOffset(noiseTex, uv, ivec2(-f_width,0)));
    C = vec3(  fragPos.x, fragPos.y + f_height/size.y, textureOffset(noiseTex, uv, ivec2(0,f_height)));
    D = vec3( fragPos.x, fragPos.y - f_height/size.y, textureOffset(noiseTex, uv, ivec2(0,-f_height)));
    N = normalize( cross(normalize(A-B), normalize(C-D)) );
/// TODO: Texture according to height and slope
    float slope = 1.0f-N.y;

    co = vec3(0,0,0);

    h=(texture(noiseTex,uv).r+1.0)/2.0;

    if (h < 0.5 ){
        co = vec3(texture(grass, uv));
    } else if (h >=0.7){
         co = vec3(texture(snow, uv));
    }else {
     co = vec3(texture(rock, uv).xyz);
    }
    /// HINT: Read noiseTex for height at uv
    /// TODO: Calculate ambient, diffuse, and specular lighting
    /// HINT: max(,) dot(,) reflect(,) normalize()

    float maxNDotH;
    maxNDotH = max(0.0,dot(N,lightDir));

     vec3 lambert, specular, ambient;
     lambert = vec3(max(0.0, dot(N,lightDir)));
     specular = vec3(0.5,0.5,0.5)*(pow(maxNDotH,10));
     ambient = 0.5*vec3(0.6,0.3,0.3);

//c += lambert+specular+ambient;
   // c = (N+ vec3(1.0)) / 2.0;
    color = vec4(co,1);
}
)"
