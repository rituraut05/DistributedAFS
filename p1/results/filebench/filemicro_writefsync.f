snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_writefsync.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-WriteFsync Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.006: Removing bigfile tree (if exists)
0.013: Pre-allocating directories in bigfile tree
0.028: Pre-allocating files in bigfile tree
0.032: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.032: Population and pre-allocation of filesets completed
0.032: Starting 1 filewriter instances
1.037: Running...
11.039: Run took 10 seconds...
11.039: Per-Operation Breakdown
finish               32ops        3ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync-file            32ops        3ops/s   0.0mb/s      0.1ms/op [0.04ms -  0.07ms]
append-file          33616ops     3361ops/s  26.3mb/s      0.3ms/op [0.10ms - 136.24ms]
11.039: IO Summary: 33648 ops 3364.368 ops/s 0/3361 rd/wr  26.3mb/s   0.3ms/op
11.039: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_writefsync.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-WriteFsync Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing bigfile tree (if exists)
0.161: Pre-allocating directories in bigfile tree
0.178: Pre-allocating files in bigfile tree
0.182: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.182: Population and pre-allocation of filesets completed
0.183: Starting 1 filewriter instances
1.188: Running...
11.189: Run took 10 seconds...
11.189: Per-Operation Breakdown
finish               34ops        3ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync-file            34ops        3ops/s   0.0mb/s      0.1ms/op [0.07ms -  0.08ms]
append-file          34957ops     3495ops/s  27.3mb/s      0.3ms/op [0.19ms - 62.62ms]
11.189: IO Summary: 34991 ops 3498.684 ops/s 0/3495 rd/wr  27.3mb/s   0.3ms/op
11.189: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_writefsync.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: FileMicro-WriteFsync Version 2.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: Removing bigfile tree (if exists)
0.159: Pre-allocating directories in bigfile tree
0.176: Pre-allocating files in bigfile tree
0.180: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
0.180: Population and pre-allocation of filesets completed
0.181: Starting 1 filewriter instances
1.186: Running...
11.187: Run took 10 seconds...
11.188: Per-Operation Breakdown
finish               40ops        4ops/s   0.0mb/s      0.0ms/op [0.00ms -  0.00ms]
sync-file            40ops        4ops/s   0.0mb/s      0.1ms/op [0.05ms -  0.09ms]
append-file          41971ops     4197ops/s  32.8mb/s      0.2ms/op [0.17ms - 12.88ms]
11.188: IO Summary: 42011 ops 4200.671 ops/s 0/4197 rd/wr  32.8mb/s   0.2ms/op
11.188: Shutting down processes

