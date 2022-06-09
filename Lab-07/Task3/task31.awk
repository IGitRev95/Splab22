#! /usr/bin/awk -f

BEGIN {
    print "Task31\n"
    FS = ","
    OFS = "---"
}

$0 ~/spring/ {print $2,$3,$4}
