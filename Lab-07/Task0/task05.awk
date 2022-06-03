#! /usr/bin/awk -f

BEGIN {print "Task05"
COUNT=0}
{
    if ($3 == "English") { COUNT++ }
}
END { print "Count = " COUNT}