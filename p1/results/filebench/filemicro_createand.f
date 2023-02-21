Filebench Version 1.4.9
38260: 0.000: Allocated 170MB of shared memory
38260: 0.001: FileMicro-CreateRand Version 2.1 personality successfully loaded
38260: 0.001: Creating/pre-allocating files and filesets
38260: 0.004: File largefile: 0.000MB
38260: 0.381: Removed any existing file largefile in 1 seconds
38260: 0.381: making tree for filset /dev/shm/test/client_test/largefile
38260: 0.394: Creating file largefile...
38260: 0.400: Preallocated 1 of 1 of file largefile in 1 seconds
38260: 0.400: waiting for fileset pre-allocation to finish
38265: 0.400: Starting 1 filecreater instances
38266: 0.401: Starting 1 filecreaterthread threads
38260: 1.402: Running...
38260: 10.403: Run took 9 seconds...
38260: 10.404: Per-Operation Breakdown
finish               205ops       23ops/s   0.0mb/s      0.0ms/op       49us/op-cpu [0ms - 0ms]
sync                 205ops       23ops/s   0.0mb/s      0.0ms/op       98us/op-cpu [0ms - 0ms]
append-file          2051ops      228ops/s 113.8mb/s      4.0ms/op     1394us/op-cpu [0ms - 19ms]
38260: 10.404: IO Summary:  2256 ops, 250.639 ops/s, (0/228 r/w), 113.8mb/s,   5037us cpu/op,   4.0ms latency
38260: 10.404: Shutting down processes
Filebench Version 1.4.9
39094: 0.000: Allocated 170MB of shared memory
39094: 0.001: FileMicro-CreateRand Version 2.1 personality successfully loaded
39094: 0.001: Creating/pre-allocating files and filesets
39094: 0.004: File largefile: 0.000MB
39094: 0.085: Removed any existing file largefile in 1 seconds
39094: 0.085: making tree for filset /dev/shm/test/client_test/largefile
39094: 0.099: Creating file largefile...
39094: 0.104: Preallocated 1 of 1 of file largefile in 1 seconds
39094: 0.104: waiting for fileset pre-allocation to finish
39098: 0.105: Starting 1 filecreater instances
39099: 0.106: Starting 1 filecreaterthread threads
39094: 1.107: Running...
39094: 3.058: Aborting...
