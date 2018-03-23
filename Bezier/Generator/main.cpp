#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>

using namespace OpenGP;

const int width=720, height=720;
typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
#define POINTSIZE 10.0f

const char* line_vshader =
#include "line_vshader.glsl"
;
const char* line_fshader =
#include "line_fshader.glsl"
;
const char* quad_vshader =
#include "quad_vshader.glsl"
;
const char* quad_fshader =
#include "quad_fshader.glsl"
;

void init();
void quadInit(std::unique_ptr<GPUMesh> &quad);
void loadTexture(std::unique_ptr<RGBA8Texture> &texture, const char* filename);

std::unique_ptr<Shader> lineShader;
std::unique_ptr<GPUMesh> line;
std::unique_ptr<GPUMesh> bezierLine;
std::vector<Vec2> controlPoints;
std::vector<Vec2> bezierLinePairs;

std::unique_ptr<GPUMesh> quad;
std::unique_ptr<Shader> quadShader;
std::unique_ptr<RGBA8Texture> sky;

const int maxDepth=10;

/*
 * deCasteljau algorithm
 *   AB, BC CD are all lines. Algorithm computes all midpoints of the three lines,
 * and draws two lines between those midpoints.
 * Then, it compute the newly generated lines' midpoints, draws a line between those
 * midpoints, and compute the final midpoint.
 * The algorithm recursively calls itself for each side of the curve
 */
void drawBezier(Vec2 A, Vec2 B, Vec2 C, Vec2 D, int currDepth ){

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
    Vec2 midpointABCD = Vec2((midpointBCD.x()+midpointABC.x())/2, (midpointBCD.y()+midpointABC.y())/2);

    drawBezier(A, midpointAB, midpointABC, midpointABCD, currDepth+1);
    drawBezier(midpointABCD, midpointBCD, midpointCD, D, currDepth+1);
}

int main(int, char**){

    Application app;
    init();

    // Mouse position and selected point
    Vec2 position = Vec2(0,0);
    Vec2 *selection = nullptr;

    // Display callback
    Window& window = app.create_window([&](Window&){


        glViewport(0,0,2*width,2*height);
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(POINTSIZE); //draws them with given size

        lineShader->bind();
        //Draw bezierLine

        // Draw line red
        lineShader->set_uniform("selection", -1);
        line->set_attributes(*lineShader);
        line->set_mode(GL_LINES);  //its triangles by default.dont use just gl_line
        line->draw();

        lineShader->set_uniform("selection", -1);
        bezierLine->set_attributes(*lineShader);
        bezierLine->set_mode(GL_LINES);
        bezierLine->draw();

        // Draw points red and selected point blue, if we're pressing down
        if(selection!=nullptr) lineShader->set_uniform("selection", int(selection-&controlPoints[0]));
            line->set_mode(GL_POINTS);
            line->draw();
            lineShader->unbind();
         });
    window.set_title("Mouse");
    window.set_size(width, height);

    // Mouse movement callback
    window.add_listener<MouseMoveEvent>([&](const MouseMoveEvent &m){   //
        // Mouse position in clip coordinates
        Vec2 p = 2.0f*(Vec2(m.position.x()/width,-m.position.y()/height) - Vec2(0.5f,-0.5f));
        if( selection && (p-position).norm() > 0.0f) {
            /// TODO: Make selected control points move with cursor
            selection->x() = position.x();
            selection->y() = position.y();
            line->set_vbo<Vec2>("vposition", controlPoints);

            bezierLinePairs.clear();
            drawBezier(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], 0);
            bezierLine->set_vbo<Vec2>("vposition", bezierLinePairs);
        }
        position = p;
    });

    // Mouse click callback
    window.add_listener<MouseButtonEvent>([&](const MouseButtonEvent &e){
        // Mouse selection case
        if( e.button == GLFW_MOUSE_BUTTON_LEFT && !e.released) {
            selection = nullptr;
            for(auto&& v : controlPoints) {
                if ( (v-position).norm() < POINTSIZE/std::min(width,height) ) { //if near the mouse point
                    selection = &v;
                    if (*selection == controlPoints[0]){
                        bezierLine->set_vbo<Vec2>("vposition", bezierLinePairs);
                    }

                    break;
                }
            }
        }
        // Mouse release case
        if( e.button == GLFW_MOUSE_BUTTON_LEFT && e.released) {
            if(selection) {
                selection->x() = position.x();
                selection->y() = position.y();
                selection = nullptr;
                line->set_vbo<Vec2>("vposition", controlPoints);
            }
        }
    });

    return app.run();
}



void init(){
    glClearColor(1,1,1, /*solid*/1.0 );

    quadShader = std::unique_ptr<Shader>(new Shader());
    quadShader->verbose = true;
    quadShader->add_vshader_from_source(quad_vshader);
    quadShader->add_fshader_from_source(quad_fshader);
    quadShader->link();

    quadInit(quad);
    loadTexture(sky, "sky.png");
    quadShader->bind();

    Transform TRS = Transform::Identity();
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
    quadShader->unbind();

    lineShader = std::unique_ptr<Shader>(new Shader());
    lineShader->verbose = true;
    lineShader->add_vshader_from_source(line_vshader);
    lineShader->add_fshader_from_source(line_fshader);
    lineShader->link();

    controlPoints = std::vector<Vec2>();
    controlPoints.push_back(Vec2(-0.6f,0.0f));
    controlPoints.push_back(Vec2(-0.5f, -0.5f));
    controlPoints.push_back(Vec2( 0.5f, 0.5f));
    controlPoints.push_back(Vec2( 0.6f, 0.0f));

    line = std::unique_ptr<GPUMesh>(new GPUMesh());
    line->set_vbo<Vec2>("vposition", controlPoints);
    std::vector<unsigned int> indices = {0,1,2,3};
    line->set_triangles(indices);

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
    //if there's an error, display it
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



