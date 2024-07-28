#ifndef	_SC_MSG_H

# define _SC_MSG_H	1

#include <sys/types.h>

//msg type
#define SEND_MSG_TYPE 1
#define REC_MSG_TYPE 0

//msg_process_delay_time
#define SENDER_SLEEP_TIME 6
#define REC_SLEEP_TIME 2

//msg_queue_key
#define ALL_TO_XRAY_QUEUE_KEY 1352
#define XRAY_TO_POWER_STATUS_QUEUE_KEY 1360
#define XRAY_TO_LCD_QUEUE_KEY 1368
#define XRAY_TO_ENMERGENT_OFF_QUEUE_KEY 1376
#define XRAY_TO_DELAY_START_QUEUE_KEY 1384
#define XRAY_TO_EX485_QUEUE_KEY 1392
#define XRAY_TO_RJ45_QUEUE_KEY 1400
#define XRAY_TO_DB_QUEUE_KEY 1408

#define RTU_DATA_SIZE sizeof(struct sc_rtu)

#define SC_REGISTER_NUM 12

enum SENDER
{
    XRAY485 = 1, POWER, LCD  
};

struct sc_rtu
{
    enum SENDER rtu_sender;
    enum SENDER rtu_rec;
    u_int8_t addr;
};

/*
struct sc_rtu_msg
{
    u_int8_t sc_device_slave_addr;
    u_int8_t sc_cmd;
    u_int8_t sc_data_num;
    u_int16_t sc_start_addr;
    u_int8_t sc_rtu_data[SC_REGISTER_NUM];
};
*/

struct my_queue_data
{
    long int msg_type;
    struct sc_rtu my_rtu;
};

struct thread_para
{
    struct my_queue_data queue_data;
    int queue_num;
};

int get_sc_msg_queue_id(int queue_key);

#endif