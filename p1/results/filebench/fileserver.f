snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f fileserver.f 
Filebench Version 1.5-alpha3
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
0.000: Allocated 173MB of shared memory
0.002: File-server Version 3.0 personality successfully loaded
0.002: Populating and pre-allocating filesets
0.018: bigfileset populated: 20000 files, avg. dir. width = 20, avg. dir. depth = 3.3, 0 leafdirs, 2487.318MB total size
0.018: Removing bigfileset tree (if exists)
71.737: Pre-allocating directories in bigfileset tree
82.681: Pre-allocating files in bigfileset tree
294.517: Waiting for pre-allocation to finish (in case of a parallel pre-allocation)
294.517: Population and pre-allocation of filesets completed
294.517: Starting 1 filereader instances
295.528: Running...
305.531: Run took 10 seconds...
305.539: Per-Operation Breakdown
statfile1            2047ops      205ops/s   0.0mb/s      9.9ms/op [0.01ms - 70.70ms]
deletefile1          2043ops      204ops/s   0.0mb/s     50.5ms/op [1.95ms - 278.69ms]
closefile3           2050ops      205ops/s   0.0mb/s      4.4ms/op [0.14ms - 135.68ms]
readfile1            2051ops      205ops/s  25.6mb/s      2.9ms/op [0.19ms - 42.10ms]
openfile2            2054ops      205ops/s   0.0mb/s     17.9ms/op [1.94ms - 82.06ms]
closefile2           2057ops      206ops/s   0.0mb/s     18.9ms/op [4.02ms - 88.58ms]
appendfilerand1      2058ops      206ops/s   1.6mb/s      4.9ms/op [0.10ms - 60.15ms]
openfile1            2060ops      206ops/s   0.0mb/s     18.2ms/op [1.96ms - 79.32ms]
closefile1           2064ops      206ops/s   0.0mb/s     17.5ms/op [2.99ms - 65.28ms]
wrtfile1             2069ops      207ops/s  25.5mb/s     43.6ms/op [0.31ms - 301.22ms]
createfile1          2082ops      208ops/s   0.0mb/s     51.8ms/op [8.38ms - 228.21ms]
305.539: IO Summary: 22635 ops 2263.330 ops/s 205/413 rd/wr  52.7mb/s  80.2ms/op
305.539: Shutting down processes