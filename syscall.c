#include "syscall.h"

void P(int *semaddr){
	if(*semaddr>0){		
		*semaddr--;
	}else{							//caso il processo viene bloccato
		pcb_t * pid=removeProcQ(&pcb_list);		//rimuovo il processo in testa a quelli in esecuzione e quindi il processo chiamante
		if (pid!=0){					//se non ci sono stati problemi,  
			int insertBlocked(semaddr,pid);		//il processo viene inserito tra i processi bloccati dal semaforo semaddr
		}
	}
}
void V(int *semaddr){
	
	pcb_t* pid=removeBlocked(semaddr);			//provo a sbloccare processo bloccato
	if (pid!=0){						//se esiste (c'e' almeno un processo bloccato)
		insertProcQ(&pcb_list, pid);			//allora lo inserisco tra i processi in esecuzione
	} 
	else{
		*semaddr++;
	}
}
int CREATEPROCESS(state_t *statep,int priority,void **cpid){//pcb_list è la lista di pcb globale
	pcb_t* newpcb=allocPcb();
	if (newpcb==NULL){
		return -1;
	}
	newpcb->p_s=statep;
	pcb_t *parent=headProcQ(pcb_list);//parent è il padre del proceso newpcb appena creato, la testa della lista è il processo corrente
	if (parent==NULL){
		insertProcQ(&pcb_list,newpcb);//se la lista è vuota inserisce il primo elemento
	}
	else{
		insertChild(parent,newpcb);
		insertProcQ(&pcb_list,newpcb);
	}
	*cpid=newpcb;////////////////////////da vedere
	return 0;
}
int TERMINATEPROCESS(void *pid){
	if(pcb_list==NULL){
		return -1;
	}
	if(pid==NULL){//eliminare proceso attuale
		pcb_t *root=removeProcQ(&pcb_list);//tolgo la testa dalla lista
	}
	else{//eliminare processo desiderato
		pcb_t *root=outProcQ(&pcb_list, pid);//tolgo un elemento dalla lista
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
}
