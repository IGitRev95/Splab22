#! /usr/bin/awk -f

BEGIN {print "Task04"}
{
    print $1, " "; for (i=NF+1;i>1;i--) {printf $(i)}
}
# for(i=NF;i>0;i--){printf $(i)}}
#    for(i=NF;i>1;i--){
#        printf $(i) " "
#        }
#}
