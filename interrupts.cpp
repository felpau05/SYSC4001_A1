/**
 *
 * @file interrupts.cpp
 * @author Paul Felfli 101295764, Ajay Uppal 101308579
 *
 */

#include <interrupts.hpp>
#include <sstream>

int main(int argc, char** argv) {

    // vectors is a std::vector<std::string> with ISR addresses as strings
    // delays  is a std::vctor<int> with per-device total service times
    // index in both vectors corresponds to the device id starting at 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< buffer to hold one line from the trace
    std::string execution;  //!< accumulator for execution log

    /******************ADD YOUR VARIABLES HERE*************************/
    // Logical clock for the simulator (in ms)
    long long t_now_ms = 0;

    // Timing konbs (stick to the assignment defaults for Part 2(i))
    const long long MODE_SWITCH_MS   = 1;
    const long long CTX_SAVE_MS      = 10;  // vary 10/20/30 in experiments
    const long long VEC_LOOKUP_MS    = 1;
    const long long FETCH_ISR_MS     = 1;
    const long long IRET_MS          = 1;

    // Vector table layout assumption (2 bytes per entry)
    const long long VECTOR_ENTRY_BYTES = 2;

    // Hellper
    auto emit = [&](long long dur, const std::string& desc) {
        execution += std::to_string(t_now_ms) + ", " +
                     std::to_string(dur)      + ", " + desc + "\n";
        t_now_ms += dur;
    };
    /******************************************************************/

    // Parse each line of the input trace file
    while (std::getline(input_file, trace)) {
        auto [activity, val] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if (activity == "CPU") {
            // Pure CPU work — just advance time by the duration
            emit(val, "CPU burst");
            continue;
        }

        if (activity == "SYSCALL" || activity == "END_IO") {
            const int dev = val;

            // Defensive bounds check for device index
            long long device_total = 0;
            if (dev >= 0 && dev < static_cast<int>(delays.size())) {
                device_total = delays[dev];
            } else {
                // If device is unknown, treat as zero-time body (still show kernel steps)
                device_total = 0;
            }

            // Kernel entry and prologue
            emit(MODE_SWITCH_MS, "enter kernel mode");
            emit(CTX_SAVE_MS,    "save context");

            // Vector table access and ISR fetch
            const long long vec_addr = static_cast<long long>(dev) * VECTOR_ENTRY_BYTES;
            emit(VEC_LOOKUP_MS,  "locate vector " + std::to_string(dev) +
                                  " at memory offset " + std::to_string(vec_addr));
            emit(FETCH_ISR_MS,   "fetch ISR entry point");

            // Body time is the remaining portion after fixed overheads
            const long long fixed_overhead =
                MODE_SWITCH_MS + CTX_SAVE_MS + VEC_LOOKUP_MS + FETCH_ISR_MS + IRET_MS;
            long long body_ms = device_total - fixed_overhead;
            if (body_ms < 0) body_ms = 0;

            // Describe body slightly differently for syscall vs end_io
            if (activity == "SYSCALL") {
                emit(body_ms, "run device driver (system call)");
            } else {
                emit(body_ms, "finalize I/O and update memory (end of interrupt)");
            }

            // Epilogue
            emit(IRET_MS, "IRET");
            continue;
        }

        // If we got here, the activity token is not recognized — ignore gracefully
        emit(0, "unrecognized activity '" + activity + "' — skipped");
        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
