snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_statfile.f
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: Stat File Version 1.0 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.013: bigfileset populated: 20000 files, avg. dir. width = 200, avg. dir. depth = 1.9, 0 leafdirs, 2500.000MB total size
0.014: Removing bigfileset tree (if exists)
0.022: Pre-allocating directories in bigfileset tree
0.929: Pre-allocating files in bigfileset tree
162.734: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
162.734: Population and pre-allocation of filesets completed
162.735: Starting 1 examinefiles instances
163.743: Running...
173.744: Run took 10 seconds...
173.744: Per-Operation Breakdown
statfile1            3230ops      323ops/s   0.0mb/s     61.5ms/op [1.52ms - 89.01ms]
173.744: IO Summary:  3230 ops 322.970 ops/s 0/0 rd/wr   0.0mb/s   0.0ms/op
173.744: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_statfile.f
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: Stat File Version 1.0 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.014: bigfileset populated: 20000 files, avg. dir. width = 200, avg. dir. depth = 1.9, 0 leafdirs, 2500.000MB total size
0.014: Removing bigfileset tree (if exists)
rm: cannot remove '/dev/shm/backup/clientdir/bigfileset': Directory not empty
0.026: Pre-allocating directories in bigfileset tree
0.234: Pre-allocating files in bigfileset tree
322.156: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
322.156: Population and pre-allocation of filesets completed
322.157: Starting 1 examinefiles instances
323.164: Running...
333.165: Per-Operation Breakdown
statfile1            3095ops      309ops/s   0.0mb/s     64.2ms/op [1.56ms - 90.56ms]
333.166: IO Summary:  3095 ops 309.466 ops/s 0/0 rd/wr   0.0mb/s   0.0ms/op
333.166: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_statfile.f
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: Stat File Version 1.0 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.013: bigfileset populated: 20000 files, avg. dir. width = 200, avg. dir. depth = 1.9, 0 leafdirs, 2500.000MB total size
0.015: Removing bigfileset tree (if exists)
0.023: Pre-allocating directories in bigfileset tree
1.016: Pre-allocating files in bigfileset tree
139.444: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
139.444: Population and pre-allocation of filesets completed
139.444: Starting 1 examinefiles instances
140.452: Running...
150.453: Per-Operation Breakdown
statfile1            3138ops      314ops/s   0.0mb/s     63.3ms/op [1.55ms - 92.58ms]
150.453: IO Summary:  3138 ops 313.763 ops/s 0/0 rd/wr   0.0mb/s   0.0ms/op
150.453: Shutting down processes