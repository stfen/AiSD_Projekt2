#pragma once
#include <iostream>

template <typename Obj>
class wVector {
private:
    Obj* tab;
    int max_size,size;
public:
    void push_back(Obj a);
    Obj index(int i);
    int GetSize();
    int GetMaxSize();
    wVector();
    ~wVector();
};