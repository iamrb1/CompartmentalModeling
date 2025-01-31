#ifndef ANNOTATEDWRAPPER_H
#define ANNOTATEDWRAPPER_H

#include <iostream>
#include <map>

class AnnotatedWrapper {
public:
    // Constructor
    AnnotatedWrapper();

    // Destructor
    ~AnnotatedWrapper();

    // Public methods
    void addAnnotation( int key, int value);
    int getAnnotation(int key);
    void removeAnnotation(int key);
    void listAnnotations();
    void clearAnnotations();

private:
    std::map<int, int> annotations;

};

#endif // ANNOTATEDWRAPPER_H