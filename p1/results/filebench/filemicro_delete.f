snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_delete.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
  481: 0.000: Allocated 170MB of shared memory
  481: 0.001: FileMicro-Delete Version 2.4 personality successfully loaded
  481: 0.001: Creating/pre-allocating files and filesets
  481: 0.045: Fileset bigfileset: 50000 files, 0 leafdirs, avg dir width = 100, avg dir depth = 2.3, 778.397MB

  481: 350.448: Removed any existing fileset bigfileset in 351 seconds
  481: 350.448: making tree for filset /dev/shm/test/client_test/bigfileset
  481: 354.639: Creating fileset bigfileset...
  481: 421.135: Preallocated 50000 of 50000 of fileset bigfileset in 67 seconds
  481: 421.135: waiting for fileset pre-allocation to finish
23034: 421.180: Starting 1 filedelete instances
23035: 421.182: Starting 16 filedeletethread threads
  481: 422.186: Running...
  481: 438.188: Run took 16 seconds...
  481: 438.189: Per-Operation Breakdown
finish               5001ops      313ops/s   0.0mb/s      0.0ms/op       30us/op-cpu [0ms - 0ms]
limit                0ops        0ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
deletefile1          5016ops      313ops/s   0.0mb/s     50.5ms/op     4288us/op-cpu [3ms - 80ms]
  481: 438.189: IO Summary:  5016 ops, 313.468 ops/s, (0/0 r/w),   0.0mb/s,      0us cpu/op,   0.0ms latency
  481: 438.189: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_delete.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
24089: 0.000: Allocated 170MB of shared memory
24089: 0.001: FileMicro-Delete Version 2.4 personality successfully loaded
24089: 0.001: Creating/pre-allocating files and filesets
24089: 0.045: Fileset bigfileset: 50000 files, 0 leafdirs, avg dir width = 100, avg dir depth = 2.3, 778.397MB
24089: 167.681: Removed any existing fileset bigfileset in 168 seconds
24089: 167.682: making tree for filset /dev/shm/test/client_test/bigfileset
24089: 171.975: Creating fileset bigfileset...
24089: 252.258: Preallocated 50000 of 50000 of fileset bigfileset in 81 seconds
24089: 252.258: waiting for fileset pre-allocation to finish
 5795: 252.326: Starting 1 filedelete instances
 5796: 252.329: Starting 16 filedeletethread threads
24089: 253.333: Running...
24089: 269.335: Run took 16 seconds...
24089: 269.335: Per-Operation Breakdown
finish               5001ops      313ops/s   0.0mb/s      0.0ms/op       14us/op-cpu [0ms - 0ms]
limit                0ops        0ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
deletefile1          5016ops      313ops/s   0.0mb/s     50.4ms/op     4372us/op-cpu [4ms - 69ms]
24089: 269.336: IO Summary:  5016 ops, 313.472 ops/s, (0/0 r/w),   0.0mb/s,      0us cpu/op,   0.0ms latency
24089: 269.336: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_delete.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 6715: 0.000: Allocated 170MB of shared memory
 6715: 0.001: FileMicro-Delete Version 2.4 personality successfully loaded
 6715: 0.001: Creating/pre-allocating files and filesets
 6715: 0.046: Fileset bigfileset: 50000 files, 0 leafdirs, avg dir width = 100, avg dir depth = 2.3, 778.397MB
 6715: 169.102: Removed any existing fileset bigfileset in 170 seconds
 6715: 169.102: making tree for filset /dev/shm/test/client_test/bigfileset
 6715: 173.647: Creating fileset bigfileset...
 6715: 244.728: Preallocated 50000 of 50000 of fileset bigfileset in 72 seconds
 6715: 244.728: waiting for fileset pre-allocation to finish
28870: 244.779: Starting 1 filedelete instances
28871: 244.781: Starting 16 filedeletethread threads
 6715: 245.784: Running...
 6715: 261.786: Run took 16 seconds...
 6715: 261.787: Per-Operation Breakdown
finish               5001ops      313ops/s   0.0mb/s      0.0ms/op       20us/op-cpu [0ms - 0ms]
limit                0ops        0ops/s   0.0mb/s      0.0ms/op        0us/op-cpu [0ms - 0ms]
deletefile1          5006ops      313ops/s   0.0mb/s     50.9ms/op     4079us/op-cpu [4ms - 79ms]
 6715: 261.787: IO Summary:  5006 ops, 312.843 ops/s, (0/0 r/w),   0.0mb/s,      0us cpu/op,   0.0ms latency
 6715: 261.787: Shutting down processes