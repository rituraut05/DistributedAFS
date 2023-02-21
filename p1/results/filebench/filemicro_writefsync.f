snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_writefsync.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 4367: 0.000: Allocated 170MB of shared memory
 4367: 0.001: FileMicro-WriteFsync Version 2.1 personality successfully loaded
 4367: 0.001: Creating/pre-allocating files and filesets
 4367: 0.004: File bigfile: 0.000MB
 4367: 0.211: Removed any existing file bigfile in 1 seconds
 4367: 0.211: making tree for filset /dev/shm/test/client_test/bigfile
 4367: 0.222: Creating file bigfile...
 4367: 0.230: Preallocated 1 of 1 of file bigfile in 1 seconds
 4367: 0.230: waiting for fileset pre-allocation to finish
 4372: 0.231: Starting 1 filewriter instances
 4373: 0.232: Starting 1 filewriterthread threads
 4367: 1.233: Running...
 4367: 71.240: Run took 70 seconds...
 4367: 71.240: Per-Operation Breakdown
finish               128ops        2ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
sync-file            128ops        2ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
append-file          131073ops     1872ops/s  14.6mb/s      0.5ms/op       54us/op-cpu [0ms - 1350ms]
 4367: 71.240: IO Summary: 131201 ops, 1874.124 ops/s, (0/1872 r/w),  14.6mb/s,    745us cpu/op,   0.5ms latency
 4367: 71.240: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_writefsync.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 5011: 0.000: Allocated 170MB of shared memory
 5011: 0.001: FileMicro-WriteFsync Version 2.1 personality successfully loaded
 5011: 0.001: Creating/pre-allocating files and filesets
 5011: 0.004: File bigfile: 0.000MB
 5011: 0.350: Removed any existing file bigfile in 1 seconds
 5011: 0.350: making tree for filset /dev/shm/test/client_test/bigfile
 5011: 0.366: Creating file bigfile...
 5011: 0.371: Preallocated 1 of 1 of file bigfile in 1 seconds
 5011: 0.371: waiting for fileset pre-allocation to finish
 5016: 0.371: Starting 1 filewriter instances
 5017: 0.373: Starting 1 filewriterthread threads
 5011: 1.373: Running...
 5011: 81.381: Run took 80 seconds...
 5011: 81.381: Per-Operation Breakdown
finish               128ops        2ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
sync-file            128ops        2ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
append-file          131073ops     1638ops/s  12.8mb/s      0.6ms/op       55us/op-cpu [0ms - 1302ms]
 5011: 81.381: IO Summary: 131201 ops, 1639.862 ops/s, (0/1638 r/w),  12.8mb/s,    746us cpu/op,   0.6ms latency
 5011: 81.381: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_writefsync.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 5553: 0.000: Allocated 170MB of shared memory
 5553: 0.001: FileMicro-WriteFsync Version 2.1 personality successfully loaded
 5553: 0.001: Creating/pre-allocating files and filesets
 5553: 0.004: File bigfile: 0.000MB
 5553: 0.327: Removed any existing file bigfile in 1 seconds
 5553: 0.327: making tree for filset /dev/shm/test/client_test/bigfile
 5553: 0.340: Creating file bigfile...
 5553: 0.346: Preallocated 1 of 1 of file bigfile in 1 seconds
 5553: 0.346: waiting for fileset pre-allocation to finish
 5561: 0.346: Starting 1 filewriter instances
 5562: 0.347: Starting 1 filewriterthread threads
 5553: 1.348: Running...

 5553: 66.355: Run took 65 seconds...
 5553: 66.356: Per-Operation Breakdown
finish               128ops        2ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
sync-file            128ops        2ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
append-file          131073ops     2016ops/s  15.8mb/s      0.5ms/op       54us/op-cpu [0ms - 1196ms]
 5553: 66.356: IO Summary: 131201 ops, 2018.263 ops/s, (0/2016 r/w),  15.8mb/s,    736us cpu/op,   0.5ms latency
 5553: 66.356: Shutting down processes