#include "lab4_util.h"

#define SYS_WRITE 4
#define STDOUT 1

/* extern int system_call(int sys_op_code,int file_node_io_index, const char * input_string,unsigned int str_length);*/
extern int system_call(int eax, ...);

int main (int argc , char* argv[], char* envp[])
{
  int i;
  char * str;
  str = itoa(argc);
  system_call(SYS_WRITE,STDOUT, str,strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STDOUT,"argv[", 5);
	    str = itoa(i);
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",4);
      system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }
  return 0;
}
