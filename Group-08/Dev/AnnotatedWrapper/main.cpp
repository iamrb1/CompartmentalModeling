#include "AnnotatedWrapper.h"
#include <string>
using namespace std;

int main()
{
    AnnotatedWrapper aw = AnnotatedWrapper();
    std::string input = "input";
    std::string output = "input";
    aw.addAnnotation(input, output);
    // aw.addAnnotation("key2", "value2");
    // aw.addAnnotation("key3", "value3");

    // aw.listAnnotations();

    // std::cout << "Value of key1: " << aw.getAnnotation("key1") << std::endl;
    // std::cout << "Value of key2: " << aw.getAnnotation("key2") << std::endl;
    // std::cout << "Value of key3: " << aw.getAnnotation("key3") << std::endl;

    // aw.removeAnnotation("key2");
    // aw.listAnnotations();

    // aw.clearAnnotations();
    // aw.listAnnotations();


    std::cout << "Hello World" << std::endl;
    return 0;
}