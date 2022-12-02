# LRU
These are some simple programs to investigate LRU.

## Kernel Configuartion (>= v6.1.0)
1. Enable cgroups: a. Enable `CONFIG_CGROUPS`, b. Enable `CONFIG_MEMCG`
2. Enable `LRU_GEN`

## Steps
1. Create a new cgroup:
	- a. Create a new cgroup in `/sys/fs/cgroup` by using `mkdir mygrp`. `cat mygrp/pids.current` should return 0.
	- b. Set the maximum memory (hard mem). (eg., `echo 50M > mygrp/memory.max`)
	- c. `memeory.swap` is disabled by default. The hard limit should be 0 and it can be set simialr to (b)
2. Get pgid/pid by running wordc
3. Add this process to cgroup. (eg., `echo [pid] > mygroup/cgroup.procs`)
4. Install and run the linux module (lru), passing the pid obtained from `wordc` as the value of arg `int_str` (eg., `sudo insmod lru.ko int_str="2672"`)
5. Choose an option from `wordc`. It will create a new child process in the same process group as wordc's main
6. Have a look at the output from `dmesg`, `vmtouch`, as well as `mygrp/memory.stat`

## References
1. An introduction to control groups (cgroups) version 2 - Michael Kerrisk - NDC TechTown 2021. [video](https://www.youtube.com/watch?v=kcnFQgg9ToY&t=2993s&ab_channel=NDCConferences)
2. Control Group v2 [docs](https://android.googlesource.com/kernel/msm/+/refs/heads/android-msm-salmon-4.9-pie-wear-mr1/Documentation/cgroup-v2.txt)