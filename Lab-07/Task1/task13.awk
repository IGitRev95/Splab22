#! /usr/bin/awk -f

BEGIN {
    print "Task13"
    FS = ","
    OFS = "\n"
    ORS = "\n--------------------\n"
}

{
    if($2>=1970) {print "Actor Name: "$4, "Movie Name: "$5}
}