snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_create.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
  760: 0.000: Allocated 170MB of shared memory
  760: 0.001: FileMicro-Create Version 2.1 personality successfully loaded
  760: 0.001: Creating/pre-allocating files and filesets
  760: 0.004: File largefile: 0.000MB
rm: cannot remove '/users/snehalw/unreliable_test/t1/largefile': Directory not empty
  760: 0.009: Removed any existing file largefile in 1 seconds
  760: 0.009: making tree for filset /users/snehalw/unreliable_test/t1/largefile
  760: 0.011: Creating file largefile...
  760: 0.019: Preallocated 1 of 1 of file largefile in 1 seconds
  760: 0.019: waiting for fileset pre-allocation to finish
  766: 0.019: Starting 1 filecreater instances
  767: 0.021: Starting 1 filecreaterthread threads
  760: 1.021: Running...
  760: 11.023: Run took 10 seconds...
  760: 11.023: Per-Operation Breakdown
finish               949ops       95ops/s   0.0mb/s      0.0ms/op       42us/op-cpu [0ms - 0ms]
append-file          950ops       95ops/s  94.9mb/s     10.1ms/op     2663us/op-cpu [6ms - 17ms]
  760: 11.023: IO Summary:   950 ops, 94.989 ops/s, (0/95 r/w),  94.9mb/s,  11653us cpu/op,  10.1ms latency

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_create.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 1426: 0.000: Allocated 170MB of shared memory
 1426: 0.001: FileMicro-Create Version 2.1 personality successfully loaded
 1426: 0.001: Creating/pre-allocating files and filesets
 1426: 0.004: File largefile: 0.000MB
rm: cannot remove '/users/snehalw/unreliable_test/t1/largefile': Directory not empty
 1426: 0.009: Removed any existing file largefile in 1 seconds
 1426: 0.009: making tree for filset /users/snehalw/unreliable_test/t1/largefile
 1426: 0.010: Creating file largefile...
 1426: 0.018: Preallocated 1 of 1 of file largefile in 1 seconds
 1426: 0.018: waiting for fileset pre-allocation to finish
 1430: 0.018: Starting 1 filecreater instances
 1431: 0.020: Starting 1 filecreaterthread threads
 1426: 1.020: Running...
 1426: 11.021: Run took 10 seconds...
 1426: 11.022: Per-Operation Breakdown
finish               847ops       85ops/s   0.0mb/s      0.0ms/op       24us/op-cpu [0ms - 0ms]
append-file          848ops       85ops/s  84.7mb/s     11.2ms/op     2901us/op-cpu [7ms - 16ms]
 1426: 11.022: IO Summary:   848 ops, 84.791 ops/s, (0/85 r/w),  84.7mb/s,  13160us cpu/op,  11.2ms latency

filebench -f filemicro_create.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 1619: 0.000: Allocated 170MB of shared memory
 1619: 0.001: FileMicro-Create Version 2.1 personality successfully loaded
 1619: 0.001: Creating/pre-allocating files and filesets
 1619: 0.004: File largefile: 0.000MB
rm: cannot remove '/users/snehalw/unreliable_test/t1/largefile': Directory not empty
 1619: 0.009: Removed any existing file largefile in 1 seconds
 1619: 0.009: making tree for filset /users/snehalw/unreliable_test/t1/largefile
 1619: 0.010: Creating file largefile...
 1619: 0.019: Preallocated 1 of 1 of file largefile in 1 seconds
 1619: 0.019: waiting for fileset pre-allocation to finish
 1623: 0.019: Starting 1 filecreater instances
 1624: 0.021: Starting 1 filecreaterthread threads
 1619: 1.021: Running...
 1619: 11.023: Run took 10 seconds...
 1619: 11.023: Per-Operation Breakdown
finish               808ops       81ops/s   0.0mb/s      0.0ms/op       25us/op-cpu [0ms - 0ms]
append-file          809ops       81ops/s  80.8mb/s     12.0ms/op     2843us/op-cpu [6ms - 1172ms]
 1619: 11.023: IO Summary:   809 ops, 80.891 ops/s, (0/81 r/w),  80.8mb/s,  12398us cpu/op,  12.0ms latency


--------------------------------------------------------------------------------------------------------------------------

