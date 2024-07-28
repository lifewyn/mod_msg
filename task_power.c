#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdbool.h>

#include "sc_msg.h"

static bool power_send_msg(int queue_number, struct my_queue_data * pointer_para)
{
    printf("\n@@@enter power_send_msg\n");

    int send_ret = 0;

    send_ret = msgsnd(queue_number, pointer_para, RTU_DATA_SIZE, 0);
    printf("the sent ret is %d\n", send_ret);
    if(send_ret == 0)
    {
        printf("the queue is sent\n");
        printf("the rtu sender is %d\n", pointer_para->my_rtu.rtu_sender);
        printf("the rtu addr is 0x%x\n", pointer_para->my_rtu.addr);
        printf("@@@OK leave power_send_msg\n");
    }
    else
    {
        printf("power send exit with error\n");
        perror("msg send error:");
        exit -1;
    }

    return true;
}

static bool power_rec_msg(int queue_num, struct my_queue_data * pointer_para)
{
    printf("\n***enter power_rec_msg\n");

    int rec_ret = 0;

    rec_ret = msgrcv(queue_num, pointer_para, RTU_DATA_SIZE, pointer_para->msg_type, 0);
    printf("the rec ret is %d\n", rec_ret);
    if(rec_ret != -1)
    {
        printf("rec ret is oK\n");
        printf("the rec rtu sender is %d\n", pointer_para->my_rtu.rtu_sender);
        printf("the rec rtu addr is 0x%x\n", pointer_para->my_rtu.addr);
        printf("***OK leave power_rec_msg\n");
    }
    else
    {
        printf("power rec exit with error\n");
        perror("msg rec error:");
        exit -1;
    }

    return true;
}

int main()
{
    int xray_rec_queue_id = -1;
    int xray2power_queue_id = -1;
    int process_count = 1;

    struct my_queue_data power_rec_data;
    memset(&power_rec_data, 0, sizeof(struct my_queue_data));
    power_rec_data.msg_type = REC_MSG_TYPE;

    struct my_queue_data power_send_data;
    memset(&power_send_data, 0, sizeof(struct my_queue_data));
    power_send_data.msg_type = SEND_MSG_TYPE;

    xray_rec_queue_id = get_sc_msg_queue_id(ALL_TO_XRAY_QUEUE_KEY);

    xray2power_queue_id = get_sc_msg_queue_id(XRAY_TO_POWER_STATUS_QUEUE_KEY);

    power_send_data.msg_type = SEND_MSG_TYPE;
    power_send_data.my_rtu.rtu_sender = POWER;
    power_send_data.my_rtu.addr = 0x56;

    power_rec_data.msg_type = REC_MSG_TYPE;

    while(1)
    {
        printf("\n****************power process time %d begin**************\n", process_count);

        power_send_msg(xray_rec_queue_id, &power_send_data);

        power_rec_msg(xray2power_queue_id, &power_rec_data);

        printf("****************power process_count time %d over**************\n", process_count);

        sleep(3);
    }

    return 0;
}