#! /usr/bin/awk -f

BEGIN {
    
    print "Task03"
    
}
$2 ~ /[a]/ {print $2"\t"$4}