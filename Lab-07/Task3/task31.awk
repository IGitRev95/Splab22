#! /usr/bin/awk -f

BEGIN {
    print "Task31\n"
    FS = ","
    OFS = "---"
}
# ask if need to count also supirior dgrees cause they surly have bachlor as well


$0 ~/spring/ {print $2,$3,$4}

