snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f mongo.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 6290: 0.000: Allocated 170MB of shared memory
 6290: 0.001: Mongo-like Version 2.3 personality successfully loaded
 6290: 0.001: Creating/pre-allocating files and filesets
 6290: 0.012: Fileset postsetdel: 10000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.7, 154.215MB
 6290: 0.020: Removed any existing fileset postsetdel in 1 seconds
 6290: 0.020: making tree for filset /dev/shm/test/client_test/postsetdel
 6290: 0.493: Creating fileset postsetdel...
 6290: 25.586: Preallocated 10000 of 10000 of fileset postsetdel in 26 seconds
 6290: 25.604: Fileset postset: 10000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.7, 156.267MB
 6290: 25.618: Removed any existing fileset postset in 1 seconds
 6290: 25.618: making tree for filset /dev/shm/test/client_test/postset
 6290: 26.141: Creating fileset postset...
 6290: 56.330: Preallocated 10000 of 10000 of fileset postset in 31 seconds
 6290: 56.330: waiting for fileset pre-allocation to finish
 1203: 56.441: Starting 1 filereader instances
 1204: 56.444: Starting 1 filereaderthread threads
 6290: 57.444: Running...



 6290: 236.461: Run took 179 seconds...
 6290: 236.461: Per-Operation Breakdown
deletefile1          10000ops       56ops/s   0.0mb/s      3.8ms/op      128us/op-cpu [2ms - 996ms]
closefile2           10001ops       56ops/s   0.0mb/s      2.5ms/op       68us/op-cpu [1ms - 884ms]
readfile1            10001ops       56ops/s   0.9mb/s      0.3ms/op       66us/op-cpu [0ms - 891ms]
openfile2            10001ops       56ops/s   0.0mb/s      3.6ms/op      122us/op-cpu [2ms - 985ms]
closefile1           10001ops       56ops/s   0.0mb/s      3.1ms/op       75us/op-cpu [1ms - 981ms]
appendfilerand1      10001ops       56ops/s   0.4mb/s      0.4ms/op       73us/op-cpu [0ms - 100ms]
openfile1            10001ops       56ops/s   0.0mb/s      3.8ms/op      129us/op-cpu [2ms - 938ms]
 6290: 236.461: IO Summary: 70006 ops, 391.060 ops/s, (56/56 r/w),   1.3mb/s,  19733us cpu/op,   8.7ms latency
 6290: 236.461: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f mongo.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
 2515: 0.000: Allocated 170MB of shared memory
 2515: 0.001: Mongo-like Version 2.3 personality successfully loaded
 2515: 0.001: Creating/pre-allocating files and filesets
 2515: 0.012: Fileset postsetdel: 10000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.7, 154.215MB
 2515: 0.340: Removed any existing fileset postsetdel in 1 seconds
 2515: 0.340: making tree for filset /dev/shm/test/client_test/postsetdel
 2515: 0.807: Creating fileset postsetdel...
 2515: 31.082: Preallocated 10000 of 10000 of fileset postsetdel in 31 seconds
 2515: 31.099: Fileset postset: 10000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.7, 156.267MB
 2515: 73.096: Removed any existing fileset postset in 42 seconds
 2515: 73.096: making tree for filset /dev/shm/test/client_test/postset
 2515: 73.617: Creating fileset postset...


 2515: 102.457: Preallocated 10000 of 10000 of fileset postset in 29 seconds
 2515: 102.457: waiting for fileset pre-allocation to finish
38679: 102.545: Starting 1 filereader instances
38680: 102.547: Starting 1 filereaderthread threads
 2515: 103.548: Running...



 2515: 281.565: Run took 178 seconds...
 2515: 281.565: Per-Operation Breakdown
deletefile1          10000ops       56ops/s   0.0mb/s      3.7ms/op      126us/op-cpu [2ms - 920ms]
closefile2           10001ops       56ops/s   0.0mb/s      2.6ms/op       85us/op-cpu [1ms - 921ms]
readfile1            10001ops       56ops/s   0.9mb/s      0.3ms/op       83us/op-cpu [0ms - 186ms]
openfile2            10001ops       56ops/s   0.0mb/s      3.6ms/op      157us/op-cpu [1ms - 921ms]
closefile1           10001ops       56ops/s   0.0mb/s      3.0ms/op       87us/op-cpu [1ms - 899ms]
appendfilerand1      10001ops       56ops/s   0.4mb/s      0.4ms/op       79us/op-cpu [0ms - 890ms]
openfile1            10001ops       56ops/s   0.0mb/s      3.7ms/op      120us/op-cpu [2ms - 969ms]
 2515: 281.565: IO Summary: 70006 ops, 393.255 ops/s, (56/56 r/w),   1.3mb/s,  20074us cpu/op,   8.7ms latency
 2515: 281.566: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f mongo.f
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
40170: 0.000: Allocated 170MB of shared memory
40170: 0.001: Mongo-like Version 2.3 personality successfully loaded
40170: 0.001: Creating/pre-allocating files and filesets
40170: 0.012: Fileset postsetdel: 10000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.7, 154.215MB
40170: 0.352: Removed any existing fileset postsetdel in 1 seconds
40170: 0.352: making tree for filset /dev/shm/test/client_test/postsetdel
40170: 0.835: Creating fileset postsetdel...



40170: 26.691: Preallocated 10000 of 10000 of fileset postsetdel in 26 seconds
40170: 26.709: Fileset postset: 10000 files, 0 leafdirs, avg dir width = 200, avg dir depth = 1.7, 156.267MB


40170: 64.635: Removed any existing fileset postset in 38 seconds
40170: 64.635: making tree for filset /dev/shm/test/client_test/postset
40170: 65.357: Creating fileset postset...
40170: 101.361: Preallocated 10000 of 10000 of fileset postset in 37 seconds
40170: 101.362: waiting for fileset pre-allocation to finish
35397: 101.426: Starting 1 filereader instances
35398: 101.428: Starting 1 filereaderthread threads
40170: 102.429: Running...
40170: 277.446: Run took 175 seconds...
40170: 277.447: Per-Operation Breakdown
deletefile1          10000ops       57ops/s   0.0mb/s      3.6ms/op      135us/op-cpu [2ms - 153ms]
closefile2           10001ops       57ops/s   0.0mb/s      2.4ms/op       85us/op-cpu [1ms - 61ms]
readfile1            10001ops       57ops/s   0.9mb/s      0.4ms/op       80us/op-cpu [0ms - 1026ms]
openfile2            10001ops       57ops/s   0.0mb/s      3.7ms/op      143us/op-cpu [2ms - 1041ms]
closefile1           10001ops       57ops/s   0.0mb/s      2.8ms/op       74us/op-cpu [1ms - 161ms]
appendfilerand1      10001ops       57ops/s   0.4mb/s      0.3ms/op       79us/op-cpu [0ms - 5ms]
openfile1            10001ops       57ops/s   0.0mb/s      3.8ms/op      154us/op-cpu [2ms - 1134ms]
40170: 277.447: IO Summary: 70006 ops, 399.995 ops/s, (57/57 r/w),   1.3mb/s,  19920us cpu/op,   8.5ms latency
40170: 277.447: Shutting down processes
