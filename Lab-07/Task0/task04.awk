#! /usr/bin/awk -f

BEGIN {print "Task04"; OFS = "\t"}
{
    printf $1
    for (i=NF;i>1;i--) {
        printf " " $(i) "\t"
    }
    printf "\n"
}