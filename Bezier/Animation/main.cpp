#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>
///THIS FILE IS NOW WORKING COPY OF ASSIGNMENT 3

using namespace OpenGP;

const int width=720, height=720;
typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

std::unique_ptr<GPUMesh> bezierLine;
std::vector<Vec2> controlPoints;
std::vector<Vec2> bezierLinePairs;
const int maxDepth=10;


const char* fb_vshader =
#include "fb_vshader.glsl"
;
const char* fb_fshader =
#include "fb_fshader.glsl"
;
const char* quad_vshader =
#include "quad_vshader.glsl"
;
const char* quad_fshader =
#include "quad_fshader.glsl"
;
const char* line_vshader =
#include "line_vshader.glsl"
;
const char* line_fshader =
#include "line_fshader.glsl"
;

std::unique_ptr<Shader> lineShader;

const float SpeedFactor = 1;
const int slow_rot_period = 30;

void init();
void quadInit(std::unique_ptr<GPUMesh> &quad);
void loadTexture(std::unique_ptr<RGBA8Texture> &texture, const char* filename);
void drawScene(float timeCount);


std::unique_ptr<GPUMesh> quad;
std::unique_ptr<Shader> quadShader;
std::unique_ptr<Shader> fbShader;


std::unique_ptr<RGBA8Texture> moon;
std::unique_ptr<RGBA8Texture> sky;
std::unique_ptr<RGBA8Texture> rocket;

std::unique_ptr<Framebuffer> fb;
std::unique_ptr<RGBA8Texture> c_buf;

Transform TRS3 = Transform::Identity();

void drawBezier(Vec2 A, Vec2 B, Vec2 C, Vec2 D, int currDepth ){
   /*
    * deCasteljau algorithm
    *   AB, BC CD are all lines. Algorithm computes all midpoints of the three lines,
    * and draws two lines between those midpoints.
    * Then, it compute the newly generated lines' midpoints, draws a line between those
    * midpoints, and compute the final midpoint.
    * The algorithm recursively calls itself for each side of the curve
    */
    if (currDepth==maxDepth){
       bezierLinePairs.push_back(A);
       bezierLinePairs.push_back(D);
       return;
      }

    Vec2 midpointAB = Vec2((B.x()+A.x())/2, (B.y()+A.y())/2);
    Vec2 midpointBC = Vec2((C.x()+B.x())/2, (C.y()+B.y())/2);
    Vec2 midpointCD = Vec2((D.x()+C.x())/2, (D.y()+C.y())/2);

    Vec2 midpointABC = Vec2((midpointBC.x()+midpointAB.x())/2, (midpointBC.y()+midpointAB.y())/2);
    Vec2 midpointBCD = Vec2((midpointCD.x()+midpointBC.x())/2, (midpointCD.y()+midpointBC.y())/2);

    //new control point
    Vec2 midpointABCD = Vec2((midpointBCD.x()+midpointABC.x())/2, (midpointBCD.y()+midpointABC.y())/2);

    drawBezier(A, midpointAB, midpointABC, midpointABCD, currDepth+1);
    drawBezier(midpointABCD, midpointBCD, midpointCD, D, currDepth+1);
}

void makeAnimationPath(){

    controlPoints = std::vector<Vec2>();

    controlPoints.push_back(Vec2(-0.5f,0.6f));
    controlPoints.push_back(Vec2( 0.5f, -0.5f));
    controlPoints.push_back(Vec2( 0.6f, -0.6f));

    controlPoints.push_back(Vec2(0.5f, 0.5f));





    bezierLinePairs = std::vector<Vec2>();
    bezierLinePairs = std::vector<Vec2>();

    drawBezier(controlPoints[0],controlPoints[1],controlPoints[2],controlPoints[3],0);

    std::vector<unsigned int> bezierIndices = std::vector<unsigned int>();

    int bezierSize = bezierLinePairs.size();
    for (int i = 0; i < bezierSize; i++){
        bezierIndices.push_back(i);
    }
}

void drawRocket(float timeCount){
    makeAnimationPath();
    glEnable(GL_BLEND);
    float t = timeCount * SpeedFactor;

    int bezierSize= bezierLinePairs.size();
    float xcoord;
    float ycoord;

    //calculate modulo
    int x= t*700;
    int m = floor(x/ bezierSize);
    int r = m * bezierSize;
    int i = x - r;

    xcoord = bezierLinePairs[i].x();
    ycoord = bezierLinePairs[i].y();

    Transform TRS = Transform::Identity();
    TRS *= Eigen::Translation3f(xcoord, ycoord, 0);
    TRS *= Eigen::AngleAxisf(M_PI/6, Eigen::Vector3f::UnitZ());
    TRS *= Eigen::AlignedScaling3f(0.0001*i, 0.0001*i, 1);
   TRS *= Eigen::AlignedScaling3f(0.2*t,0.2*t,1);

    quadShader->bind();
    quadShader->set_uniform("M", TRS.matrix());

    glActiveTexture(GL_TEXTURE0);

    rocket->bind();
    quadShader->set_uniform("tex", 0);
    quad->set_attributes(*quadShader);
    quad->draw();
    rocket->unbind();
    quadShader->unbind();

    glDisable(GL_BLEND);
}

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );

    fbShader = std::unique_ptr<Shader>(new Shader());
    fbShader->verbose = true;
    fbShader->add_vshader_from_source(fb_vshader);
    fbShader->add_fshader_from_source(fb_fshader);
    fbShader->link();

    quadShader = std::unique_ptr<Shader>(new Shader());
    quadShader->verbose = true;
    quadShader->add_vshader_from_source(quad_vshader);
    quadShader->add_fshader_from_source(quad_fshader);
    quadShader->link();

    quadInit(quad);

    //init path
    controlPoints = std::vector<Vec2>();
    controlPoints.push_back(Vec2(-0.6f,0.0f));
    controlPoints.push_back(Vec2(-0.5f, -0.5f));
    controlPoints.push_back(Vec2( 0.5f, 0.5f));
    controlPoints.push_back(Vec2( 0.6f, 0.0f));

    bezierLinePairs = std::vector<Vec2>();
    drawBezier(controlPoints[0],controlPoints[1],controlPoints[2],controlPoints[3],0);
    std::vector<unsigned int> bezierIndices = std::vector<unsigned int>();
    int bezierSize = bezierLinePairs.size();
    for (int i = 0; i < bezierSize; i++){
        bezierIndices.push_back(i);
    }

    bezierLine = std::unique_ptr<GPUMesh>(new GPUMesh());
    bezierLine->set_vbo<Vec2>("vposition", bezierLinePairs);
    bezierLine->set_triangles(bezierIndices);

    loadTexture(moon, "moon.png");
    loadTexture(sky, "sky.png");
  //  loadTexture(earth, "earth.png");
    loadTexture(rocket, "rocket.png");
}

void quadInit(std::unique_ptr<GPUMesh> &quad) {
    quad = std::unique_ptr<GPUMesh>(new GPUMesh());
    std::vector<Vec3> quad_vposition = {
        Vec3(-1, -1, 0),
        Vec3(-1,  1, 0),
        Vec3( 1, -1, 0),
        Vec3( 1,  1, 0)
    };
    quad->set_vbo<Vec3>("vposition", quad_vposition);
    std::vector<unsigned int> quad_triangle_indices = {
        0, 2, 1, 1, 2, 3
    };
    quad->set_triangles(quad_triangle_indices);
    std::vector<Vec2> quad_vtexcoord = {
        Vec2(0, 0),
        Vec2(0,  1),
        Vec2( 1, 0),
        Vec2( 1,  1)
    };
    quad->set_vtexcoord(quad_vtexcoord);
}

void loadTexture(std::unique_ptr<RGBA8Texture> &texture, const char *filename) {
    // Used snippet from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    //decode
    unsigned error = lodepng::decode(image, width, height, filename);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

    // unfortunately they are upside down...lets fix that
    unsigned char* row = new unsigned char[4*width];
    for(int i = 0; i < int(height)/2; ++i) {
        memcpy(row, &image[4*i*width], 4*width*sizeof(unsigned char));
        memcpy(&image[4*i*width], &image[image.size() - 4*(i+1)*width], 4*width*sizeof(unsigned char));
        memcpy(&image[image.size() - 4*(i+1)*width], row, 4*width*sizeof(unsigned char));
    }
    delete row;

    texture = std::unique_ptr<RGBA8Texture>(new RGBA8Texture());
    texture->upload_raw(width, height, &image[0]);
}

void drawScene(float timeCount)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float t = timeCount * SpeedFactor;
    Transform TRS = Transform::Identity();
    //background.draw(TRS.matrix());
    quadShader->bind();
    quadShader->set_uniform("M", TRS.matrix());
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to the active unit for drawing

    sky->bind();
    // Set the shader's texture uniform to the index of the texture unit we have
    // bound the texture to
    quadShader->set_uniform("tex", 0);
    quad->set_attributes(*quadShader);
    quad->draw();
    sky->unbind();

    TRS *= Eigen::Translation3f(0.5f, 0.5f, 0);
    Transform TRS2 = TRS;
    TRS3 = TRS;
    TRS *= Eigen::AngleAxisf((t*M_PI/ slow_rot_period), Eigen::Vector3f::UnitZ());
    TRS *= Eigen::AlignedScaling3f(0.4f, 0.4f, 1);

    quadShader->bind();
    quadShader->set_uniform("M", TRS.matrix());
    // Make texture unit 0 active
    glActiveTexture(GL_TEXTURE0);

    moon->bind();
    quadShader->set_uniform("tex", 0);
    quad->set_attributes(*quadShader);
    quad->draw();
    moon->unbind();
    quadShader->unbind();

    //Second layer of moon
    TRS2 *= Eigen::AngleAxisf(-(t*M_PI)/slow_rot_period*1.2, Eigen::Vector3f::UnitZ());
    TRS2 *= Eigen::AlignedScaling3f(0.3f, 0.3f, 1);

    quadShader->bind();
    quadShader->set_uniform("M", TRS2.matrix());
    // Make texture unit 0 active
    glActiveTexture(GL_TEXTURE0);
    moon->bind();

    // Set the shader's texture uniform to the index of the texture unit we have
    quadShader->set_uniform("tex", 0);
    quad->set_attributes(*quadShader);
    quad->draw();
    moon->unbind();
    quadShader->unbind();

    glDisable(GL_BLEND);
}

int main(int, char**){

    Application app;
    init();

    fb = std::unique_ptr<Framebuffer>(new Framebuffer());

    /// TODO: initialize color buffer texture, and allocate memory
    c_buf = std::unique_ptr<RGBA8Texture>(new RGBA8Texture());
    c_buf->allocate(width,height);

    /// TODO: attach color texture to framebuffer
    fb->attach_color_texture(*c_buf);

    Window& window = app.create_window([](Window&){
        glViewport(0,0,width,height);

        fb->bind();
        glClear(GL_COLOR_BUFFER_BIT);

        drawScene(glfwGetTime());
        fb->unbind();

        glViewport(0, 0, 2*width, 2*height);
        glClear(GL_COLOR_BUFFER_BIT);
        fbShader->bind();
        //Bind texture and set uniforms
        glActiveTexture(GL_TEXTURE0);
        c_buf->bind();
        fbShader->set_uniform("tex", 0);
        fbShader->set_uniform("tex_width", float(width));
        fbShader->set_uniform("tex_height", float(height));

        quad->set_attributes(*fbShader);
        quad->draw();
       c_buf->unbind();
        fbShader->unbind();

        glEnable(GL_BLEND);

        Transform TRS = Transform::Identity();
       //TRS3 *= Eigen::Translation3f(0.5f, 0.5f, 0);
        TRS3 *= Eigen::AngleAxisf((glfwGetTime()) + M_PI / 2, Eigen::Vector3f::UnitZ());
        TRS3 *= Eigen::AlignedScaling3f(0.2f, 0.2f, 1);

        quadShader->bind();
        quadShader->set_uniform("M", TRS3.matrix());
        // Make texture unit 0 active
        glActiveTexture(GL_TEXTURE0);

        moon->bind();
        quadShader->set_uniform("tex", 0);
        quad->set_attributes(*quadShader);
        quad->draw();
        moon->unbind();
        quadShader->unbind();

        drawRocket(glfwGetTime());

       glDisable(GL_BLEND);
    });
    window.set_title("FrameBuffer");
    window.set_size(width, height);

    return app.run();
}


