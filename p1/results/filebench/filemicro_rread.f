snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rread.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
29950: 0.000: Allocated 170MB of shared memory
29950: 0.001: FileMicro-ReadRand Version 2.2 personality successfully loaded
29950: 0.001: Creating/pre-allocating files and filesets
29950: 0.004: File bigfile1: 4096.000MB
29950: 0.012: Removed any existing file bigfile1 in 1 seconds
29950: 0.012: making tree for filset /dev/shm/test/client_test/bigfile1
29950: 0.025: Creating file bigfile1...
29950: 61.236: Preallocated 1 of 1 of file bigfile1 in 62 seconds
29950: 61.236: waiting for fileset pre-allocation to finish
30128: 61.236: Starting 1 filereader instances
30129: 61.238: Starting 1 filereaderthread threads
29950: 62.239: Running...
29950: 64.239: Run took 2 seconds...
29950: 64.240: Per-Operation Breakdown
finish               17950ops     8974ops/s   0.0mb/s      0.0ms/op        8us/op-cpu [0ms - 0ms]
write-file           17952ops     8975ops/s  17.5mb/s      0.1ms/op       43us/op-cpu [0ms - 278ms]
29950: 64.240: IO Summary: 17952 ops, 8974.815 ops/s, (8975/0 r/w),  17.5mb/s,    119us cpu/op,   0.1ms latency
29950: 64.240: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rread.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
30289: 0.000: Allocated 170MB of shared memory
30289: 0.001: FileMicro-ReadRand Version 2.2 personality successfully loaded
30289: 0.001: Creating/pre-allocating files and filesets
30289: 0.004: File bigfile1: 4096.000MB
30289: 0.004: Re-using file bigfile1.
30289: 0.004: Creating file bigfile1...
30289: 54.330: Preallocated 1 of 1 of file bigfile1 in 55 seconds
30289: 54.330: waiting for fileset pre-allocation to finish
30365: 54.330: Starting 1 filereader instances
30366: 54.332: Starting 1 filereaderthread threads
30289: 55.333: Running...
30289: 57.334: Run took 2 seconds...
30289: 57.334: Per-Operation Breakdown
finish               15018ops     7508ops/s   0.0mb/s      0.0ms/op       11us/op-cpu [0ms - 0ms]
write-file           15019ops     7508ops/s  14.5mb/s      0.1ms/op       45us/op-cpu [0ms - 300ms]
30289: 57.334: IO Summary: 15019 ops, 7508.434 ops/s, (7508/0 r/w),  14.5mb/s,    147us cpu/op,   0.1ms latency
30289: 57.334: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rread.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
30825: 0.000: Allocated 170MB of shared memory
30825: 0.001: FileMicro-ReadRand Version 2.2 personality successfully loaded
30825: 0.001: Creating/pre-allocating files and filesets
30825: 0.004: File bigfile1: 4096.000MB
30825: 0.004: Re-using file bigfile1.
30825: 0.004: Creating file bigfile1...
30825: 54.607: Preallocated 1 of 1 of file bigfile1 in 55 seconds
30825: 54.607: waiting for fileset pre-allocation to finish
30899: 54.607: Starting 1 filereader instances
30900: 54.609: Starting 1 filereaderthread threads
30825: 55.610: Running...
30825: 57.611: Run took 2 seconds...
30825: 57.611: Per-Operation Breakdown
finish               14023ops     7010ops/s   0.0mb/s      0.0ms/op        7us/op-cpu [0ms - 0ms]
write-file           14024ops     7011ops/s  13.5mb/s      0.1ms/op       46us/op-cpu [0ms - 302ms]
30825: 57.611: IO Summary: 14024 ops, 7010.973 ops/s, (7011/0 r/w),  13.5mb/s,    163us cpu/op,   0.1ms latency
30825: 57.611: Shutting down processe