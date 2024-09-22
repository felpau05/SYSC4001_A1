import random as rd

for i in range(0, 6):
    traces = "CPU, " + str(rd.randint(10, 100)) + "\n"
    count = 0
    for j in range(0, rd.randint(3, 50)):
        syscall_vecpos = rd.randint(1, 7)
        syscall_exectime = rd.randint(100, 400)
        traces += "SYSCALL " + str(syscall_vecpos) + ", " + str(syscall_exectime) + "\n"

        traces += "CPU, " + str(rd.randint(10, 100)) + "\n"

        endio_vecpos = rd.randint(16, 20)
        endio_exectime = rd.randint(100, 400)
        traces += "END_IO " + str(endio_vecpos) + ", " + str(endio_exectime) + "\n"

        traces += "CPU, " + str(rd.randint(10, 100)) + "\n"

        syscall_vecpos = rd.randint(8, 15)
        syscall_exectime = rd.randint(100, 400)
        traces += "SYSCALL " + str(syscall_vecpos) + ", " + str(syscall_exectime) + "\n"

        traces += "CPU, " + str(rd.randint(10, 100)) + "\n"

        endio_vecpos = rd.randint(20, 25)
        endio_exectime = rd.randint(100, 400)
        traces += "END_IO " + str(endio_vecpos) + ", " + str(endio_exectime) + "\n"

        traces += "CPU, " + str(rd.randint(10, 100)) + "\n"
        count += 1
    f1 = open("trace_" + str(i) + ".txt", 'w')

    f1.write(traces)
    f1.close()

    print("Written trace_" + str(i) + ".txt with " + str(count) + " interations.")

vectors = []
vector_file = ""
while(len(vectors) < 26):
    vector = rd.randint(128, 2048)
    if(vector not in vectors):
        vectors.append(vector)
        vector_file += format(vector, "#06X") + "\n"

f2 = open("vector_table.txt", 'w')
f2.write(vector_file)
f2.close()

print("Written vector table")