Filebench Version 1.5-alpha3
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Create Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing largefile tree (if exists)
0.012: Pre-allocating directories in largefile tree
0.022: Pre-allocating files in largefile tree
0.025: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.025: Population and pre-allocation of filesets completed
0.026: Starting 1 filecreater instances
1.031: Running...
11.032: Run took 10 seconds...
11.033: Per-Operation Breakdown
finish               494ops       49ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
append-file          495ops       49ops/s  49.4mb/s     20.2ms/op [6.23ms - 22.60ms]
11.033: IO Summary:   495 ops 49.494 ops/s 0/49 rd/wr  49.4mb/s  20.2ms/op
11.033: Shutting down processes
Workload: filemicro_create.f: ------- Written 0th iteration-------

Filebench Version 1.5-alpha3
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Create Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.004: Removing largefile tree (if exists)
0.185: Pre-allocating directories in largefile tree
0.197: Pre-allocating files in largefile tree
0.200: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.200: Population and pre-allocation of filesets completed
0.201: Starting 1 filecreater instances
1.206: Running...
11.207: Run took 10 seconds...
11.207: Per-Operation Breakdown
finish               494ops       49ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
append-file          495ops       49ops/s  49.4mb/s     20.1ms/op [5.63ms - 83.18ms]
11.207: IO Summary:   495 ops 49.493 ops/s 0/49 rd/wr  49.4mb/s  20.1ms/op
11.207: Shutting down processes
Workload: filemicro_create.f: ------- Written 1th iteration-------

Filebench Version 1.5-alpha3
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Create Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.004: Removing largefile tree (if exists)
0.181: Pre-allocating directories in largefile tree
0.193: Pre-allocating files in largefile tree
0.197: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.197: Population and pre-allocation of filesets completed
0.197: Starting 1 filecreater instances
1.200: Running...
11.202: Run took 10 seconds...
11.202: Per-Operation Breakdown
finish               497ops       50ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
append-file          498ops       50ops/s  49.7mb/s     20.0ms/op [6.44ms - 57.74ms]
11.202: IO Summary:   498 ops 49.794 ops/s 0/50 rd/wr  49.7mb/s  20.0ms/op
11.202: Shutting down processes
Workload: filemicro_create.f: ------- Written 2th iteration-------

================================================================================================
DIFFERENT NODES
================================================================================================

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_create.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Create Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.009: Removing largefile tree (if exists)
0.019: Pre-allocating directories in largefile tree
0.041: Pre-allocating files in largefile tree
0.046: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.046: Population and pre-allocation of filesets completed
0.047: Starting 1 filecreater instances
1.052: Running...
11.054: Run took 10 seconds...
11.054: Per-Operation Breakdown
finish               469ops       47ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
append-file          470ops       47ops/s  46.9mb/s     21.2ms/op [16.54ms - 190.79ms]
11.054: IO Summary:   470 ops 46.994 ops/s 0/47 rd/wr  46.9mb/s  21.2ms/op
11.054: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_create.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Create Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing largefile tree (if exists)
0.203: Pre-allocating directories in largefile tree
0.220: Pre-allocating files in largefile tree
0.224: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.224: Population and pre-allocation of filesets completed
0.225: Starting 1 filecreater instances
1.230: Running...
11.231: Run took 10 seconds...
11.231: Per-Operation Breakdown
finish               473ops       47ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
append-file          474ops       47ops/s  47.3mb/s     21.0ms/op [12.85ms - 56.87ms]
11.232: IO Summary:   474 ops 47.394 ops/s 0/47 rd/wr  47.3mb/s  21.0ms/op
11.232: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_create.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-Create Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing largefile tree (if exists)
0.206: Pre-allocating directories in largefile tree
0.224: Pre-allocating files in largefile tree
0.228: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.228: Population and pre-allocation of filesets completed
0.229: Starting 1 filecreater instances
1.235: Running...
11.236: Run took 10 seconds...
11.236: Per-Operation Breakdown
finish               469ops       47ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
append-file          470ops       47ops/s  46.9mb/s     21.2ms/op [12.71ms - 172.15ms]
11.236: IO Summary:   470 ops 46.995 ops/s 0/47 rd/wr  46.9mb/s  21.2ms/op
11.236: Shutting down processes
