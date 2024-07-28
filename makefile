.PHONY: all
all: task_power task_xray_process

task_power: task_power.c sc_msg.h
	gcc -o task_power task_power.c sc_msg.c

task_xray_process: task_xray_process.c sc_msg.h sc_msg.c
	gcc -o task_xray_process task_xray_process.c sc_msg.c

.PHONY: clean
clean:
	rm task_power task_xray_process