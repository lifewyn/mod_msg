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

static bool xray_send_msg(int queue_number, struct my_queue_data * pointer_para)
{
    printf("\n@@@enter xray_send_msg\n");

    int send_ret = 0;

    send_ret = msgsnd(queue_number, pointer_para, RTU_DATA_SIZE, 0);
    printf("the sent ret is %d\n", send_ret);
    if(send_ret == 0)
    {
        printf("the queue is sent\n");
        printf("the rtu sender is %d\n", pointer_para->my_rtu.rtu_sender);
        printf("the rtu addr is 0x%x\n", pointer_para->my_rtu.addr);
        printf("@@@OK leave xray_send_msg\n");
    }
    else
    {
        printf("xray send exit with error\n");
        perror("msg send error:");
        exit -1;
    }

    return true;
}

static bool xray_rec_msg(int queue_num, struct my_queue_data * pointer_para)
{
    printf("\n***enter xray_rec_msg\n");

    int rec_ret = 0;

    rec_ret = msgrcv(queue_num, pointer_para, RTU_DATA_SIZE, pointer_para->msg_type, 0);
    printf("the rec ret is %d\n", rec_ret);
    if(rec_ret != -1)
    {
        printf("rec ret is oK\n");
        printf("the rec rtu sender is %d\n", pointer_para->my_rtu.rtu_sender);
        printf("the rec rtu addr is 0x%x\n", pointer_para->my_rtu.addr);
        printf("***OK leave xray_rec_msg\n");
    }
    else
    {
        printf("xray rec exit with error\n");
        perror("msg rec error:");
        exit -1;
    }

    return true;
}

int main()
{
    int xray2power_queue_id = -1;
    int xray_rec_queue_id = -1;
    int xray2lcd_queue_id = -1;
    //int xray2db_queue_id = -1;
    int process_count = 1;

    struct my_queue_data xray_rec_data;
    memset(&xray_rec_data, 0, sizeof(struct my_queue_data));
    xray_rec_data.msg_type = REC_MSG_TYPE;

    struct my_queue_data xray_send_data;
    memset(&xray_send_data, 0, sizeof(struct my_queue_data));
    xray_send_data.msg_type = SEND_MSG_TYPE;

    xray_rec_queue_id = get_sc_msg_queue_id(ALL_TO_XRAY_QUEUE_KEY);

    xray2power_queue_id = get_sc_msg_queue_id(XRAY_TO_POWER_STATUS_QUEUE_KEY);

    xray2lcd_queue_id = get_sc_msg_queue_id(XRAY_TO_LCD_QUEUE_KEY);

    //xray2db_queue_id = get_sc_msg_queue_id(XRAY_TO_DB_QUEUE_KEY);

    while(1)
    {
        printf("\n****************xray process time %d begin**************\n", process_count);

        xray_rec_msg(xray_rec_queue_id, &xray_rec_data);
        //xray_send_msg(xray2db_queue_id, &xray_rec_data);

        //should call the modbus lib to run cmd adn return the data
        memcpy(&xray_send_data.my_rtu, &xray_rec_data.my_rtu, RTU_DATA_SIZE);

        printf("\n***after meemcpy the rec data is:\n");
        printf("the send msg_type is %ld\n", xray_send_data.msg_type);
        printf("the send rtu sender is %d\n", xray_send_data.my_rtu.rtu_sender);
        printf("the send rtu addr is 0x%x\n", xray_send_data.my_rtu.addr);

        switch(xray_rec_data.my_rtu.rtu_sender)
        {
            case POWER:
                xray_send_msg(xray2power_queue_id, &xray_send_data);
                //xray_send_msg(xray2db_queue_id, &xray_send_data);
                break;

            case LCD:
                xray_send_msg(xray2lcd_queue_id, &xray_send_data);
                //xray_send_msg(xray2db_queue_id, &xray_send_data);
                break;            

            default:
                printf("invalid sender\n");
        }

        printf("****************process_count time %d over**************\n", process_count);
        process_count++;

        sleep(2);
    }

    return 0;
}