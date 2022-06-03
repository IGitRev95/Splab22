#! /usr/bin/awk -f

BEGIN {print "Task06"}
{
    if ($4 >= 87) { print $0 }
}
