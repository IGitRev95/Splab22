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

#define O_RDRW 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern int system_call(int op_code, ...); 

int open(const char *path, int oflag, int mode_t);
int close(const char *path);
int read(int fd, const char *input_buff, int number_of_bytes_to_read);
int write(int fd, const char *output_buff, int number_of_bytes_to_write);
int seek(int fd, int offset, int seek);
int my_exit(int status);
void err_exit();

/* given file descributor - return the size of the file in bytes. */
int get_file_size(int fd);

int main (int argc , char* argv[], char* envp[])
{
  int file_desc ;
  if(argc != 3){
    err_exit();
  }
  
  file_desc = open(argv[1],O_RDRW,0644);
  seek(file_desc,0x291,SEEK_SET);
  write(file_desc,argv[2],strlen(argv[2]));
  write(file_desc,".\0",2);
  
  close(argv[1]);
  my_exit(0);
  return 0;
}

int open(const char *path, int oflag, int mode_t){    
    int ret=-1;
    ret = system_call(5,path,oflag,mode_t);
    if (ret < 0){
      err_exit();
    }
    return ret;
}

int close(const char *path){
    int ret=-1;
    ret = system_call(SYS_CLOSE,path);
    if (ret < 0){
      err_exit();
    }
    return ret;
}

int read(int fd, const char *input_buff, int number_of_bytes_to_read){
    int ret=-1;
    ret = system_call(SYS_READ, fd, input_buff, number_of_bytes_to_read);
    if (ret < 0 || (ret==0 && number_of_bytes_to_read>0)){
      err_exit();
    }
    return ret;
}

int write(int fd, const char *output_buff, int number_of_bytes_to_write){
    int ret=-1;
    ret = system_call(SYS_WRITE,fd,output_buff,number_of_bytes_to_write);
    if (ret < 0 || (ret==0 && number_of_bytes_to_write>0)){
      err_exit();
    }
    return ret;
}

int seek(int fd, int offset, int seek){
    int ret=-1;
    ret = system_call(SYS_ISEEK,fd,offset,seek);
    if (ret == 0 && ((offset>0 && seek==0) || seek!=0)){
      err_exit();
    }
    return ret;
}

int my_exit(int status){
  return system_call(SYS_EXIT,status);
}

void err_exit(){
  my_exit(0x55);
}

int get_file_size(int fd){
  int file_size, cur_fd_pos;
  cur_fd_pos = seek(fd, 0,SEEK_CUR);
  file_size = seek(fd,0,SEEK_END)+1;
  seek(fd,cur_fd_pos,SEEK_SET);
  return file_size;
}
