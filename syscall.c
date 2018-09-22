#include "syscall.h"
#include "./asl/asl.h"
#include "./pcb/pcb_t.h"
#include "./pcb/const.h"
#include "/usr/include/uarm/uARMtypes.h"

void P(int *semaddr){
	if(*semaddr>0){		
		*semaddr--;
	}else{							//caso il processo viene bloccato
		pcb_t * pid=removeProcQ(&pcb_wait);		//rimuovo il processo in testa a quelli in esecuzione e quindi il processo chiamante
		if (pid!=0){					//se non ci sono stati problemi,  
			int insertBlocked(semaddr,pid);		//il processo viene inserito tra i processi bloccati dal semaforo semaddr
		}
	}
}
void V(int *semaddr){
	
	pcb_t* pid=removeBlocked(semaddr);			//provo a sbloccare processo bloccato
	if (pid!=0){						//se esiste (c'e' almeno un processo bloccato)
		insertProcQ(&pcb_wait, pid);			//allora lo inserisco tra i processi in esecuzione
	} 
	else{
		*semaddr++;
	}
}
int CREATEPROCESS(state_t *statep,int priority,void **cpid){//pcb_wait è la lista di pcb globale
	pcb_t* newpcb=allocPcb();
	if (newpcb==NULL){
		return -1;
	}
	newpcb->p_s=*statep;
	pcb_t *parent=headProcQ(pcb_wait);//parent è il padre del proceso newpcb appena creato, la testa della lista è il processo corrente
	if (parent==NULL){
		insertProcQ(&pcb_wait,newpcb);//se la lista è vuota inserisce il primo elemento
	}
	else{
		insertChild(parent,newpcb);
		insertProcQ(&pcb_wait,newpcb);
	}
	*cpid=newpcb;////////////////////////da vedere
	return 0;
}
int TERMINATEPROCESS(void *pid){
	if(pcb_wait==NULL){
		return -1;
	}
	pcb_t *root;
	if(pid==NULL){//eliminare proceso attuale
		root=removeProcQ(&pcb_wait);//tolgo la testa dalla lista
	}
	else{//eliminare processo desiderato
		root=outProcQ(&pcb_wait, pid);//tolgo un elemento dalla lista
	}
	pcb_t *child=NULL;//inizializzo child
	do{//prima vado al proproproproNipote e inizio a eliminare i figli, poi passo al padre, elimino, i figli, poi passo al nonno ed elimino i figli
		child=removeChild(root);
		if(child!=NULL){//per non far partire TERMINATEPROCESS senza motivo. Altrimenti eliminerebbe la nuova testa.In questo modo eliminerei tutti i processi esistenti
			TERMINATEPROCESS(child);
		}
	}while(child!=NULL);//finchè ci sono figli
	if(root!=NULL){//se la testa esiste aggiungo il pcb alla freePcb
		freePcb(root);
	} 
	if(pid!=NULL){
		return 0;
	}
}*/
