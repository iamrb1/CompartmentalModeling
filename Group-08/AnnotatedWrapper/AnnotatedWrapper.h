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
    void add_annotation( int key, int value);
    int get_annotation(int key);
    void remove_annotation(int key);
    void list_annotations();
    void clear_annotations();

private:
    std::map<int, int> annotations;

};

#endif // ANNOTATEDWRAPPER_H