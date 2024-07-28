.PHONY: all
all: my_send my_rec task_power task_xray_process task_lcd

my_send: my_send.c sc_msg.h
	gcc -o my_send my_send.c

my_rec: my_rec.c sc_msg.h
	gcc -o my_rec my_rec.c

task_power: task_power.c sc_msg.h
	gcc -o task_power task_power.c sc_msg.c

task_xray_process: task_xray_process.c sc_msg.h sc_msg.c
	gcc -o task_xray_process task_xray_process.c sc_msg.c

task_lcd: task_lcd.c sc_msg.h
	gcc -o task_lcd task_lcd.c -lpthread

.PHONY: clean
clean:
	rm my_send my_rec task_power task_xray_process task_lcd