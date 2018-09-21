#ifndef SYSCALL_H
#define SYSCALL_H

void P(int *semaddr);
void P(int *semaddr);
CREATEPROCESS(state_t *statep,int priority,void **cpid);
int TERMINATEPROCESS(void *pid);
#endif
