snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqwrite.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
34380: 0.000: Allocated 170MB of shared memory
34380: 0.001: FileMicro-SeqWrite Version 2.2 personality successfully loaded
34380: 0.001: Creating/pre-allocating files and filesets
34380: 0.004: File bigfile: 0.000MB
34380: 0.239: Removed any existing file bigfile in 1 seconds
34380: 0.239: making tree for filset /dev/shm/test/client_test/bigfile
34380: 0.252: Creating file bigfile...
34380: 0.257: Preallocated 1 of 1 of file bigfile in 1 seconds
34380: 0.257: waiting for fileset pre-allocation to finish
34384: 0.257: Starting 1 filewriter instances
34385: 0.259: Starting 1 filewriterthread threads
34380: 1.259: Running...
34380: 11.261: Run took 10 seconds...
34380: 11.261: Per-Operation Breakdown
finish               0ops        0ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
write-file           755ops       75ops/s  75.4mb/s     13.2ms/op     2874us/op-cpu [5ms - 40ms]
34380: 11.261: IO Summary:   755 ops, 75.492 ops/s, (0/75 r/w),  75.4mb/s,  14026us cpu/op,  13.2ms latency
34380: 11.261: Shutting down processes


snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqwrite.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
34672: 0.000: Allocated 170MB of shared memory
34672: 0.001: FileMicro-SeqWrite Version 2.2 personality successfully loaded
34672: 0.001: Creating/pre-allocating files and filesets
34672: 0.004: File bigfile: 0.000MB
34672: 0.271: Removed any existing file bigfile in 1 seconds
34672: 0.271: making tree for filset /dev/shm/test/client_test/bigfile
34672: 0.284: Creating file bigfile...
34672: 0.289: Preallocated 1 of 1 of file bigfile in 1 seconds
34672: 0.289: waiting for fileset pre-allocation to finish
34676: 0.289: Starting 1 filewriter instances
34677: 0.291: Starting 1 filewriterthread threads
34672: 1.291: Running...
34672: 11.293: Run took 10 seconds...
34672: 11.293: Per-Operation Breakdown
finish               0ops        0ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
write-file           637ops       64ops/s  63.6mb/s     15.6ms/op     3030us/op-cpu [7ms - 42ms]
34672: 11.293: IO Summary:   637 ops, 63.693 ops/s, (0/64 r/w),  63.6mb/s,  14835us cpu/op,  15.6ms latency
34672: 11.293: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqwrite.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
34947: 0.000: Allocated 170MB of shared memory
34947: 0.001: FileMicro-SeqWrite Version 2.2 personality successfully loaded
34947: 0.001: Creating/pre-allocating files and filesets
34947: 0.004: File bigfile: 0.000MB
34947: 0.234: Removed any existing file bigfile in 1 seconds
34947: 0.234: making tree for filset /dev/shm/test/client_test/bigfile
34947: 0.249: Creating file bigfile...
34947: 0.254: Preallocated 1 of 1 of file bigfile in 1 seconds
34947: 0.254: waiting for fileset pre-allocation to finish
34951: 0.254: Starting 1 filewriter instances
34952: 0.256: Starting 1 filewriterthread threads
34947: 1.256: Running...
34947: 11.258: Run took 10 seconds...
34947: 11.258: Per-Operation Breakdown
finish               0ops        0ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
write-file           820ops       82ops/s  81.9mb/s     12.1ms/op     2878us/op-cpu [5ms - 42ms]
34947: 11.258: IO Summary:   820 ops, 81.992 ops/s, (0/82 r/w),  81.9mb/s,  12866us cpu/op,  12.1ms latency
34947: 11.258: Shutting down processes

--------------------------------------------------------------------------------------------------------------------------

