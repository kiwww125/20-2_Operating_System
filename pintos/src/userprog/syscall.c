#include <stdio.h>
#include <stdbool.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/syscall.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
static void chk_address(void * address){
  if(!is_user_vaddr(address)){
    exit(-1);
  }
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // printf("syscall : %d\n", *(uint32_t *)(f->esp));
  // hex_dump(f->esp, f->esp, 100, 1); 
  uint32_t *args = (uint32_t *)f->esp;
  chk_address(args);
  //printf("\n\n esp : %p\n\n", args[0]);
  //arg는 항상 뒤에서부터 쌓는다 user/syscall.c 참조
  switch(*((uint32_t*)f->esp)) {
    case SYS_HALT :
      halt();
      break;                
    case SYS_EXIT :  
      chk_address(args + 4);
      exit((int)args[1]);       
      break;          
    case SYS_EXEC :
      chk_address(args + 4);
      f->eax = exec((const char*)args[1]);
      break;                   
    case SYS_WAIT :
      chk_address(args + 4);
      f->eax = wait((pid_t)args[1]);
      break;
    case SYS_CREATE :
      break;
    case SYS_REMOVE :
      break;
    case SYS_OPEN :
      break;
    case SYS_FILESIZE :
      break;
    case SYS_READ :
      chk_address(args+4);
      chk_address(args+8);
      chk_address(args+12);
      f->eax = read((int)args[1], (void *)args[2], (unsigned)args[3]);
      break;
    case SYS_WRITE :
      chk_address(args + 4);
      chk_address(args + 8);
      chk_address(args + 12);
      f->eax = write((int)args[1], (void *)args[2], (unsigned)args[3]);
      break;
    case SYS_SEEK :
      break;
    case SYS_TELL :
      break;
    case SYS_CLOSE :
      break;
  }
  
}

void
halt(void){
  shutdown_power_off();
}

void
exit(int status){
  //thread_name => cmd_line
  char *cmd, *ptr1; 
  cmd = strtok_r(thread_name()," ", &ptr1);
  // printf("%s: exit(%d)\n", thread_name(), status);
  printf("%s: exit(%d)\n", cmd, status);

  //set status
  thread_current()->exit_status = status;
  thread_exit();
}

pid_t
exec(const char *cmd_line){
  return process_execute(cmd_line); 
}

int
wait(pid_t pid){
  return process_wait(pid);
}

bool create(const char* file, unsigned initial_size);
bool remove(const char* file);
int open(const char *file);
int filesize(int fd);

int
read(int fd, void *buffer, unsigned size){  
  //stdin
  int r_size =0, ret = -1;
  if (fd == 0) {
    while( input_getc() != '\0' && r_size++ < size) {
      r_size++;
    } 
  }

  if(r_size) ret = r_size;
  return ret; 
}

//return actual written bytes
int
write(int fd, const void *buffer, unsigned size){
  //console write
  if(fd == 1){
    putbuf(buffer, size);
    return size;
  }
  return -1;
}

void seek(int fd, unsigned position);
unsigned tell(int fd);
void close(int fd);
