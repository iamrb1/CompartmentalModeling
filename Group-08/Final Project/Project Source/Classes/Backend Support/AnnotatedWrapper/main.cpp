// #include <iostream>
// #include "AnnotatedWrapper.hpp"

// int main() {
//     cse::AnnotatedWrapper<std::string> wrapper;

//     wrapper.addAnnotation("font", "Arial");
//     wrapper.addAnnotation("color", "Red");

//     // Lambda to print annotations
//     wrapper.listAnnotations([](const std::string& key, const std::string& value) {
//         std::cout << "Key: " << key << ", Value: " << value << "\n";
//     });

//     std::cout << "\nFont: " << wrapper.getAnnotation("font") << "\n";

//     wrapper.removeAnnotation("font");
//     std::cout << "Font after removal: " << wrapper.getAnnotation("font") << "\n";

//     wrapper.clearAnnotations();

//     // std::cout << "\nVersion: " << cse::AnnotatedWrapper<std::string>::version << "\n";

//     return 0;
// }