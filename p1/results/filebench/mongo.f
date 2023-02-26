snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f mongo.f
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: Mongo-like Version 2.3 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.009: postsetdel populated: 10000 files, avg. dir. width = 200, avg. dir. depth = 1.7, 0 leafdirs, 156.250MB total size
0.011: Removing postsetdel tree (if exists)
0.018: Pre-allocating directories in postsetdel tree
0.574: Pre-allocating files in postsetdel tree
9.493: postset populated: 10000 files, avg. dir. width = 200, avg. dir. depth = 1.7, 0 leafdirs, 156.250MB total size
9.494: Removing postset tree (if exists)
9.508: Pre-allocating directories in postset tree
10.004: Pre-allocating files in postset tree
18.878: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
18.925: Population and pre-allocation of filesets completed
18.925: Starting 1 filereader instances
19.932: Running...
29.933: Run took 10 seconds...
29.933: Per-Operation Breakdown
deletefile1          693ops       69ops/s   0.0mb/s      3.7ms/op [3.23ms - 11.98ms]
closefile2           693ops       69ops/s   0.0mb/s      0.3ms/op [0.19ms -  0.51ms]
readfile1            693ops       69ops/s   1.1mb/s      0.4ms/op [0.27ms -  6.98ms]
openfile2            693ops       69ops/s   0.0mb/s      3.6ms/op [2.94ms -  6.87ms]
closefile1           693ops       69ops/s   0.0mb/s      2.3ms/op [1.83ms -  2.73ms]
appendfilerand1      693ops       69ops/s   0.5mb/s      0.5ms/op [0.13ms -  0.81ms]
openfile1            693ops       69ops/s   0.0mb/s      3.6ms/op [2.71ms -  8.90ms]
29.933: IO Summary:  4851 ops 485.050 ops/s 69/69 rd/wr   1.6mb/s   7.2ms/op
29.933: Shutting down processes