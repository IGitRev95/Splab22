#! /usr/bin/awk -f

BEGIN {
    print "Task21\n"
    FS = ","
    standard_lunch_student_count=0
    total_math_score = 0
    total_reading_score = 0
    total_writing_score = 0
}

{
    if($4=="standard") 
    {
        standard_lunch_student_count++
        total_math_score+=$6
        total_reading_score+=$7
        total_writing_score+=$8
    } 
}

END{
    if (standard_lunch_student_count != 0){
    print "avg math: " total_math_score/standard_lunch_student_count
    print "avg read: " total_reading_score/standard_lunch_student_count
    print "avg write: " total_writing_score/standard_lunch_student_count
    }
    else { print "No students arg meeting condition"}
}