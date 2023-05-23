snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rwritedsync.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
31116: 0.000: Allocated 170MB of shared memory
31116: 0.001: FileMicro-WriteRandDsync Version 2.1 personality successfully loaded
31116: 0.001: Creating/pre-allocating files and filesets
31116: 0.004: File bigfile: 1024.000MB
31116: 0.012: Removed any existing file bigfile in 1 seconds
31116: 0.012: making tree for filset /dev/shm/test/client_test/bigfile
31116: 0.026: Creating file bigfile...
31116: 35.272: Preallocated 1 of 1 of file bigfile in 36 seconds
31116: 35.272: waiting for fileset pre-allocation to finish
31349: 35.272: Starting 1 filewriter instances
31350: 35.273: Starting 1 filewriterthread threads
31116: 36.274: Running...
31116: 41.275: Run took 5 seconds...
31116: 41.275: Per-Operation Breakdown
finish               32768ops     6553ops/s   0.0mb/s      0.0ms/op       13us/op-cpu [0ms - 0ms]
write-file           32769ops     6553ops/s  12.8mb/s      0.1ms/op       33us/op-cpu [0ms - 107ms]
31116: 41.275: IO Summary: 32769 ops, 6553.057 ops/s, (0/6553 r/w),  12.8mb/s,    135us cpu/op,   0.1ms latency
31116: 41.275: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rwritedsync.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
31655: 0.000: Allocated 170MB of shared memory
31655: 0.001: FileMicro-WriteRandDsync Version 2.1 personality successfully loaded
31655: 0.001: Creating/pre-allocating files and filesets
31655: 0.004: File bigfile: 1024.000MB
31655: 0.004: Re-using file bigfile.
31655: 0.004: Creating file bigfile...
31655: 21.331: Preallocated 1 of 1 of file bigfile in 22 seconds
31655: 21.331: waiting for fileset pre-allocation to finish
31834: 21.331: Starting 1 filewriter instances
31835: 21.332: Starting 1 filewriterthread threads
31655: 22.333: Running...
31655: 27.334: Run took 5 seconds...
31655: 27.334: Per-Operation Breakdown
finish               28603ops     5720ops/s   0.0mb/s      0.0ms/op       14us/op-cpu [0ms - 0ms]
write-file           28604ops     5720ops/s  11.0mb/s      0.1ms/op       31us/op-cpu [0ms - 22ms]
31655: 27.334: IO Summary: 28604 ops, 5720.143 ops/s, (0/5720 r/w),  11.0mb/s,    169us cpu/op,   0.1ms latency
31655: 27.334: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_rwritedsync.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
32138: 0.000: Allocated 170MB of shared memory
32138: 0.001: FileMicro-WriteRandDsync Version 2.1 personality successfully loaded
32138: 0.001: Creating/pre-allocating files and filesets
32138: 0.004: File bigfile: 1024.000MB
32138: 0.004: Re-using file bigfile.
32138: 0.004: Creating file bigfile...
32138: 23.489: Preallocated 1 of 1 of file bigfile in 24 seconds
32138: 23.489: waiting for fileset pre-allocation to finish
32324: 23.489: Starting 1 filewriter instances
32325: 23.491: Starting 1 filewriterthread threads
32138: 24.491: Running...
32138: 29.492: Run took 5 seconds...
32138: 29.493: Per-Operation Breakdown
finish               32768ops     6553ops/s   0.0mb/s      0.0ms/op       13us/op-cpu [0ms - 0ms]
write-file           32769ops     6553ops/s  12.8mb/s      0.1ms/op       34us/op-cpu [0ms - 21ms]
32138: 29.493: IO Summary: 32769 ops, 6553.006 ops/s, (0/6553 r/w),  12.8mb/s,    127us cpu/op,   0.1ms latency
32138: 29.493: Shutting down processes

--------------------------------------------------------------------------------------------------------------------------

