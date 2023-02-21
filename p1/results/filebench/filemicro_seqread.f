snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqread.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
32705: 0.000: Allocated 170MB of shared memory
32705: 0.001: FileMicro-SeqRead Version 2.1 personality successfully loaded
32705: 0.001: Creating/pre-allocating files and filesets
32705: 0.004: File largefile: 1024.000MB
32705: 0.004: Re-using file largefile.
32705: 0.004: Creating file largefile...
32705: 22.305: Preallocated 1 of 1 of file largefile in 23 seconds
32705: 22.305: waiting for fileset pre-allocation to finish
32902: 22.305: Starting 1 filereader instances
32903: 22.306: Starting 1 filereaderthread threads
32705: 23.307: Running...
32705: 33.308: Run took 10 seconds...
32705: 33.308: Per-Operation Breakdown
seqread-file         47919ops     4791ops/s 4786.7mb/s      0.2ms/op      190us/op-cpu [0ms - 14ms]
32705: 33.308: IO Summary: 47919 ops, 4791.429 ops/s, (4791/0 r/w), 4786.7mb/s,    256us cpu/op,   0.2ms latency
32705: 33.309: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqread.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
33272: 0.000: Allocated 170MB of shared memory
33272: 0.001: FileMicro-SeqRead Version 2.1 personality successfully loaded
33272: 0.001: Creating/pre-allocating files and filesets
33272: 0.004: File largefile: 1024.000MB
33272: 0.004: Re-using file largefile.
33272: 0.004: Creating file largefile...
33272: 23.041: Preallocated 1 of 1 of file largefile in 24 seconds
33272: 23.041: waiting for fileset pre-allocation to finish
33464: 23.041: Starting 1 filereader instances
33465: 23.043: Starting 1 filereaderthread threads
33272: 24.043: Running...
33272: 34.045: Run took 10 seconds...
33272: 34.045: Per-Operation Breakdown
seqread-file         47977ops     4797ops/s 4792.4mb/s      0.2ms/op      191us/op-cpu [0ms - 7ms]
33272: 34.045: IO Summary: 47977 ops, 4797.142 ops/s, (4797/0 r/w), 4792.4mb/s,    247us cpu/op,   0.2ms latency
33272: 34.045: Shutting down processes

snehalw@node1:~/unreliablefs/filebench_workloads$ filebench -f filemicro_seqread.f 
Filebench Version 1.4.9
WARNING: Could not open /proc/sys/kernel/shmmax file!
It means that you probably ran Filebench not as a root. Filebench will not increase shared
region limits in this case, which can lead to the failures on certain workloads.
33823: 0.000: Allocated 170MB of shared memory
33823: 0.001: FileMicro-SeqRead Version 2.1 personality successfully loaded
33823: 0.001: Creating/pre-allocating files and filesets
33823: 0.004: File largefile: 1024.000MB
33823: 0.004: Re-using file largefile.
33823: 0.004: Creating file largefile...
33823: 18.790: Preallocated 1 of 1 of file largefile in 19 seconds
33823: 18.790: waiting for fileset pre-allocation to finish
33982: 18.790: Starting 1 filereader instances
33983: 18.791: Starting 1 filereaderthread threads
33823: 19.792: Running...
33823: 29.793: Run took 10 seconds...
33823: 29.794: Per-Operation Breakdown
seqread-file         47511ops     4751ops/s 4745.9mb/s      0.2ms/op      192us/op-cpu [0ms - 24ms]
33823: 29.794: IO Summary: 47511 ops, 4750.578 ops/s, (4751/0 r/w), 4745.9mb/s,    260us cpu/op,   0.2ms latency
33823: 29.794: Shutting down processes