snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rwritedsync.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-WriteRandDsync Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile tree (if exists)
0.013: Pre-allocating directories in bigfile tree
0.026: Pre-allocating files in bigfile tree
32.933: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
32.933: Population and pre-allocation of filesets completed
32.933: Starting 1 filewriter instances
33.939: Running...
38.940: Run took 5 seconds...
38.940: Per-Operation Breakdown
finish               32768ops     6553ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.02ms]
write-file           32769ops     6553ops/s  12.8mb/s      0.1ms/op [0.10ms - 103.07ms]
38.940: IO Summary: 32769 ops 6553.191 ops/s 0/6553 rd/wr  12.8mb/s   0.1ms/op
38.940: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rwritedsync.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-WriteRandDsync Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile tree (if exists)
0.013: Pre-allocating directories in bigfile tree
0.030: Pre-allocating files in bigfile tree
29.717: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
29.717: Population and pre-allocation of filesets completed
29.717: Starting 1 filewriter instances
30.723: Running...
35.724: Run took 5 seconds...
35.724: Per-Operation Breakdown
finish               32768ops     6553ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.01ms]
write-file           32769ops     6553ops/s  12.8mb/s      0.1ms/op [0.10ms - 106.83ms]
35.724: IO Summary: 32769 ops 6552.883 ops/s 0/6553 rd/wr  12.8mb/s   0.1ms/op
35.724: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rwritedsync.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-WriteRandDsync Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile tree (if exists)
0.014: Pre-allocating directories in bigfile tree
0.030: Pre-allocating files in bigfile tree
30.063: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
30.063: Population and pre-allocation of filesets completed
30.063: Starting 1 filewriter instances
31.069: Running...
36.070: Run took 5 seconds...
36.070: Per-Operation Breakdown
finish               32768ops     6553ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.01ms]
write-file           32769ops     6553ops/s  12.8mb/s      0.1ms/op [0.10ms - 100.69ms]
36.070: IO Summary: 32769 ops 6553.090 ops/s 0/6553 rd/wr  12.8mb/s   0.1ms/op
36.070: Shutting down processes
