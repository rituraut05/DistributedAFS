snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f varmail.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: Varmail Version 3.0 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.018: bigfileset populated: 20000 files, avg. dir. width = 1000, avg. dir. depth = 1.4, 0 leafdirs, 310.325MB total size
0.018: Removing bigfileset tree (if exists)
54.623: Pre-allocating directories in bigfileset tree
54.820: Pre-allocating files in bigfileset tree
153.810: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
153.810: Population and pre-allocation of filesets completed
153.810: Starting 1 filereader instances
154.817: Running...
214.823: Run took 60 seconds...
214.825: Per-Operation Breakdown
closefile4           12585ops      210ops/s   0.0mb/s      0.5ms/op [0.13ms - 42.24ms]
readfile4            12585ops      210ops/s   3.0mb/s      0.3ms/op [0.06ms - 39.80ms]
openfile4            12585ops      210ops/s   0.0mb/s      4.9ms/op [1.14ms - 61.05ms]
closefile3           12586ops      210ops/s   0.0mb/s      3.4ms/op [0.35ms - 58.33ms]
fsyncfile3           12586ops      210ops/s   0.0mb/s      0.1ms/op [0.04ms -  0.81ms]
appendfilerand3      12587ops      210ops/s   1.6mb/s      0.5ms/op [0.07ms - 41.58ms]
readfile3            12587ops      210ops/s   2.9mb/s      0.3ms/op [0.08ms -  4.04ms]
openfile3            12587ops      210ops/s   0.0mb/s      4.8ms/op [1.29ms - 61.00ms]
closefile2           12588ops      210ops/s   0.0mb/s      2.9ms/op [0.59ms - 60.91ms]
fsyncfile2           12588ops      210ops/s   0.0mb/s      0.1ms/op [0.04ms - 49.63ms]
appendfilerand2      12588ops      210ops/s   1.6mb/s      0.6ms/op [0.00ms - 44.69ms]
createfile2          12588ops      210ops/s   0.0mb/s     28.8ms/op [3.58ms - 102.49ms]
deletefile1          12590ops      210ops/s   0.0mb/s     28.8ms/op [1.62ms - 108.33ms]
214.825: IO Summary: 163630 ops 2726.947 ops/s 419/420 rd/wr   9.2mb/s  19.0ms/op
214.825: Shutting down processes