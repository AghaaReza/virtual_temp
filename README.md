# virtual_temp
Here's a simple Linux kernel module that simulates a virtual temperature sensor using the /proc filesystem. Every time you read from /proc/virtual_temp, it returns a random temperature between 20–40°C.

**Build Instructions**
 
 Ensure your kernel headers are installed. Then:

```bash
make
````
**Usage**
1. Insert the module:

````bash
sudo insmod virtual_temp.ko
````
2. Check the log:

````bash
dmesg | grep virtual_temp
````

3. Read the virtual temperature:

````bash
cat /proc/virtual_temp # Example output: Temperature: 32°C
````

4. Remove the module:
````bash
sudo rmmod virtual_temp
````



