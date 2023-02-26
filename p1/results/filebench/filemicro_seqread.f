snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqread.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-SeqRead Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing largefile tree (if exists)
0.013: Pre-allocating directories in largefile tree
0.026: Pre-allocating files in largefile tree
30.346: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
30.346: Population and pre-allocation of filesets completed
30.346: Starting 1 filereader instances
31.352: Running...
41.354: Run took 10 seconds...
41.354: Per-Operation Breakdown
seqread-file         56231ops     5622ops/s 5616.7mb/s      0.2ms/op [0.00ms - 100.17ms]
41.354: IO Summary: 56231 ops 5622.234 ops/s 5622/0 rd/wr 5616.7mb/s   0.2ms/op
41.354: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqread.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-SeqRead Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.004: Reusing existing largefile tree
0.004: Pre-allocating files in largefile tree
0.092: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.092: Population and pre-allocation of filesets completed
0.093: Starting 1 filereader instances
1.098: Running...
11.100: Run took 10 seconds...
11.100: Per-Operation Breakdown
seqread-file         57196ops     5719ops/s 5713.2mb/s      0.2ms/op [0.00ms -  7.99ms]
11.100: IO Summary: 57196 ops 5718.794 ops/s 5719/0 rd/wr 5713.2mb/s   0.2ms/op
11.100: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqread.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-SeqRead Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.004: Reusing existing largefile tree
0.004: Pre-allocating files in largefile tree
0.098: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.098: Population and pre-allocation of filesets completed
0.098: Starting 1 filereader instances
1.103: Running...
11.104: Run took 10 seconds...
11.104: Per-Operation Breakdown
seqread-file         57681ops     5768ops/s 5761.8mb/s      0.2ms/op [0.00ms -  8.42ms]
11.105: IO Summary: 57681 ops 5767.546 ops/s 5768/0 rd/wr 5761.8mb/s   0.2ms/op
11.105: Shutting down processes

