#ifndef THERMAL_CONDUCTIVITY_2D_MY_ARRAY_H
#define THERMAL_CONDUCTIVITY_2D_MY_ARRAY_H

#include <iostream>


template<typename T>
class MyArray{
    private:
    T* array;

    public:
    int height = 0;
    int width = 0;

    MyArray(int i, int j){
        array = (T*) malloc((i * j) * sizeof(T));
        if (array == nullptr){
            std::cerr << "Failed to allocate memory for array with size " << i << " * " << j << std::endl;
        }
        std::fill( array, array + sizeof( array ), 0 );
        height = i;
        width = j;
    }

    T get_value(int i, int j) const {
        return array[i * j + j];
    }

    T* get_row(int i) const {
        T* row = (T*) malloc(width * sizeof(T));
        if (row == nullptr){
            std::cerr << "Failed to allocate memory for row" << std::endl;
        }
        for (int j = 0; j < width; j++){
            row[j] = this->get_value(i, j);
        }

        return row;
    }

    void set_value(int i, int j, T value){
        array[i * j + j] = value;
    }

    void set_row(int i, T* new_row){
        for (int j = 0; j < width; j++){
            this->set_value(i, j, new_row[j]);
        }
    }
};

#endif //THERMAL_CONDUCTIVITY_2D_MY_ARRAY_H
