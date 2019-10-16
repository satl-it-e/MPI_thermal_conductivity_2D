#include "visualization.h"


void visualize(const MyArray<double> &field, std::string &filename, const double &min, const double &max){
    cv::Mat vis_field(field.height, field.width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Vec3b color;
    double ratio;

    for (int i = 0; i < field.height; i++){
        for (int j = 0; j < field.width; j++){
            ratio = (field.get_value(i, j) - min) / (max - min);
            color[1] = (uchar) std::max(0.0, 255 * (ratio - 1));
            color[0] = (uchar) std::max(0.0, 255 * (1 - ratio));
            color[2] = (uchar) (255 - color[0] - color[1]);
            vis_field.at<cv::Vec3b>(cv::Point(j, i)) = color;
        }
    }

    cv::imwrite(filename, vis_field);
}

