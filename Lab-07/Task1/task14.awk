#! /usr/bin/awk -f

BEGIN {
    print "Task14"
    FS = ","
}

{
    if($1>=50 && $1<=70) {print $5}
}