#!/bin/bash
MemTotal=$(cat /proc/meminfo | awk '/^MemTotal:/{print $2}')
MemFree=$(cat /proc/meminfo | awk '/^MemFree:/{print $2}')
Used=$(expr $MemTotal - $MemFree)
shared=$(cat /proc/meminfo | awk '/^Shmem/{print $2}')
buffers=$(cat /proc/meminfo | awk '/^Buffers/{print $2}')
cached=$(cat /proc/meminfo | awk '/^Cached/{print $2}')
minus=$(expr $Used - $buffers - $cached)
plus=$(expr $Used + $buffers + $cached)
swaptotal=$(cat /proc/meminfo | awk '/^SwapTotal/{print $2}')
swapfree=$(cat /proc/meminfo | awk '/^SwapFree/{print $2}')
swapused=$(expr $swaptotal - $swapfree)
echo "            total      used       free     shared    buffers     cached"    
echo "Mem:	  $MemTotal   $Used    $MemFree      $shared     $buffers     $cached"
echo "-/+ buffers/cache:   $minus    $plus"
echo "Swap:	        $swaptotal	  $swapused          $swapfree"
