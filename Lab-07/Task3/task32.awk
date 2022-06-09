#! /usr/bin/awk -f

BEGIN {
    print "Task32\n"
    FS = ","
    poem_with_f_n_counter = 0
}

$0 ~/f[a,u]n/ {poem_with_f_n_counter++}

END{ print "The number of poem that have fun or fan is: "poem_with_f_n_counter}

