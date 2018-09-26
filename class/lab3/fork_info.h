#ifndef FORK_INFO_H
#define FORK_INFO_H

typedef struct Fork_info
{
  unsigned long clone_flags;
  int parent_pid;
  int child_pid;
  unsigned short parent_uid; // This may also be a unsigned long
  char command_name[16];
  long child_return;
} Fork_info;


#endif // FORK_INFO_H