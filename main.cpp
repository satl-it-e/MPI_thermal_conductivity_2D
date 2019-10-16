#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>

#include <mpi.h>

#include "config.h"
#include "my_queue.h"
#include "visualization.h"
#include "my_array.h"


struct distribution_conf{
    unsigned int height;
    unsigned int width;
    unsigned int lines_per_rank;
//    unsigned int number_of_ranks;
} dist_c;

bool system_is_stable(const MyConfig &mc){
    return mc.delta_t <= (pow(std::max(mc.delta_x, mc.delta_y), 2) / (4 * mc.alpha));
}





void visualization_thread(MyQueue<MyArray<double>> &v_q, const std::string &out_filename,
        const double &field_min, const double &field_max){
    int i = 0;
    std::vector<MyArray<double>> result;
    while (true){
        result = v_q.pop();
        if (result.empty()){
            break;
        }
        std::string filename(std::to_string(i) + "_" + out_filename);
        visualize(result.front(), filename, field_min, field_max);
        i++;
    }
}


int read_field_from_file(MyConfig &mc, MyArray<double> &field, double &field_min, double &field_max){
    std::ifstream file(mc.map_file);
    try{
        if (file.is_open()){
            std::istream& cf = file;

            std::ios::fmtflags flags( cf.flags() ); // Save stream state
            cf.exceptions(std::ifstream::failbit); // Enable exception on fail
            std::string line;

            for(int i = 0; i < (int) (mc.length / mc.delta_y); i++){
                for(int j = 0; j < (int) (mc.width / mc.delta_x); j++){
                    double value;
                    cf >> value;

                    // Looking for min and max on the field
                    if (value < field_min){
                        field_min = value;
                    } else if (value > field_max){
                        field_max = value;
                    }
                    field.set_value(i, j, value);
                }
                getline(cf, line);
            }
        } else{
            std::cerr << "File " << mc.map_file << " couldn't be opened." << std::endl;
            return -4;
        }
    } catch(std::string &err){
        std::cerr << "Something wrong with loading field." << std::endl;
        std::cerr << err << std::endl;
        return -3;
    }

    file.close();
    return 0;
}


int write_field_to_file(MyConfig &mc, std::vector<std::vector<double>> &field){
    std::ofstream out_file(mc.last_state_filename);
    if (out_file.good()) {
        for (auto &row: field) {
            for (auto &el: row) {
                out_file << el << " ";
            }
            out_file << std::endl;
        }
    } else{
        std::cerr << "Couldn't open out file for saving current state: " << mc.last_state_filename << std::endl;
        return -1;
    }
    return 0;
}


int main(int argc, char* argv[]){

    // Need to find number of ranks
    int commsize, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {

        // Configurations
        std::string conf_file_name = "config.dat";
        if (argc >= 2) {
            conf_file_name = argv[1];
        }

        MyConfig mc;
        mc.load_configs_from_file(conf_file_name);
        if (mc.is_configured()) {
            std::cout << "Configurations loaded successfully.\n" << std::endl;
        } else {
            std::cerr << "Error. Not all configurations were loaded properly.";
            return -1;
        }

        // Check system stability
        if (!system_is_stable(mc)) {
            std::cerr << "System is not stable." << std::endl;
            return -2;
        }

        // First iteration
        std::cout << "Reading first iteration." << std::endl;

        std::vector<MyArray<double>> stripes;
        stripes.reserve(static_cast<unsigned long>(commsize));

        dist_c.height = mc.length;
        dist_c.width = mc.width;
        dist_c.lines_per_rank = dist_c.height / commsize;

        for (int i = 0; i < commsize - 1; i++){
            stripes.emplace_back(MyArray<double>(dist_c.lines_per_rank + 1, dist_c.width));
        }
        stripes.emplace_back(MyArray<double>(dist_c.lines_per_rank + dist_c.height % commsize + 1, dist_c.width));

    }else{

    }

//
//
//    MyArray<double> field, new_field, middle_field;
//    double field_min = INFINITY, field_max = -INFINITY;
//
//    if (read_field_from_file(mc, field, field_min, field_max) != 0){
//        return -3;
//    }
//
//    new_field = field;
//
//    // Visualization queue
//    MyQueue<MyArray<double> > vis_q;
//    std::thread visualization(visualization_thread, std::ref(vis_q), std::ref(mc.visualization_filename),
//            std::ref(field_min), std::ref(field_max));
//
//    vis_q.push(field);
//
//    std::cout << "Next iterations." << std::endl;
//    ctpl::thread_pool threads((int) mc.num_of_threads);
//
//    int visualization_interval = (int) (mc.visualization_interval / mc.delta_t);
//
//
//    // Iterations
//    for (int i = 0; i < mc.num_of_steps; i++){
//        for (int j = 0; j < visualization_interval; j++){
//            next_iteration(mc, field, new_field, threads);
//            middle_field = std::move(field);
//            field = std::move(new_field);
//            new_field = std::move(middle_field);
//        }
//        vis_q.push(field);
//    }
//
//    if (mc.last_state_filename != "-"){
//        std::cout << "Writing last state to file" << std::endl;
//        write_field_to_file(mc, field);
//    }
//
//
//    vis_q.finish();
//    visualization.join();
//
//    std::cout << "Finish." << std::endl;
    return 0;
}
