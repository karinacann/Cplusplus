#include <iostream>

using namespace std;

int myfunction(int value){
    value = value+1;
    return value;

}

void myfunction_pointer(int* val){


}

int main(int /*argc*/, char** /*argv[]*/)
{
    cout << "Hello World!" << endl;

    int a =1;
    int b= myfunction(a);
    cout << a << b <<endl;
    return EXIT_SUCCESS;
}
