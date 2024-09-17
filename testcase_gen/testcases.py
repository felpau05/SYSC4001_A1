import random as rd

print("Creating test cases")

for i in range(0, 5):
    traces = "CPU, " + str(rd.randint(10, 100)) + "\n"
    count = 0
    for j in range(0, rd.randint(3, 50)):
        traces += "SYSCALL " + str(rd.randint(0, 15)) + ", " + str(rd.randint(100, 400)) + "\n"
        traces += "END_IO " + str(rd.randint(16, 25)) + ", " + str(rd.randint(100, 400)) + "\n"
        traces += "CPU, " + str(rd.randint(10, 100)) + "\n"
        traces += "SYSCALL " + str(rd.randint(0, 15)) + ", " + str(rd.randint(100, 400)) + "\n"
        traces += "END_IO " + str(rd.randint(16, 25)) + ", " + str(rd.randint(100, 400)) + "\n"
        traces += "CPU, " + str(rd.randint(10, 100)) + "\n"
        count += 1
    f = open("trace_" + str(i) + ".txt", 'w')
    f.write(traces)
    print("Wrote trace_" + str(i) + ".txt with " + str(count) + " interations")
    f.close()