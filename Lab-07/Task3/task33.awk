#! /usr/bin/awk -f

BEGIN {
    print "Task33\n"
    FS = ","
    OFS = "---"
}

$2 ~/happy/ {print $2,$3,$4}
