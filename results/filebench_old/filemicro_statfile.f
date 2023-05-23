snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_statfile.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
35318: 0.000: Allocated 170MB of shared memory
35318: 0.001: Stat File Version 1.0 personality successfully loaded
35318: 0.001: Creating/pre-allocating files and filesets
35318: 0.021: Fileset bigfileset: 20000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.9, 2483.404MB
35318: 168.234: Removed any existing fileset bigfileset in 169 seconds
35318: 168.234: making tree for filset /dev/shm/test/client_test/bigfileset
35318: 169.172: Creating fileset bigfileset...
35318: 421.785: Preallocated 20000 of 20000 of fileset bigfileset in 253 seconds
35318: 421.785: waiting for fileset pre-allocation to finish
38018: 421.785: Starting 1 examinefiles instances
38019: 421.787: Starting 20 examinefilethread threads
35318: 422.793: Running...
35318: 432.794: Run took 10 seconds...
35318: 432.795: Per-Operation Breakdown
statfile1            19565ops     1956ops/s   0.0mb/s     10.1ms/op     2489us/op-cpu [2ms - 1296ms]
35318: 432.795: IO Summary: 19565 ops, 1956.320 ops/s, (0/0 r/w),   0.0mb/s,      0us cpu/op,   0.0ms latency
35318: 432.795: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_statfile.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
10107: 0.000: Allocated 170MB of shared memory
10107: 0.001: Stat File Version 1.0 personality successfully loaded
10107: 0.001: Creating/pre-allocating files and filesets
10107: 0.020: Fileset bigfileset: 20000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.9, 2483.404MB
10107: 72.922: Removed any existing fileset bigfileset in 73 seconds
10107: 72.922: making tree for filset /dev/shm/test/client_test/bigfileset
10107: 73.785: Creating fileset bigfileset...
10107: 330.797: Preallocated 20000 of 20000 of fileset bigfileset in 258 seconds
10107: 330.797: waiting for fileset pre-allocation to finish
12183: 330.797: Starting 1 examinefiles instances
12184: 330.799: Starting 20 examinefilethread threads
10107: 331.804: Running...
10107: 341.806: Run took 10 seconds...
10107: 341.806: Per-Operation Breakdown
statfile1            25469ops     2547ops/s   0.0mb/s      7.7ms/op     2476us/op-cpu [2ms - 201ms]
10107: 341.806: IO Summary: 25469 ops, 2546.688 ops/s, (0/0 r/w),   0.0mb/s,      0us cpu/op,   0.0ms latency
10107: 341.806: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_statfile.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
29118: 0.000: Allocated 170MB of shared memory
29118: 0.001: Stat File Version 1.0 personality successfully loaded
29118: 0.001: Creating/pre-allocating files and filesets
29118: 0.020: Fileset bigfileset: 20000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.9, 2483.404MB
29118: 73.160: Removed any existing fileset bigfileset in 74 seconds
29118: 73.160: making tree for filset /dev/shm/test/client_test/bigfileset
29118: 74.042: Creating fileset bigfileset...
29118: 330.278: Preallocated 20000 of 20000 of fileset bigfileset in 257 seconds
29118: 330.278: waiting for fileset pre-allocation to finish
31225: 330.278: Starting 1 examinefiles instances
31226: 330.281: Starting 20 examinefilethread threads
29118: 331.286: Running...
29118: 341.287: Run took 10 seconds...
29118: 341.287: Per-Operation Breakdown
statfile1            20012ops     2001ops/s   0.0mb/s      9.9ms/op     2480us/op-cpu [3ms - 1229ms]
29118: 341.287: IO Summary: 20012 ops, 2001.025 ops/s, (0/0 r/w),   0.0mb/s,      0us cpu/op,   0.0ms latency
29118: 341.287: Shutting down processes

--------------------------------------------------------------------------------------------------------------------------

