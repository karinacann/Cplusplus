R"(
#version 330 core
uniform int selection;

out vec4 color;

void main() {
    if ( selection == gl_PrimitiveID ) {    // built in variable thats the index of whats drawn.the primitives are just points, so the gl_PrimID is going to be 0, 1, 2, 3
        color = vec4(0,0,1,1);
    } else {
        color = vec4(1,0,0,1);
    }
}

//for new one: color = vec4(float(gl_PrimitveID + offsetID)/255,0,0,1); converting from 0 to 255 to 0 to 1
//glReadPixels(int(pixelPosition[0]0), height//will read value of that pixel into the array. will put color into array
//set sleection to be address of first control point plus whatever index it is. then unbind
)"
