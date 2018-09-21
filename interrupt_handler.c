#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/ulibuarm.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/uARMconst.h"
#include "syscall.h"
#include "/usr/include/uarm/arch.h"
int clock_wait;	//true se il prossimo interrupt dovra' risvegliare i processi waiting for clock
int *interval_timer=(int*)0x000002E4;	//punta all'interval timer
int clock_rate=0x10000;
int PIPPO=2;
state_t *new_interrupt=(state_t*)0x00007058;
//void ** fast_interrupt_pc=0x0000001C;
typedef int* memaddr;
void scheduler(){
	tprint("sono lo scheduler!!!\n");
}
void intHandler(){	//prendo per presupposto sia un interval timer interrupt
	//reimposto il timer al valore corretto
	*interval_timer=clock_rate;		
	scheduler();
	tprint("scheduler finito!\n");
	while(1){
		tprint("in handler\n");
	}
	/*if(clock_wait){
		
	}else{
		interval_timer=0x0000.02E4;
		//da mettere il caso in cui si dovra' mettere clock_wait=true
		*interval_timer= lock_rate/1000*3;
		scheduler();
	}*/
	
}

void sysHandler(){	//prendo per presupposto sia un interval timer interrupt
	state_t* old_state=(void*)SYSBK_OLDAREA;
	int sysnum=old_state->a1;	//prendo i parametri della syscall
	int a2=old_state->a2;
	int a3=old_state->a3;
	int a4=old_state->a4;
	
	if (sysnum==PIPPO){
		while(a2>0){	
			tprint("syscalled!!!!\n");
			a2--;
		}
	}
	scheduler();
	while(1){
		tprint("in sysHandler\n");
	}
	
	/*if(clock_wait){
		
	}else{
		interval_timer=0x0000.02E4;
		//da mettere il caso in cui si dovra' mettere clock_wait=true
		*interval_timer= lock_rate/1000*3;
		scheduler();
	}*/
	
}
void pippoInutile(){
	//SYSCALL(2,5,0,0);
	int i=0;
	while(i<20)
		tprint("pippo e' inutile\n");
	while(1){}
}
void init(){
	
	
}
void programma(){
	
	while(1){
		tprint("in programma\n");
	}
}


void initState(void* state2init, void* hand,int new) {
	state_t* state = (state_t*)state2init;
	state->a1=0;    //r0
	state->a2=0;    //r1
	state->a3=0;    //r2
	state->a4=0;    //r3
	state->v1=0;    //r4
	state->v2=0;    //r5
	state->v3=0;    //r6
	state->v4=0;    //r7
	state->v5=0;    //r8
	state->v6=0;    //r9
	state->sl=0;    //r10
	state->fp=0;    //r11
	state->ip=0;    //r12
	if(new)
		state->sp = RAM_TOP;
	else
		state->sp = RAM_TOP-FRAMESIZE;
	state->lr=0;    //r14
	state->pc = (int)hand;
	if(new)
		state->cpsr = 0 | STATUS_TIMER_ID | STATUS_ID | STATUS_SYS_MODE;// & (~STATUS_TIMER_ID);
	else
		state->cpsr = 0 & ~STATUS_TIMER_ID | STATUS_ID | STATUS_SYS_MODE;
	state->CP15_Control = CP15_DISABLE_VM(state->CP15_Control);
	state->CP15_EntryHi=0;
	state->CP15_Cause=0;
	state->TOD_Hi=0;
	state->TOD_Low=0;
	//STATUS_ALL_INT_DISABLE
	
}


int main(){			
	*interval_timer=clock_rate;
	
	initState((void*)INT_NEWAREA, intHandler,1);
	initState((void*)TLB_NEWAREA, sysHandler,1);
	initState((void*)PGMTRAP_NEWAREA, sysHandler,1);
	initState((void*)SYSBK_NEWAREA, sysHandler,1);
	int i=0;
	/*while(i<20){
		i++;
		tprint("in while (20 iterazioni)\n");
	}*/
	
	//STATUS_ALL_INT_ENABLE
	state_t newState;
	initState(&newState, pippoInutile,0);
	LDST(&newState);
	tprint("starting!!!!\n");
	SYSCALL(1,0,0,0);
	tprint("syscall finished!!!!!\n");
	
	while(1){
		tprint("in while (main)\n");
	}
	HALT();

	tprint("error");


return 0;
}
