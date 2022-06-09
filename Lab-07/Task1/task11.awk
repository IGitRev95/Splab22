#! /usr/bin/awk -f

BEGIN {print "Task11"
    i=0
}

{
    if(i<ARGC) {print "ARGV[" i "] = " ARGV[i]}
    i++ 
}