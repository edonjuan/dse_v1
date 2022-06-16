/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>


/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>



/* Board Header file */
#include "Board.h"

#define TASKSTACKSIZE   512
#define TIMEOUT         12      /* Timeout value */

Task_Struct task1Struct, task2Struct;
Char task1Stack[TASKSTACKSIZE];
Char task2Stack[TASKSTACKSIZE];

Mailbox_Struct mbxStruct;
Mailbox_Handle mbxHandle;

typedef struct MsgObj {
    Int         id;
    Char        val;
} MsgObj, *Msg;


Void task1(UArg arg0, UArg arg1)
{
    MsgObj msg;

    msg.id = 1;
    msg.val = 'a';

    while (1) {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED0);

        if(Mailbox_post(mbxHandle, &msg, BIOS_NO_WAIT))
        {
            System_printf("T1: Massage sent! \n");
        }
        else
        {
            System_printf("T1: SLOTS No available \n");
        }
    }
}

Void task2(UArg arg0, UArg arg1)
{
    MsgObj msg;
    static int cont;

    while (1) {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED1);


        if (Mailbox_pend(mbxHandle, &msg, BIOS_NO_WAIT))
        {
            System_printf("T2: read id = %d and val = '%c'.\n",msg.id, msg.val);
         }
        else
        {
            System_printf("T2: No massages in mailbox \n");
        }

        if(cont++>6)
        {
            BIOS_exit(0);
        }
    }
}


/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;
    Mailbox_Params mbxParams;

    /* Call board init functions */
    Board_initGeneral();
    // Board_initEMAC();
    Board_initGPIO();
    // Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initUSBMSCHFatFs();
    // Board_initWatchdog();
    // Board_initWiFi();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 500;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task1Stack;
    Task_construct(&task1Struct, (Task_FuncPtr)task1, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task2Stack;
    Task_construct(&task2Struct, (Task_FuncPtr)task2, &taskParams, NULL);

    //evtHandle = Event_handle(&evtStruct);

    /* Construct a Mailbox Instance */
    Mailbox_Params_init(&mbxParams);
    Mailbox_construct(&mbxStruct,sizeof(Int), 2, &mbxParams, NULL);
    mbxHandle = Mailbox_handle(&mbxStruct);

     /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    /* Start BIOS */
    System_printf("START BIOS \n");
    BIOS_start();


    return (0);
}
