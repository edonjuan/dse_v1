/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>

/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>

/* Board Header file */
#include "Board.h"

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Swi_Struct swi0Struct;
Swi_Handle swi0Handle;

Char task0Stack[TASKSTACKSIZE];

Void heartBeatFxn(UArg arg0, UArg arg1)
{
    UInt key;

    while (1) {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED0);

        System_printf("** INSTANCIA SW0**\n");
        System_printf("Swi0_post: ");
        Swi_post(swi0Handle);

        System_printf("\n** INSTANCIA SW1**\n");
        System_printf("Swi1_post: ");
        Swi_post(swi1Handle);

        System_printf("Swi's disabled\n");
        key = Swi_disable();
        System_printf("Swi0_inc 2 tries... \n");
        Swi_inc(swi0Handle);
        Swi_inc(swi0Handle);
        System_printf("Swi's enabled\n");
        Swi_restore(key);

        System_printf("Swi0_or (25):\n");
        Swi_or(swi0Handle, 25);

        // ANDN and DEC
        System_printf("Swi1_andn 0b01:\n");
        Swi_andn(swi1Handle, 0x01);
        System_printf("Swi1_andn 0b10:\n");
        Swi_andn(swi1Handle, 0x02);

        System_printf("Swi1_dec1:\n");
        Swi_dec(swi1Handle);
        System_printf("Swi1_dec2:\n");
        Swi_dec(swi1Handle);
        System_printf("Swi1_dec3:\n");
        Swi_dec(swi1Handle);

        System_printf("Calling BIOS_exit\n");
        BIOS_exit(0);
    }
}

Void swi0Fxn(UArg arg0, UArg arg1)
{
    System_printf("swi0Fxn POSTED! --> trigger0 = %d\n\n", Swi_getTrigger());
}

Void swi1Fxn(UArg arg0, UArg arg1)
{
    System_printf("swi1Fxn POSTED! --> trigger1 = %d\n\n", Swi_getTrigger());
}

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;
    /* Call board init functions */
    Board_initGeneral();
    // Board_initEMAC();
    Board_initGPIO();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);

    Swi_Params swiParams;
    Swi_Params_init(&swiParams);
    swiParams.trigger = 0;

    Swi_construct(&swi0Struct, (Swi_FuncPtr)swi0Fxn, &swiParams, NULL);
    swi0Handle = Swi_handle(&swi0Struct);

     /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
