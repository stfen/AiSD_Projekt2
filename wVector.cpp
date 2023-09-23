#include "wVector.h"

template <typename Obj>
wVector<Obj>::wVector() {
    max_size = 1;
    size = 0;
    tab = new Obj[max_size];
}

template <typename Obj>
wVector<Obj>::~wVector() {
    //delete[] tab;
}

template <typename Obj>
Obj wVector<Obj>::index(int i) {
    return tab[i];
}

template <typename Obj>
void wVector<Obj>::push_back(Obj a) {
    if (size == max_size) {
        max_size += 4;
        Obj* temp = new Obj[max_size];
        for (int i = 0; i < size; i++) {
            temp[i] = tab[i];
        }
        delete[] tab;
        tab = temp;
    }
    tab[size++] = a;
}

template <typename Obj>
int wVector<Obj>::GetSize() {
    return size;
}

template <typename Obj>
int wVector<Obj>::GetMaxSize() {
    return max_size;
}