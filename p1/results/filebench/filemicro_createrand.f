Filebench Version 1.5-alpha3
0.000: Allocated 173MB of shared memory
0.002: FileMicro-CreateRand Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.003: Removing largefile tree (if exists)
0.015: Pre-allocating directories in largefile tree
0.018: Pre-allocating files in largefile tree
0.060: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.060: Population and pre-allocation of filesets completed
0.060: Starting 1 filecreater instances
1.066: Running...
11.067: Run took 10 seconds...
11.067: Per-Operation Breakdown
finish               90ops        9ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync                 90ops        9ops/s   0.0mb/s      0.1ms/op [0.06ms -  0.09ms]
append-file          906ops       91ops/s  44.9mb/s     11.0ms/op [0.15ms - 85.80ms]
11.067: IO Summary:   996 ops 99.586 ops/s 0/91 rd/wr  44.9mb/s  11.0ms/op
11.067: Shutting down processes
Workload: filemicro_createrand.f: ------- Written 0th iteration-------
Filebench Version 1.5-alpha3
0.000: Allocated 173MB of shared memory
0.002: FileMicro-CreateRand Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.003: Removing largefile tree (if exists)
0.014: Pre-allocating directories in largefile tree
0.018: Pre-allocating files in largefile tree
0.059: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.059: Population and pre-allocation of filesets completed
0.059: Starting 1 filecreater instances
1.064: Running...
11.066: Run took 10 seconds...
11.066: Per-Operation Breakdown
finish               93ops        9ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync                 93ops        9ops/s   0.0mb/s      0.1ms/op [0.06ms -  0.08ms]
append-file          937ops       94ops/s  46.5mb/s     10.6ms/op [0.14ms - 51.25ms]
11.066: IO Summary:  1030 ops 102.986 ops/s 0/94 rd/wr  46.5mb/s  10.6ms/op
11.066: Shutting down processes
Workload: filemicro_createrand.f: ------- Written 1th iteration-------
Filebench Version 1.5-alpha3
0.000: Allocated 173MB of shared memory
0.002: FileMicro-CreateRand Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.003: Removing largefile tree (if exists)
0.014: Pre-allocating directories in largefile tree
0.018: Pre-allocating files in largefile tree
0.062: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.063: Population and pre-allocation of filesets completed
0.063: Starting 1 filecreater instances
1.068: Running...
11.069: Run took 10 seconds...
11.070: Per-Operation Breakdown
finish               94ops        9ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync                 94ops        9ops/s   0.0mb/s      0.1ms/op [0.06ms -  0.08ms]
append-file          947ops       95ops/s  47.1mb/s     10.5ms/op [0.13ms - 60.65ms]
11.070: IO Summary:  1041 ops 104.088 ops/s 0/95 rd/wr  47.1mb/s  10.5ms/op
11.070: Shutting down processes
Workload: filemicro_createrand.f: ------- Written 2th iteration-------

================================================================================================
DIFFERENT NODES
================================================================================================

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_createrand.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-CreateRand Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.004: Removing largefile tree (if exists)
0.170: Pre-allocating directories in largefile tree
0.184: Pre-allocating files in largefile tree
0.188: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.188: Population and pre-allocation of filesets completed
0.189: Starting 1 filecreater instances
1.194: Running...
11.196: Run took 10 seconds...
11.196: Per-Operation Breakdown
finish               92ops        9ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync                 92ops        9ops/s   0.0mb/s      0.1ms/op [0.06ms -  0.07ms]
append-file          925ops       92ops/s  45.9mb/s     10.8ms/op [0.14ms - 177.37ms]
11.196: IO Summary:  1017 ops 101.687 ops/s 0/92 rd/wr  45.9mb/s  10.8ms/op
11.196: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_createrand.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-CreateRand Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing largefile tree (if exists)
0.204: Pre-allocating directories in largefile tree
0.217: Pre-allocating files in largefile tree
0.221: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.221: Population and pre-allocation of filesets completed
0.222: Starting 1 filecreater instances
1.227: Running...
11.229: Run took 10 seconds...
11.229: Per-Operation Breakdown
finish               76ops        8ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync                 76ops        8ops/s   0.0mb/s      0.6ms/op [0.05ms - 40.79ms]
append-file          770ops       77ops/s  37.8mb/s     12.9ms/op [0.15ms - 117.47ms]
11.229: IO Summary:   846 ops 84.588 ops/s 0/77 rd/wr  37.8mb/s  12.9ms/op
11.229: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_createrand.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-CreateRand Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.004: Removing largefile tree (if exists)
0.177: Pre-allocating directories in largefile tree
0.191: Pre-allocating files in largefile tree
0.195: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.195: Population and pre-allocation of filesets completed
0.196: Starting 1 filecreater instances
1.200: Running...
11.201: Run took 10 seconds...
11.201: Per-Operation Breakdown
finish               71ops        7ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync                 71ops        7ops/s   0.0mb/s      0.1ms/op [0.05ms -  0.16ms]
append-file          715ops       71ops/s  35.5mb/s     13.9ms/op [0.15ms - 140.95ms]
11.201: IO Summary:   786 ops 78.592 ops/s 0/71 rd/wr  35.5mb/s  14.0ms/op
11.201: Shutting down processes