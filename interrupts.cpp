#include<interrupts.hpp>

int main(int argc, char** argv) {

    if(argc < 2 || argc > 2) {
        std::cout << "ERROR!\nExpected 1 argument, received " << argc - 1 << std::endl;
        std::cout << "To run the program, do: ./interrutps <your_trace_file.txt>" << std::endl;
    }

    std::ifstream input_file;
    input_file.open(argv[1]);
    std::string trace;
    std::string execution;
    int current_time = 0;

    //define RNG
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 rng(rd()); // seed the generator
    std::uniform_int_distribution<> context_time_distr(1, 3); // define the range

    //This keeps track of sensor interrupt calls and display interrupt calls
    int sensor_display_count = 0;

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        //split line by ','
        auto activity = split_delim(trace, ",")[0];
        auto duration = std::stoi(split_delim(trace, ",")[1]);

        if(activity == "CPU"){
            //append CPU activity to execution.txt and advance time
            execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", CPU execution\n";
            current_time += duration;
        } else {
            //if interrupt, get name and INT#.
            auto interrupt  = split_delim(activity, " ")[0];
            auto intr_num   = std::stoi(split_delim(activity, " ")[1]);
            
            if(sensor_display_count % 4 == 0) { // Sensor SYSCALL
                //append each step of interrupt mechanism in the execution.txt and advance time
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
                current_time++;

                int context_save_time = context_time_distr(rng); //random number between 1 and 3
                execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
                current_time += context_save_time;
                
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", find vector " + std::to_string(intr_num) 
                                + " in memory position " + std::to_string(ADDR_BASE + (intr_num * VECTOR_SIZE)) + "\n";
                current_time++;

                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", obtain ISR address\n";
                current_time++;

                std::uniform_int_distribution<> ISR_distr(10, duration); // define the range
                auto ISR_instruction_1 = ISR_distr(rng); // Random time for the 1st instruction of ISR
                std::uniform_int_distribution<> ISR_distr_1(10, duration - ISR_instruction_1); // define the range
                auto ISR_instruction_2 = ISR_distr_1(rng) - 1; // Random time for the 2nd instruction of ISR (-1 so that ISR_instruction_3 is never 0)
                auto ISR_instruction_3 = duration - (ISR_instruction_1 + ISR_instruction_2); // Random time for the 3rd instruction of ISR

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

                //append each step of interrupt mechanism in the execution.txt and advance time
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
                current_time++;

                int context_save_time = context_time_distr(rng); //random number between 1 and 3
                execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
                current_time += context_save_time;
                
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", find vector " + std::to_string(intr_num) 
                                + " in memory position " + std::to_string(ADDR_BASE + (intr_num * VECTOR_SIZE)) + "\n";
                current_time++;

                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", obtain ISR address\n";
                current_time++;

                execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", END_IO\n";
                current_time += duration;

            } else if (sensor_display_count % 4 == 2) { // Display SYSCALL
                //append each step of interrupt mechanism in the execution.txt and advance time
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
                current_time++;

                int context_save_time = context_time_distr(rng); //random number between 1 and 3
                execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
                current_time += context_save_time;
                
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", find vector " + std::to_string(intr_num) 
                                + " in memory position " + std::to_string(ADDR_BASE + (intr_num * VECTOR_SIZE)) + "\n";
                current_time++;

                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", obtain ISR address\n";
                current_time++;

                std::uniform_int_distribution<> ISR_distr(10, duration); // define the range
                auto ISR_instruction_1 = ISR_distr(rng); // Random time for the 1st instruction of ISR
                std::uniform_int_distribution<> ISR_distr_1(10, duration - ISR_instruction_1); // define the range
                auto ISR_instruction_2 = ISR_distr_1(rng) - 1; // Random time for the 2nd instruction of ISR (-1 so that ISR_instruction_3 is never 0)
                auto ISR_instruction_3 = duration - (ISR_instruction_1 + ISR_instruction_2); // Random time for the 3rd instruction of ISR

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

                //append each step of interrupt mechanism in the execution.txt and advance time
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
                current_time++;

                int context_save_time = context_time_distr(rng); //random number between 1 and 3
                execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
                current_time += context_save_time;
                
                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", find vector " + std::to_string(intr_num) 
                                + " in memory position " + std::to_string(ADDR_BASE + (intr_num * VECTOR_SIZE)) + "\n";
                current_time++;

                execution += std::to_string(current_time) + ", " + std::to_string(1) + ", obtain ISR address\n";
                current_time++;

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
    output_file << execution;
    output_file.close();

    std::cout << "Output generated in execution.txt" << std::endl;
    return 0;
}