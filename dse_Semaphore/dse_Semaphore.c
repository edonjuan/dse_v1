
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>

/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/USBMSCHFatFs.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

/* Board Header file */
#include "Board.h"

#define TASKSTACKSIZE   512

Task_Struct task1Struct,task2Struct;
Semaphore_Struct semStruct;
Semaphore_Handle semHandle;

Void task1Fxn(UArg arg0, UArg arg1);
Void task2Fxn(UArg arg0, UArg arg1);

Int recurso = 0;
UInt32 time_sleep;
Char task1Stack[TASKSTACKSIZE];
Char task2Stack[TASKSTACKSIZE];


/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();

    Task_Params taskParams;
    Semaphore_Params semParams;

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task1Stack;
    taskParams.priority = 1;
    Task_construct(&task1Struct, (Task_FuncPtr)task1Fxn, &taskParams, NULL);

    taskParams.stack = &task2Stack;
    taskParams.priority = 1;
    Task_construct(&task2Struct, (Task_FuncPtr)task2Fxn, &taskParams, NULL);

    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&semStruct, 1, &semParams);
    semHandle = Semaphore_handle(&semStruct);

    time_sleep = 20000 / Clock_tickPeriod; // 10000uS = 100mS
    System_printf("Running BIOS\n\n");
    BIOS_start();
    return(0);
}



Void task1Fxn(UArg arg0, UArg arg1)
{
    // APARTAR RECURSO

    Semaphore_pend(semHandle,BIOS_WAIT_FOREVER);
    System_printf("TAREA1 running\n");

    while(1)
    {
        if(recurso < 8)
        {
            recurso ++;
            System_printf("Recurso usado en Tarea1, recurso = %d \n" , recurso);
        }
        else
        {
            Semaphore_post(semHandle);
            System_printf("TAREA1: RECURSO LIBERADO\n\n");
            break;
        }
        Task_sleep(time_sleep);
    }
}

/*
 *  ======== task2Fxn ========
 */
Void task2Fxn(UArg arg0, UArg arg1)
{
    // APARTAR RECURSO
    Semaphore_pend(semHandle,BIOS_WAIT_FOREVER);

    System_printf("TAREA2 running\n");

    while(1)
    {
        if(recurso > 3)
        {
            recurso --;
            System_printf("Recurso usado en Tarea2, recurso = %d \n" , recurso);
        }
        else
        {
            Semaphore_post(semHandle);
            System_printf("Calling BIOS_exit from task2\n");
            BIOS_exit(0);
        }
        Task_sleep(time_sleep);
    }
}

