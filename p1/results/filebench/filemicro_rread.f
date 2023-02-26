snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rread.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-ReadRand Version 2.2 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile1 tree (if exists)
0.014: Pre-allocating directories in bigfile1 tree
0.031: Pre-allocating files in bigfile1 tree
85.537: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
85.537: Population and pre-allocation of filesets completed
85.538: Starting 1 filereader instances
86.543: Running...
88.543: Run took 2 seconds...
88.544: Per-Operation Breakdown
finish               22388ops    11192ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
write-file           22389ops    11193ops/s  21.9mb/s      0.1ms/op [0.00ms - 317.22ms]
88.544: IO Summary: 22389 ops 11192.782 ops/s 11193/0 rd/wr  21.9mb/s   0.1ms/op
88.544: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rread.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-ReadRand Version 2.2 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile1 tree (if exists)
0.013: Pre-allocating directories in bigfile1 tree
0.030: Pre-allocating files in bigfile1 tree
84.850: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
84.851: Population and pre-allocation of filesets completed
84.851: Starting 1 filereader instances
85.857: Running...
87.857: Run took 2 seconds...
87.858: Per-Operation Breakdown
finish               22699ops    11348ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
write-file           22700ops    11349ops/s  22.2mb/s      0.1ms/op [0.00ms - 307.09ms]
87.858: IO Summary: 22700 ops 11348.678 ops/s 11349/0 rd/wr  22.2mb/s   0.1ms/op
87.858: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rread.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-ReadRand Version 2.2 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile1 tree (if exists)
0.013: Pre-allocating directories in bigfile1 tree
0.025: Pre-allocating files in bigfile1 tree
85.982: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
85.982: Population and pre-allocation of filesets completed
85.982: Starting 1 filereader instances
86.988: Running...
88.989: Run took 2 seconds...
88.989: Per-Operation Breakdown
finish               22141ops    11069ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.01ms]
write-file           22142ops    11070ops/s  21.6mb/s      0.1ms/op [0.00ms - 304.64ms]
88.989: IO Summary: 22142 ops 11069.517 ops/s 11070/0 rd/wr  21.6mb/s   0.1ms/op
88.989: Shutting down processes
