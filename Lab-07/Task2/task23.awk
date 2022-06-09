#! /usr/bin/awk -f

BEGIN {
    print "Task23\n"
    FS = ","
    print "========\nSuccess Student List\n========"
    success_student_counter=0
}
# ask if need to count also supirior dgrees cause they surly have bachlor as well
{
    if($6>=80 && $7>=80 && $8>=80) 
    {
        success_student_counter++
        print $0
    } 
}

END{
    print "The number of students: " success_student_counter
}