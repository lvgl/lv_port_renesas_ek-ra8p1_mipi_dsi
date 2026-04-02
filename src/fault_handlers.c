#include "bsp_api.h"

void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void SecureFault_Handler(void);
void UsageFault_Handler( void );

/*
 * Need to enable individual fault handlers :-
 *
 *     SCB->SHCSR |= (SCB_SHCSR_USGFAULTENA_Msk | \
            SCB_SHCSR_BUSFAULTENA_Msk | \
            SCB_SHCSR_MEMFAULTENA_Msk );
 */

void HardFault_Handler(void)
{
    assert(0);
}

void MemManage_Handler(void)
{
    assert(0);
}

void BusFault_Handler(void)
{
    assert(0);
}

void SecureFault_Handler(void)
{
    assert(0);
}

void UsageFault_Handler(void)
{
    assert(0);
}

