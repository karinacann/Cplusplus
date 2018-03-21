/*
 * CSC 305 201801 UVIC
 * The purpose of this source file is to demonstrate the Mesh class which you may use in assignment 2
 * Its only functionality is to render vertices/normals/textures and load textures from png files
 * A demonstration of an ImGui menu window is also included in this file
*/
#include "Mesh/Mesh.h"
#include "OpenGP/GL/glfw_helpers.h"

#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/Application.h>
#include <OpenGP/GL/ImguiRenderer.h>
//#include <iostream>

using namespace OpenGP;

void toFile(std::vector<Vec3> &vertexList,std::vector<unsigned int> &indexList, std::string name){
    std::ofstream outputFile;
    outputFile.open(name);

    int size= vertexList.size();
    int indexSize = indexList.size();
    for (int i=0;i<size;i++){
        Vec3 a =vertexList[i];
        outputFile <<"v " << a[0] <<" "<< a[1]<<" "<<a[2]<<"\n";
    }
    outputFile<<"# number of vertices: "<<size<<std::endl;

    for (int i=0;i<indexSize-3;i=i+3){
        int a =indexList[i];
        int b =indexList[i+1];
        int c =indexList[i+2];

        a++;
        b++;
        c++;

        outputFile <<"f " << a<<" "<< b<<" "<<c<<"\n";
    }

    outputFile<<"# number of faces: "<<indexSize/3<<std::endl;

    //texture

    //normal
    outputFile.close();
}

int main() {

    Application app;
    ImguiRenderer imrenderer;
    Mesh renderMesh;

    /// Example rendering a mesh
    /// Call to compile shaders
    renderMesh.init();

///CYLINDER**************************************************************************************


    std::vector<Vec3> myVertList;
    std::vector<unsigned int> myIndexList;

    Vec3 origin = Vec3(0,0,0);
    float radius = 0.5f;
    float cylinderHeight=1.0f;

    int numberPartitions = 40;

    myVertList.push_back(origin);

    float fraction = 2.0*M_PI/numberPartitions;
    float theta = fraction;
    float x=radius;
    float z=0;

//First circle of cylinder
   //the x and z rotate by theta
    for (int i=0;i<numberPartitions;i++){
        x=radius*cos(theta);
        z=radius*sin(theta);
        theta+=fraction;
        myVertList.push_back(Vec3(x,origin[1],z));
    }

    //assign vertices to faces
    for (int i=1;i<=numberPartitions;i++){
        myIndexList.push_back(0);
        myIndexList.push_back(i);
        if (i==numberPartitions){
            myIndexList.push_back(1);
        }else{
            myIndexList.push_back(i+1);
        }
    }
    //make second circle - translated from first ones.
    Vec3 translatedOrigin = Vec3(origin[0],origin[1]+cylinderHeight,origin[2]);
    int translatedOriginListPosition = myVertList.size();

    myVertList.push_back(translatedOrigin);

    theta = fraction;
    x=radius;
    z=0;

    for (int i=0;i<numberPartitions;i++){
        x=radius*cos(theta);
        z=radius*sin(theta);
        theta+=fraction;
        myVertList.push_back(Vec3(x,cylinderHeight,z));
    }

    //add new faces
    for (int i=1;i<=numberPartitions;i++){
        myIndexList.push_back(numberPartitions+1);
        myIndexList.push_back(numberPartitions+1+i);
        if (i==numberPartitions){
            myIndexList.push_back(numberPartitions+1+1);
        }else{
            myIndexList.push_back(numberPartitions+i+1+1);
        }
    }

    //load sides into index list
    for (int i=1; i<numberPartitions;i++){
        myIndexList.push_back(i); //first face
        myIndexList.push_back(i+numberPartitions+1);
        if(i==numberPartitions-1){
            //at end of cylinder, so can complete the last triangle here.
            myIndexList.push_back(1);

            myIndexList.push_back(1);//second face
            myIndexList.push_back(i+numberPartitions+1);
            myIndexList.push_back(i+numberPartitions+2);
       }else{
        myIndexList.push_back(i+1); //last vertex of first face

        myIndexList.push_back(i+1); //second face
        myIndexList.push_back(i+numberPartitions+1);
        myIndexList.push_back(i+numberPartitions+2);
       }
    }
    renderMesh.loadVertices(myVertList, myIndexList);

///CUBE***************************************************************************
    float length = 1;
   std::vector<Vec3> vertList;
    vertList.push_back(Vec3(0,0,0));
    vertList.push_back(Vec3(length,0,0));
    vertList.push_back(Vec3(length,0,length));
    vertList.push_back(Vec3(0,0,length));

    vertList.push_back(Vec3(0,length,0));
    vertList.push_back(Vec3(length,length,0));
    vertList.push_back(Vec3(length,length,length));
    vertList.push_back(Vec3(0,length,length));

    std::vector<unsigned int> indexList;
   //bottom gd
    indexList.push_back(0); // Face 1
    indexList.push_back(1);
    indexList.push_back(2);
    indexList.push_back(2); // Face 2
    indexList.push_back(3);
    indexList.push_back(0);

    //left side gd
    indexList.push_back(3); // Face 3
    indexList.push_back(0);
    indexList.push_back(4);
    indexList.push_back(4); // Face 4
    indexList.push_back(7);
    indexList.push_back(3);

    //front /
    indexList.push_back(0); // Face 5
    indexList.push_back(1);
    indexList.push_back(5);
    indexList.push_back(5); // Face 6
    indexList.push_back(4);
    indexList.push_back(0);

    //right gd /
    indexList.push_back(1); // Face 7
    indexList.push_back(2);
    indexList.push_back(6);
    indexList.push_back(6); // Face 8
    indexList.push_back(5);
    indexList.push_back(1);

    //back side
    indexList.push_back(2); // Face 9
    indexList.push_back(3);
    indexList.push_back(7);
    indexList.push_back(7); // Face 10
    indexList.push_back(6);
    indexList.push_back(2);

      //front side
    indexList.push_back(4); // Face 11
    indexList.push_back(5);
    indexList.push_back(6);
    indexList.push_back(6); // Face 12
    indexList.push_back(7);
    indexList.push_back(4);

  renderMesh.loadVertices(vertList, indexList);

///SPHERE*****************************************************************************

    std::vector<Vec3> mySphereVertList;
    std::vector<unsigned int> mySphereIndexList;

    //void GenerateSphereMesh(OpenGP::Vec3 center, float radius, int numberLatitude, int numberLongitude) {
    //Determine the poles, e.g., take along y-axis
    //Generate points, i.e., intersections of longitudes and latitudes

    Vec3 center=Vec3(0,0,0);
    float sRadius=1.0f;
    int numberLatitude=40;
    int numberLongitude=40;

    Vec3 newVertex;
    float sTheta;
    float phi;

    for (int p=0; p<=numberLatitude;p++){
        sTheta = M_PI*p/numberLatitude;
        for (int q=0;q<numberLongitude;q++){
            phi=2*M_PI*q/numberLongitude;

            Vec3 newVertex =
            Vec3(center[0]+sin(sTheta)*cos(phi),
            center[1]+sin(sTheta)*sin(phi),
            center[2]+cos(sTheta));

            mySphereVertList.push_back(newVertex);
        }
    }
    //Generate triangles,

    //Got the computation for the second vertex of the faces from
    //https://gist.github.com/davidbitton/1094320
    for (int p=0; p<numberLatitude;p++){
        for (int q=0;q<numberLongitude;q++){

            int first = (p*numberLongitude)+q;
            mySphereIndexList.push_back(first); //should be good
            mySphereIndexList.push_back(first+numberLongitude+1);
            mySphereIndexList.push_back(first+1); //should be good

          //  triangle 2
            mySphereIndexList.push_back(first); //should be good
            mySphereIndexList.push_back(first+numberLongitude);
            mySphereIndexList.push_back(first+numberLongitude+1);
        }
    }

   //renderMesh.loadVertices(mySphereVertList, mySphereIndexList);

///*************************WRITE .OBJ FILE ****************************************
///***CUBE
    toFile(vertList, indexList, "myCube.obj");
    toFile(myVertList, myIndexList, "myCylinder.obj");
    toFile(mySphereVertList, mySphereIndexList, "mySphere.obj");

///*************************READ .OBJ FILE ****************************************

    std::vector<Vec3> myReadVertList;
    std::vector<unsigned int> myReadIndexList;

    std::ifstream inputFile;
    //Got syntax from https://www.uow.edu.au/~lukes/TEXTBOOK/notes-cpp/io/readtextfile.html
    inputFile.open("bunny.obj");
    if (!inputFile){
        std::cerr <<"Unable to open file";
        exit(1);
    }
    std::string line;

   //https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
   while(getline(inputFile,line)){
        if(line.substr(0,2)=="v "){
            std::istringstream temp(line.substr(2));
            Vec3 nextVertex;
            temp>>nextVertex[0];
            temp>>nextVertex[1];
            temp>>nextVertex[2];

            myReadVertList.push_back(nextVertex);

        }else if (line.substr(0,2)=="f "){
            std::istringstream temp(line.substr(2));
            int a,b,c;

            temp>>a;
            temp>>b;
            temp>>c;
            //decrement?
            a--;
            b--;
            c--;
            myReadIndexList.push_back(a);
            myReadIndexList.push_back(b);
            myReadIndexList.push_back(c);
        }else if (line.substr(0,2)=="# "){
        }else{
        }
   }

  inputFile.close();
renderMesh.loadVertices(myReadVertList, myReadIndexList);

///***********************OTHER************************************************************

    /// Load normals
    std::vector<Vec3> normList;
    normList.push_back(Vec3(0,0,1));
    normList.push_back(Vec3(0,0,1));
    normList.push_back(Vec3(0,0,1));
    normList.push_back(Vec3(0,0,1));
  //  renderMesh.loadNormals(normList);

    /// Load textures (assumes texcoords)
    renderMesh.loadTextures("earth.png");

    /// Load texture coordinates (assumes textures)
    std::vector<Vec2> tCoordList;
    tCoordList.push_back(Vec2(0,0));
    tCoordList.push_back(Vec2(1,0));
    tCoordList.push_back(Vec2(1,1));
    tCoordList.push_back(Vec2(0,1));
    renderMesh.loadTexCoords(tCoordList);
//
    /// Create main window, set callback function
    auto &window1 = app.create_window([&](Window &window){
        int width, height;
        std::tie(width, height) = window.get_size();

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// Wireframe rendering, might be helpful when debugging your mesh generation
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        float ratio = width / (float) height;
        Mat4x4 modelTransform = Mat4x4::Identity();
        Mat4x4 model = modelTransform.matrix();
        Mat4x4 projection = OpenGP::perspective(70.0f, ratio, 0.1f, 10.0f);

        //camera movement
        float time = .5f * (float)glfwGetTime();
        Vec3 cam_pos(2*cos(time), 2.0, 2*sin(time));
        Vec3 cam_look(0.0f, 0.0f, 0.0f);
        Vec3 cam_up(0.0f, 1.0f, 0.0f);
        Mat4x4 view = OpenGP::lookAt(cam_pos, cam_look, cam_up);

        renderMesh.draw(model, view, projection);
    });
    window1.set_title("Assignment 2");

    /// Create window for IMGUI, set callback function
    auto &window2 = app.create_window([&](Window &window){
        int width, height;
        std::tie(width, height) = window.get_size();

        imrenderer.begin_frame(width, height);

        ImGui::BeginMainMenuBar();
        ImGui::MenuItem("File");
        ImGui::MenuItem("Edit");
        ImGui::MenuItem("View");
        ImGui::MenuItem("Help");
        ImGui::EndMainMenuBar();

        ImGui::Begin("Test Window 1");
        ImGui::Text("This is a test imgui window");
        ImGui::End();

        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        imrenderer.end_frame();
    });
    window2.set_title("imgui Test");

    return app.run();
}

//x = r cos 2pi*u
//y = rsin2pi*u
//z = v/h

//maps rectangle in u,v space to cylinder ofradius
//theeta and height h in world coordinates
//s = u
//t = v
