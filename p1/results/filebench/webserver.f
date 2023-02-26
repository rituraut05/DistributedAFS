snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f webserver.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: Web-server Version 3.1 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.005: logfiles populated: 1 files, avg. dir. width = 200, avg. dir. depth = 0.0, 0 leafdirs, 0.002MB total size
0.005: Removing logfiles tree (if exists)
0.031: Pre-allocating directories in logfiles tree
0.049: Pre-allocating files in logfiles tree
0.090: bigfileset populated: 20000 files, avg. dir. width = 200, avg. dir. depth = 1.9, 0 leafdirs, 310.502MB total size
0.090: Removing bigfileset tree (if exists)
71.293: Pre-allocating directories in bigfileset tree
72.144: Pre-allocating files in bigfileset tree
207.605: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
207.606: Population and pre-allocation of filesets completed
207.606: Starting 1 filereader instances
208.617: Running...
218.623: Run took 10 seconds...
218.658: Per-Operation Breakdown
appendlog            1181ops      118ops/s   0.9mb/s      2.4ms/op [0.15ms - 195.97ms]
closefile10          1131ops      113ops/s   0.0mb/s      9.6ms/op [0.11ms - 239.97ms]
readfile10           1131ops      113ops/s   1.7mb/s      0.5ms/op [0.06ms - 55.77ms]
openfile10           1133ops      113ops/s   0.0mb/s     33.0ms/op [4.92ms - 496.24ms]
closefile9           1134ops      113ops/s   0.0mb/s     10.0ms/op [0.13ms - 479.00ms]
readfile9            1134ops      113ops/s   1.7mb/s      0.3ms/op [0.07ms - 38.21ms]
openfile9            1138ops      114ops/s   0.0mb/s     34.8ms/op [6.31ms - 498.39ms]
closefile8           1140ops      114ops/s   0.0mb/s      7.9ms/op [0.17ms - 238.03ms]
readfile8            1140ops      114ops/s   1.7mb/s      0.4ms/op [0.06ms - 65.80ms]
openfile8            1144ops      114ops/s   0.0mb/s     30.8ms/op [7.52ms - 504.93ms]
closefile7           1144ops      114ops/s   0.0mb/s      9.6ms/op [0.18ms - 479.70ms]
readfile7            1144ops      114ops/s   1.8mb/s      0.5ms/op [0.06ms - 120.85ms]
openfile7            1149ops      115ops/s   0.0mb/s     34.2ms/op [5.85ms - 509.66ms]
closefile6           1150ops      115ops/s   0.0mb/s      9.8ms/op [0.13ms - 482.31ms]
readfile6            1150ops      115ops/s   1.8mb/s      0.5ms/op [0.06ms - 74.91ms]
openfile6            1154ops      115ops/s   0.0mb/s     31.6ms/op [5.75ms - 511.97ms]
closefile5           1156ops      116ops/s   0.0mb/s      9.3ms/op [0.17ms - 484.06ms]
readfile5            1156ops      116ops/s   1.8mb/s      0.5ms/op [0.06ms - 126.99ms]
openfile5            1160ops      116ops/s   0.0mb/s     32.1ms/op [5.72ms - 496.31ms]
closefile4           1162ops      116ops/s   0.0mb/s      9.5ms/op [0.16ms - 237.31ms]
readfile4            1162ops      116ops/s   1.8mb/s      0.4ms/op [0.07ms - 28.45ms]
openfile4            1165ops      116ops/s   0.0mb/s     31.8ms/op [4.76ms - 494.31ms]
closefile3           1166ops      117ops/s   0.0mb/s     10.0ms/op [0.09ms - 484.99ms]
readfile3            1166ops      117ops/s   1.8mb/s      0.3ms/op [0.06ms - 13.17ms]
openfile3            1171ops      117ops/s   0.0mb/s     32.5ms/op [3.50ms - 259.26ms]
closefile2           1173ops      117ops/s   0.0mb/s      8.7ms/op [0.13ms - 234.68ms]
readfile2            1173ops      117ops/s   1.9mb/s      0.5ms/op [0.06ms - 92.28ms]
openfile2            1174ops      117ops/s   0.0mb/s     30.2ms/op [3.56ms - 494.95ms]
closefile1           1174ops      117ops/s   0.0mb/s      7.9ms/op [0.17ms - 230.40ms]
readfile1            1174ops      117ops/s   1.8mb/s      0.4ms/op [0.06ms - 24.39ms]
openfile1            1179ops      118ops/s   0.0mb/s     32.9ms/op [4.03ms - 497.67ms]
218.658: IO Summary: 35808 ops 3580.421 ops/s 1153/118 rd/wr  18.7mb/s  38.4ms/op
218.658: Shutting down processes
