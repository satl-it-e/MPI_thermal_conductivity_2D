#ifndef THERMAL_CONDUCTIVITY_2D_VISUALIZATION_H
#define THERMAL_CONDUCTIVITY_2D_VISUALIZATION_H


#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "my_array.h"


void visualize(const MyArray<double> &field, std::string &filename,
        const double &min, const double &max);

#endif //THERMAL_CONDUCTIVITY_2D_VISUALIZATION_H
