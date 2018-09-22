#ifndef SYSCALL_H
#define SYSCALL_H

#include "./asl/asl.h"
#include "./pcb/pcb_t.h"
#include "./pcb/const.h"
#include "/usr/include/uarm/uARMtypes.h"
state_t *sys5_sys_old;
state_t *sys5_TLB_old;
state_t *sys5_PGMT_old;
state_t *sys5_sys_new;
state_t *sys5_TLB_new;
state_t *sys5_PGMT_new;
pcb_t* pcb_wait;
void P(int *semaddr);
void V(int *semaddr);
int CREATEPROCESS(state_t *statep,int priority,void **cpid);
int TERMINATEPROCESS(void *pid);
#endif
