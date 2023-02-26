snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_createrand.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
40008: 0.000: Allocated 170MB of shared memory
40008: 0.001: FileMicro-CreateRand Version 2.1 personality successfully loaded
40008: 0.001: Creating/pre-allocating files and filesets
40008: 0.005: File largefile: 0.000MB
rm: cannot remove '/dev/shm/test/client_test/largefile/00000001': Directory not empty
40008: 10.853: Removed any existing file largefile in 11 seconds
40008: 10.853: making tree for filset /dev/shm/test/client_test/largefile
40008: 10.854: Creating file largefile...
40008: 10.859: Preallocated 1 of 1 of file largefile in 1 seconds
40008: 10.859: waiting for fileset pre-allocation to finish
40122: 10.859: Starting 1 filecreater instances
40123: 10.860: Starting 1 filecreaterthread threads
40008: 11.861: Running...
40008: 19.862: Run took 8 seconds...
40008: 19.862: Per-Operation Breakdown
finish               206ops       26ops/s   0.0mb/s      0.0ms/op       49us/op-cpu [0ms - 0ms]
sync                 206ops       26ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
append-file          2061ops      258ops/s 128.0mb/s      3.4ms/op     1266us/op-cpu [0ms - 21ms]
40008: 19.862: IO Summary:  2267 ops, 283.350 ops/s, (0/258 r/w), 128.0mb/s,   4639us cpu/op,   3.4ms latency
40008: 19.862: Shutting down processes
snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_createrand.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
40273: 0.000: Allocated 170MB of shared memory
40273: 0.001: FileMicro-CreateRand Version 2.1 personality successfully loaded
40273: 0.001: Creating/pre-allocating files and filesets
40273: 0.004: File largefile: 0.000MB
rm: cannot remove '/dev/shm/test/client_test/largefile/00000001': Directory not empty
40273: 15.053: Removed any existing file largefile in 16 seconds
40273: 15.053: making tree for filset /dev/shm/test/client_test/largefile
40273: 15.054: Creating file largefile...
40273: 15.059: Preallocated 1 of 1 of file largefile in 1 seconds
40273: 15.059: waiting for fileset pre-allocation to finish
40411: 15.059: Starting 1 filecreater instances
40412: 15.060: Starting 1 filecreaterthread threads
40273: 16.061: Running...
40273: 24.062: Run took 8 seconds...
40273: 24.062: Per-Operation Breakdown
finish               203ops       25ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
sync                 203ops       25ops/s   0.0mb/s      0.0ms/op       49us/op-cpu [0ms - 0ms]
append-file          2031ops      254ops/s 128.2mb/s      3.5ms/op     1315us/op-cpu [0ms - 14ms]
40273: 24.062: IO Summary:  2234 ops, 279.218 ops/s, (0/254 r/w), 128.2mb/s,   4343us cpu/op,   3.5ms latency
40273: 24.062: Shutting down processes
snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_createrand.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
40530: 0.000: Allocated 170MB of shared memory
40530: 0.001: FileMicro-CreateRand Version 2.1 personality successfully loaded
40530: 0.001: Creating/pre-allocating files and filesets
40530: 0.011: File largefile: 0.000MB
rm: cannot remove '/dev/shm/test/client_test/largefile/00000001': Directory not empty
40530: 18.264: Removed any existing file largefile in 19 seconds
40530: 18.264: making tree for filset /dev/shm/test/client_test/largefile
40530: 18.266: Creating file largefile...
40530: 18.270: Preallocated 1 of 1 of file largefile in 1 seconds
40530: 18.270: waiting for fileset pre-allocation to finish
40698: 18.270: Starting 1 filecreater instances
40699: 18.271: Starting 1 filecreaterthread threads
40530: 19.272: Running...

40530: 27.273: Run took 8 seconds...
40530: 27.273: Per-Operation Breakdown
finish               204ops       25ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
sync                 204ops       25ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
append-file          2041ops      255ops/s 128.4mb/s      3.7ms/op     1362us/op-cpu [0ms - 19ms]
40530: 27.273: IO Summary:  2245 ops, 280.595 ops/s, (0/255 r/w), 128.4mb/s,   4728us cpu/op,   3.7ms latency
40530: 27.273: Shutting down processes

--------------------------------------------------------------------------------------------------------------------------

