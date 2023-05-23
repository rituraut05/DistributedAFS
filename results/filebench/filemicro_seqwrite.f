snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqwrite.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-SeqWrite Version 2.2 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing bigfile tree (if exists)
0.012: Pre-allocating directories in bigfile tree
0.028: Pre-allocating files in bigfile tree
0.032: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.032: Population and pre-allocation of filesets completed
0.032: Starting 1 filewriter instances
1.038: Running...
11.039: Run took 10 seconds...
11.039: Per-Operation Breakdown
finish               0ops        0ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
write-file           476ops       48ops/s  47.5mb/s     20.9ms/op [12.69ms - 69.46ms]
11.039: IO Summary:   476 ops 47.595 ops/s 0/48 rd/wr  47.5mb/s  20.9ms/op
11.039: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqwrite.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-SeqWrite Version 2.2 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing bigfile tree (if exists)
0.212: Pre-allocating directories in bigfile tree
0.224: Pre-allocating files in bigfile tree
0.227: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.227: Population and pre-allocation of filesets completed
0.228: Starting 1 filewriter instances
1.232: Running...
11.233: Run took 10 seconds...
11.233: Per-Operation Breakdown
finish               0ops        0ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
write-file           450ops       45ops/s  44.9mb/s     22.2ms/op [8.73ms - 217.89ms]
11.233: IO Summary:   450 ops 44.995 ops/s 0/45 rd/wr  44.9mb/s  22.2ms/op
11.233: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqwrite.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-SeqWrite Version 2.2 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing bigfile tree (if exists)
0.192: Pre-allocating directories in bigfile tree
0.205: Pre-allocating files in bigfile tree
0.209: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.209: Population and pre-allocation of filesets completed
0.210: Starting 1 filewriter instances
1.215: Running...
11.216: Run took 10 seconds...
11.216: Per-Operation Breakdown
finish               0ops        0ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
write-file           494ops       49ops/s  49.3mb/s     20.2ms/op [6.86ms - 39.86ms]
11.216: IO Summary:   494 ops 49.396 ops/s 0/49 rd/wr  49.3mb/s  20.2ms/op
11.216: Shutting down processes