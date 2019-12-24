There are artifacts that include both driver file (C++ implemented, stored in "module_to_core") and execution test program (C standard 99, find it in "driver_test") 
attached to an student assignment, specifically for the university course «System programming»  

OS: Linux Ubuntu v. 18.04  
Kernel version: 4.15  
IDE: CLion 2019.2  

1.Linux terminal commands to generate a module file (.ko), to embed it into a kernel stuff of OS and to create a device-connected file also: 
      
1) Make a module file — switch to your project folder and enter «make» command:

make

2) Embed the freshly generated module into OS kernel:

as being eligible to act as a superuser - insmod buffer.ko  
as a non-priviliged user — sudo insmod buffer.ko

3) See what major number was assigned when embedding:

dmesg | tail -20 

(NOTE: use «dmesg» to track driver-related operations like registration, major number assignment, removal from OS kernel, error messages too)   

4) Create a device file to be tested by reading and writing functions:

NOTE: instead 236, input a number which is valid since step 3 completed (that number may differ in particular applications)

(superuser) mknod /dev/buffer c 236 0   
(others) sudo mknod /dev/buffer c 236 0 

5) Grant extended access on device file created in step 4:
 
(superuser) chmod 777 /dev/buffer  
(others) sudo chmod 777 /dev/buffer 

6) Test writing to buffer:

(you may push any alternative input, not just 5) echo 5 > /dev/buffer 

7) If step 6 is ok, then try reading what was written in previous step:

cat /dev/buffer

8) In steps 6, 7 you are expected to have no error messages, if everything is correct you will see your input as an output once «cat» instruction is executed.



2. To proceed with open/read/write testing, switch to a folder «tester.c» stored in. Afterwards please compile it 

gcc tester.c -o  tester

ahead of running by

./tester

Once device file gets accessed, you will be proposed to enter any text to write it to the buffer. Lets do that.
Then, press «Enter» to see your input as a program output. If things look right, process finishes with return code 0.


3. To reverse driver embedding / to remove the driver, switch back to driver folder, then enter command

make clean 

If clean-up messages appear, the OS kernel does not have any longer the module embedded in step 2.

NOTE: if you need to remove module due to some technical troubles, use this command:

(buffer is just a sample, do remember to change this name if your module is given another one) 

(superuser) rmmod buffer    
(others) sudo rmmod buffer  

NOTE: if you need to remove device file due to some technical troubles or failed operations, use this command:           

(superuser) rm /dev/buffer  
(others) sudo rm /dev/buffer
