#include<interrupts.hpp>

int main() {
    std::ifstream input_file;
    input_file.open("trace.txt");
    std::string trace;
    std::string execution;
    int current_time = 0;

    //define RNG
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 rng(rd()); // seed the generator
    std::uniform_int_distribution<> context_time_distr(1, 3); // define the range

    while(std::getline(input_file, trace)) {
        auto activity = split_delim(trace, ",")[0];
        auto duration = std::stoi(split_delim(trace, ",")[1]);

        if(activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", CPU execution\n";
            current_time += duration;
        } else {
            auto interrupt  = split_delim(activity, " ")[0];
            auto intr_num   = std::stoi(split_delim(activity, " ")[1]);
            
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
            current_time++;

            int context_save_time = context_time_distr(rng);
            execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
            current_time += context_save_time;
            
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", find vector " + std::to_string(intr_num) 
                            + " in memory position " + std::to_string(ADDR_BASE + (intr_num * VECTOR_SIZE)) + "\n";
            current_time++;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", obtain ISR address\n";
            current_time++;

            execution += std::to_string(current_time) + ", " + std::to_string(duration) + ", " + interrupt + " ISR exection\n";
            current_time += duration;

            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", IRET\n";
            current_time++;

        }
    }

    input_file.close();
    std::ofstream output_file("execution.txt");
    output_file << execution;
    output_file.close();

    std::cout << execution << std::endl;
    return 0;
}