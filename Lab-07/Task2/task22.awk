#! /usr/bin/awk -f

BEGIN {
    print "Task22\n"
    FS = ","
    bachelor_parent_counter=0
}
# ask if need to count also supirior dgrees cause they surly have bachlor as well
{
    if($3=="bachelor's degree") 
    {
        bachelor_parent_counter++
    } 
}

END{
    print "number of bachelor parents: " bachelor_parent_counter
}