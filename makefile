YAEOS: kernel_handler.o pcb_t.o asl.o syscall.o
	arm-none-eabi-ld -T /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x /usr/include/uarm/crtso.o /usr/include/uarm/libuarm.o kernel_handler.o pcb_t.o asl.o syscall.o -o YAEOS

kernel_handler.o: kernel_handler.c  syscall.o
	arm-none-eabi-gcc -c -T arm7tdmi kernel_handler.c

syscall.o: syscall.c pcb_t.o asl.o syscall.h
	arm-none-eabi-gcc -c -T arm7tdmi syscall.c

asl.o: asl/asl.c asl/asl.h pcb_t.o
	arm-none-eabi-gcc -c -T arm7tdmi asl/asl.c

pcb_t.o: pcb/pcb_t.c pcb/pcb_t.h
	arm-none-eabi-gcc -c -T arm7tdmi pcb/pcb_t.c

clean:
	rm -f *.o YAEOS
