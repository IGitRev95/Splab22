#! /usr/bin/awk -f

BEGIN {
    print "Task12"
    FS = ","
    OFS = " | "
}

{
    print $4, $3, $2
}