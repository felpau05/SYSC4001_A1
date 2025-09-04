#include<interrupts.hpp>

int main(int argc, char** argv) {

    auto vectors = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;
    std::string execution;

    int current_time = 0;

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration, intr_num] = parse_trace(trace);

        execution += "foo\n";
        
    }

    input_file.close();

    write_output(execution);

    return 0;
}