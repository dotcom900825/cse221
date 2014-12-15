## CSE221 project source code [Github Link](https://github.com/dotcom900825/cse221)
### CPU, Scheduling, and OS Services
To run the CPU, Scheduling, OS Services test, there will be no any specific input parameter. All the test functions will run automatically and outputs in the corresponding txt files. If you want to run a specific test, you can do so by commenting out other tests in the main function.

We provide the makefile along the way, you can complie the source code by running 
```sh
$ make
```
**Output file description**
* **Read_Overhead.txt**: contains the average measured CPU cycle counts for reading from RTDSC call related registers.

* **Procedure_Call_Overhead.txt**: contains the average measured CPU cycle counts of calling functions with different arguments from 0 ~ 7.

* **System_Call_Overhead.txt**: contains the average measured CPU cycles overhead of making system call.

* **Thread_Creation_Overhead.txt**: contains the average measured CPU cycle overhead used to create a kernel thread.

* **Process_Creation_Overhead.txt**: contains the overhead used to create a process.

* **Context_Switch_Overhead.txt** contains the context switch time between different processes.

* **Kernel_Switch_Overhead.txt**: contains the overhead to do the kernel thread switch.

###Memory 
* **RAM access time**: For the RAM access time, we can use ram_access_time.c to conduct the experiment. To run the test, we need to specify the output test data file and the stride size. Then we can just run the test command by the following command. The out file will contain the CPU cycle count for individual integer accesses for different size of memory regions.
```sh
$ ./ram_access_time
```

* **RAM bandwidth**: For the ram bandwidth measurement, we can use the ram_bandwidth.c, it will generate two output files, RAM_wr_bandwidth and RAM_rd_bandwidth

* **Page fault service time**: For the page fault service time experiment, we can use the page_fault.c. Before we conduct the experiment, we need to first generate a big testing file filled with random data. We can create one by running the following command
```sh
$ dd if=/dev/urandom of=random bs=1048576 count=3072
``` 
Then we can run the page_fault command which will generate

###Network
* **Round Trip Time**: For the RTT measurement, we use the client.py and server.py to conduct this experiment. First on the remote server, you can run the server.py as following:
```sh
$ python server.py
```
Then on the client side, you can run the client.py as following:
```sh
$ python client.py
```
Then type in the remote server ip address, or if you want to perform local round trip time measurement then you can run server.py and client.py on the same machine and put 127.0.0.1 for the ip address. The results will get output to the txt file named by the remote ip address you used.

* **Peak bandwidth**: For the peak bandwidth measurement, we can run the bandwidth_server.py on the remote side and bandwidth_client.py on the local environment to test remote bandwidth. Then we can run both client and server in the local environment to test the local bandwidth. We can specify the test file size and host info when we start the client and server program. 
On the server side:
```sh
$ python bandwidthServer.py
What's the file size
32
```
On the client side:
```sh
$ python bandwidthClient.py
What's the host name
127.0.0.1
What's the file size
32
```

* **Connection overhead**: For the connection overhead experiment, we need to fire up server program on the remote machine by just running server.py, and on the local environment we run the setup_test.py and teardown_test.py to measure connection setup overhead and connection teardown overhead respectively. 

### File System
Before you start running the test, you might need to prepare some randomly filled test file in different size. We provide a util python script for that purpose: file_generator.py. By modifying the bytes parameters, you can generate different test file size on demand.

* To measure the **system file cache size**, we can use cache_size_test.c, after compile it, you can run the command as following, let's say you want to test reading 4GB size data from a 5G test file called 5gb_file. 
```sh
$ ./cache_size_test 5368709120  5gb_file
```

* To measure the **File read time**, we can use read_time_test.c, after compile it, you can run the command in the following way. You can generate a series of test files in different size by using GenerateFile.py. The command will out put two columns of data every time, first column is sequential access time in us, the second column is random access time in us.
```sh
$ ./read_time_test 2097152 2mb_file
```

* To measure the **remote file read time**, we need to setup the testing environment first. In our setup, one Macbook serves as NFS and enable file sharing for all the testing files. The other Macbook serves as client and mout the NFS's folder onto its file system. Then we use the remote_read_time.c to experiment sequential and random read time. Unfortunately, before you start the test, you need to modify the file path within the source code. 
```sh
$ ./remote_read_test 16777216
```

* To measure the **Contention**:, we need to first run a number of processes performing random or sequential read continueslly. We do that by running the read_time_contention.c as following. The second parameter control whether it keep reading sequentially or randomely, 1 for random read and 0 for sequential read.
* 
```sh
$ ./read_time_contention testFile 1
```
Then we can fire up another process, that run the previous used read_time_test.c on a different file. This can guarantee that there are a number of processes reading different files and another one performing measurement.






