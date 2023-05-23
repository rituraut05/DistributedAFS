snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_delete.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Delete Version 2.4 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.049: bigfileset populated: 50000 files, avg. dir. width = 100, avg. dir. depth = 2.3, 0 leafdirs, 781.250MB total size
0.051: Removing bigfileset tree (if exists)
0.058: Pre-allocating directories in bigfileset tree
4.275: Pre-allocating files in bigfileset tree
49.724: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
49.795: Population and pre-allocation of filesets completed
49.796: Starting 1 filedelete instances
50.803: Running...
60.805: Run took 10 seconds...
60.805: Per-Operation Breakdown
finish               1860ops      186ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.02ms]
limit                0ops        0ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
deletefile1          1860ops      186ops/s   0.0mb/s     85.0ms/op [3.57ms - 194.84ms]
60.805: IO Summary:  1860 ops 185.980 ops/s 0/0 rd/wr   0.0mb/s   0.0ms/op
60.805: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_delete.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Delete Version 2.4 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.027: bigfileset populated: 50000 files, avg. dir. width = 100, avg. dir. depth = 2.3, 0 leafdirs, 781.250MB total size
0.029: Removing bigfileset tree (if exists)
0.036: Pre-allocating directories in bigfileset tree
4.302: Pre-allocating files in bigfileset tree
50.997: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
51.068: Population and pre-allocation of filesets completed
51.069: Starting 1 filedelete instances
52.077: Running...
62.079: Run took 10 seconds...
62.079: Per-Operation Breakdown
finish               1801ops      180ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.02ms]
limit                0ops        0ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
deletefile1          1801ops      180ops/s   0.0mb/s     87.9ms/op [3.52ms - 213.22ms]
62.079: IO Summary:  1801 ops 180.076 ops/s 0/0 rd/wr   0.0mb/s   0.0ms/op
62.079: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_delete.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Delete Version 2.4 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.027: bigfileset populated: 50000 files, avg. dir. width = 100, avg. dir. depth = 2.3, 0 leafdirs, 781.250MB total size
0.029: Removing bigfileset tree (if exists)
0.036: Pre-allocating directories in bigfileset tree
4.216: Pre-allocating files in bigfileset tree
49.217: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
49.293: Population and pre-allocation of filesets completed
49.293: Starting 1 filedelete instances
50.301: Running...
60.303: Run took 10 seconds...
60.303: Per-Operation Breakdown
finish               1824ops      182ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.02ms]
limit                0ops        0ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
deletefile1          1824ops      182ops/s   0.0mb/s     86.9ms/op [3.58ms - 207.03ms]
60.303: IO Summary:  1824 ops 182.375 ops/s 0/0 rd/wr   0.0mb/s   0.0ms/op
60.303: Shutting down processes
