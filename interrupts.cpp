#include<interrupts.hpp>

int main(int argc, char** argv) {

    if(argc != 3) {
        std::cout << "ERROR!\nExpected 2 argument, received " << argc - 1 << std::endl;
        std::cout << "To run the program, do: ./interrutps <your_trace_file.txt> <your_vector_table.txt>" << std::endl;
    }

    std::ifstream input_file;
    input_file.open(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open file: " << argv[1] << std::endl;
        return 1;
    }

    std::ifstream input_vector_table;
    input_vector_table.open(argv[2]);
    if (!input_vector_table.is_open()) {
        std::cerr << "Error: Unable to open file: " << argv[2] << std::endl;
        return 1;
    }

    std::string vector;
    std::vector<std::string> vectors;
    while(std::getline(input_vector_table, vector)) {
        vectors.push_back(vector);
    }

    std::string trace;
    std::string execution;
    int current_time = 0;

    // Define LCG (more memory efficient than uniform distribution)
    SimpleLCG lcg(12345);  // Seed the LCG with a number (can be customized)

    //This keeps track of sensor interrupt calls and display interrupt calls
    int sensor_display_count = 0;

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {

        //split line by ','
        auto parts = split_delim(trace, ",");
        if (parts.size() < 2) {
            std::cerr << "Error: Malformed input line: " << trace << std::endl;
            continue;
        }

        auto activity = parts[0];
        auto duration = std::stoi(parts[1]);

        if(activity == "CPU"){
            //append CPU activity to execution.txt and advance time
            execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", CPU execution\n";
            current_time += duration;

        } else {
            //if interrupt, get name and INT#.

            auto parts_1 = split_delim(activity, " ");
            if (parts_1.size() < 2) {
                std::cerr << "Error: Malformed input line: " << trace << std::endl;
                continue;
            }
            auto interrupt  = parts_1[0];
            auto intr_num   = std::stoi(parts_1[1]);
            
            if(sensor_display_count % 4 == 0) { // Sensor SYSCALL
                //append each step of interrupt mechanism in the execution.txt and advance time

                //random number between 1 and 3 for context save time
                // int context_save_time = context_time_distr(rng);
                int context_save_time = lcg.get_random(1, 3);

                //function that fills out the interrupt boilerplate
                std::pair<std::string, int> boiler_plate = intr_boilerplate(current_time, intr_num, context_save_time, vectors);
                execution += boiler_plate.first;
                current_time = boiler_plate.second;

                int ISR_instruction_1 = lcg.get_random(10, duration);
                int ISR_instruction_2 = lcg.get_random(10, duration - ISR_instruction_1) - 1;
                int ISR_instruction_3 = duration - (ISR_instruction_1 + ISR_instruction_2);

                execution += std::to_string(current_time) + ", " + std::to_string(ISR_instruction_1) + ", SYSCALL: run the ISR\n";
                current_time += ISR_instruction_1;
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_instruction_2) + ", transfer data\n";
                current_time += ISR_instruction_2;
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_instruction_3) + ", check for errors\n";
                current_time += ISR_instruction_3;

            } else if (sensor_display_count % 4 == 1) { // Sensor END_IO

                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", check priority of interrupt\n";
                current_time++;
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", check if masked\n";
                current_time++;

                ///random number between 1 and 3 for context save time
                int context_save_time = lcg.get_random(1, 3);

                //function that fills out the interrupt boilerplate
                std::pair<std::string, int> boiler_plate = intr_boilerplate(current_time, intr_num, context_save_time, vectors);
                execution += boiler_plate.first;
                current_time = boiler_plate.second;

                execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", END_IO\n";
                current_time += duration;

            } else if (sensor_display_count % 4 == 2) { // Display SYSCALL
                //append each step of interrupt mechanism in the execution.txt and advance time

                //random number between 1 and 3 for context save time
                int context_save_time = lcg.get_random(1, 3);

                //function that fills out the interrupt boilerplate
                std::pair<std::string, int> boiler_plate = intr_boilerplate(current_time, intr_num, context_save_time, vectors);
                execution += boiler_plate.first;
                current_time = boiler_plate.second;

                int ISR_instruction_1 = lcg.get_random(10, duration);
                int ISR_instruction_2 = lcg.get_random(10, duration - ISR_instruction_1) - 1;
                int ISR_instruction_3 = duration - (ISR_instruction_1 + ISR_instruction_2);

                execution += std::to_string(current_time) + ", " + std::to_string(ISR_instruction_1) + ", SYSCALL: run the ISR\n";
                current_time += ISR_instruction_1;
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_instruction_2) + ", transfer data to the display\n";
                current_time += ISR_instruction_2;
                execution += std::to_string(current_time) + ", " + std::to_string(ISR_instruction_3) + ", check for errors\n";
                current_time += ISR_instruction_3;

            } else if (sensor_display_count % 4 == 3) { // Display END_IO
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", check priority of interrupt\n";
                current_time++;
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", check if masked\n";
                current_time++;

                //random number between 1 and 3 for context save time
                int context_save_time = lcg.get_random(1, 3);

                //function that fills out the interrupt boilerplate
                std::pair<std::string, int> boiler_plate = intr_boilerplate(current_time, intr_num, context_save_time, vectors);
                execution += boiler_plate.first;
                current_time = boiler_plate.second;

                execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", END_IO\n";
                current_time += duration;

            }
            sensor_display_count++;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", IRET\n";
            current_time++;
        }
    }

    input_file.close();
    std::ofstream output_file("execution.txt");

    if (output_file.is_open()) {
        output_file << execution;
        output_file.close();  // Close the file when done
        std::cout << "File content overwritten successfully." << std::endl;
    } else {
        std::cerr << "Error opening file!" << std::endl;
    }

    std::cout << "Output generated in execution.txt" << std::endl;
    return 0;
}