#include "lab4_util.h"

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_ISEEK 19

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RD 0
#define O_WR 0001
#define O_RDRW 2
#define O_CREAT 0100

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define EOF 4
int debug_mode_flag = 0;
extern int system_call(int op_code, ...); 

int open(const char *path, int oflag, int mode_t);
int close(int fd);
int read(int fd, const char *input_buff, int number_of_bytes_to_read);
int write(int fd, const char *output_buff, int number_of_bytes_to_write);
int seek(int fd, int offset, int seek);
int my_exit(int status);
void err_exit(const char * err_msg);

/* given file descributor - return the size of the file in bytes. */
int get_file_size(int fd);
void debugWrite(int sys_call_ID, int sys_call_ret);
char getc(int fd);

int main (int argc , char* argv[], char* envp[]){
  int input_fd = STDIN;
  int output_fd = STDOUT;

  char c;
  
  char * o_buff;
  char * output_buff;
  int count = 0, input_file_index = -1, output_file_index = -1, i;

  /* Program args parsing */

  for(i = 1; i < argc; i++){
    if(strncmp("-D", argv[i], 2) == 0) {debug_mode_flag = 1;}

    if(strncmp("-i", argv[i], 2) == 0) {input_file_index = i;}
    
    if(strncmp("-o", argv[i], 2) == 0) {output_file_index = i;}
  }

  if(input_file_index != -1){
    input_fd = open(argv[input_file_index] + 2, O_RDRW, 0);
  }
  
  if(output_file_index != -1){
    output_fd = open(argv[output_file_index] + 2, O_CREAT+O_WR, 0644);
  }

  if(debug_mode_flag){
    debug_mode_flag = 0;
    write(STDERR, "in: ", strlen("in: "));
    if(input_fd == STDIN){
      output_buff = "STDIN";
    }else{
      output_buff = itoa(input_fd);  
    }
    write(STDERR, output_buff, strlen(output_buff));
    write(STDERR, " out: ", strlen(" out: "));
    if(output_fd == STDOUT){
      output_buff = "STDOUT";
    }else{
      output_buff = itoa(output_fd);  
    }
    write(STDERR, output_buff, strlen(output_buff));
    write(STDERR, "\n", strlen("\n"));
    debug_mode_flag = 1;
  }
  
  int in_word_flag = 0;
  while((c=getc(input_fd)) != EOF){
    switch(c){
      case ' ':
      case '\n':
      {
        if(in_word_flag){
          count++;
          in_word_flag = 0;
        }
        break;
      }
      default:{
        in_word_flag = 1;
      }
    }
  }
  if(in_word_flag){
    count++;
  }
  o_buff = itoa(count);
	write(output_fd, o_buff, strlen(o_buff));
  
  if(input_fd != STDIN){
    close(input_fd);
  }
  
  if(output_fd != STDOUT){
    close(output_fd);
  }
  
  my_exit(0);
  return 0;  
  
}

int open(const char *path, int oflag, int mode_t){    
    int ret=-1;
    ret = system_call(SYS_OPEN,path,oflag,mode_t);
    if (ret < 0){
      err_exit("File Opening Fail");
    }
    debugWrite(SYS_OPEN, ret);
    return ret;
}

int close(int fd){
    int ret=-1;
    ret = system_call(SYS_CLOSE,fd);
    if (ret < 0){
      err_exit("File Closing Fail");
    }
    debugWrite(SYS_CLOSE, ret);
    return ret;
}

int read(int fd, const char *input_buff, int number_of_bytes_to_read){
    int ret=-1;
    ret = system_call(SYS_READ, fd, input_buff, number_of_bytes_to_read);
    if (ret < 0){
      err_exit("Fail to read from file");
    }
    debugWrite(SYS_READ, ret);
    return ret;
}

int write(int fd, const char *output_buff, int number_of_bytes_to_write){
    int ret=-1;
    ret = system_call(SYS_WRITE,fd,output_buff,number_of_bytes_to_write);
    if (ret < 0 || (ret==0 && number_of_bytes_to_write>0)){
      err_exit("Fail to write to file");
    }
    debugWrite(SYS_WRITE, ret);
    return ret;
}

int seek(int fd, int offset, int seek){
    int ret=-1;
    ret = system_call(SYS_ISEEK,fd,offset,seek);
    if (ret == 0 && ((offset>0 && seek==0) || seek!=0)){
      err_exit("Fail to seek in file");
    }
    debugWrite(SYS_ISEEK, ret);
    return ret;
}

int my_exit(int status){
  return system_call(SYS_EXIT,status);
}

void err_exit(const char * err_msg){
  write(STDOUT,err_msg,strlen(err_msg));
  my_exit(0x55);
}

int get_file_size(int fd){
  int file_size, cur_fd_pos;
  cur_fd_pos = seek(fd, 0,SEEK_CUR);
  file_size = seek(fd,0,SEEK_END)+1;
  seek(fd,cur_fd_pos,SEEK_SET);
  return file_size;
}

void debugWrite(int sys_call_ID, int sys_call_ret){
  char * output_buff;
  if(debug_mode_flag){
    debug_mode_flag = 0;  /* turn off debug_mode_flag to avoid syscall write recursion */

    write(STDERR, "\ndebug: ", strlen("\ndebug: "));
    output_buff = itoa(sys_call_ID);
    write(STDERR, output_buff, strlen(output_buff));
    write(STDERR, " ", 1);
    output_buff = itoa(sys_call_ret);
    write(STDERR, output_buff, strlen(output_buff));
    write(STDERR, "\n", 1);

    debug_mode_flag = 1;  /* turn back on the debug_mode_flag */
  }
}

char getc(int fd){
  char i_buff[1];
  int read_byte_count = read(fd, i_buff, 1);
  if(read_byte_count == 0){
    return EOF;
  }
  return i_buff[0];
}