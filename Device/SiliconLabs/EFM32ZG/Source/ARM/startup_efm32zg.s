;/**************************************************************************//**
; * @file startup_efm32zg.s
; * @brief    CMSIS Core Device Startup File for
; *           Silicon Labs EFM32ZG Device Series
; * @version 4.4.0
; * @date     03. February 2012
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
                IF :DEF: __STACK_SIZE
Stack_Size      EQU     __STACK_SIZE
                ELSE
Stack_Size      EQU     0x00000400
                ENDIF

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
                IF :DEF: __HEAP_SIZE
Heap_Size       EQU     __HEAP_SIZE
                ELSE
Heap_Size       EQU     0x0
                ENDIF

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY, ALIGN=8
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts

                DCD     DMA_IRQHandler        ; 0: DMA Interrupt
                DCD     GPIO_EVEN_IRQHandler        ; 1: GPIO_EVEN Interrupt
                DCD     TIMER0_IRQHandler        ; 2: TIMER0 Interrupt
                DCD     ACMP0_IRQHandler        ; 3: ACMP0 Interrupt
                DCD     ADC0_IRQHandler        ; 4: ADC0 Interrupt
                DCD     I2C0_IRQHandler        ; 5: I2C0 Interrupt
                DCD     GPIO_ODD_IRQHandler        ; 6: GPIO_ODD Interrupt
                DCD     TIMER1_IRQHandler        ; 7: TIMER1 Interrupt
                DCD     USART1_RX_IRQHandler        ; 8: USART1_RX Interrupt
                DCD     USART1_TX_IRQHandler        ; 9: USART1_TX Interrupt
                DCD     LEUART0_IRQHandler        ; 10: LEUART0 Interrupt
                DCD     PCNT0_IRQHandler        ; 11: PCNT0 Interrupt
                DCD     RTC_IRQHandler        ; 12: RTC Interrupt
                DCD     CMU_IRQHandler        ; 13: CMU Interrupt
                DCD     VCMP_IRQHandler        ; 14: VCMP Interrupt
                DCD     MSC_IRQHandler        ; 15: MSC Interrupt
                DCD     AES_IRQHandler        ; 16: AES Interrupt
                DCD     0                         ; 17: Reserved
                DCD     0                         ; 18: Reserved

__Vectors_End
__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  DMA_IRQHandler        [WEAK]
                EXPORT  GPIO_EVEN_IRQHandler        [WEAK]
                EXPORT  TIMER0_IRQHandler        [WEAK]
                EXPORT  ACMP0_IRQHandler        [WEAK]
                EXPORT  ADC0_IRQHandler        [WEAK]
                EXPORT  I2C0_IRQHandler        [WEAK]
                EXPORT  GPIO_ODD_IRQHandler        [WEAK]
                EXPORT  TIMER1_IRQHandler        [WEAK]
                EXPORT  USART1_RX_IRQHandler        [WEAK]
                EXPORT  USART1_TX_IRQHandler        [WEAK]
                EXPORT  LEUART0_IRQHandler        [WEAK]
                EXPORT  PCNT0_IRQHandler        [WEAK]
                EXPORT  RTC_IRQHandler        [WEAK]
                EXPORT  CMU_IRQHandler        [WEAK]
                EXPORT  VCMP_IRQHandler        [WEAK]
                EXPORT  MSC_IRQHandler        [WEAK]
                EXPORT  AES_IRQHandler        [WEAK]


DMA_IRQHandler
GPIO_EVEN_IRQHandler
TIMER0_IRQHandler
ACMP0_IRQHandler
ADC0_IRQHandler
I2C0_IRQHandler
GPIO_ODD_IRQHandler
TIMER1_IRQHandler
USART1_RX_IRQHandler
USART1_TX_IRQHandler
LEUART0_IRQHandler
PCNT0_IRQHandler
RTC_IRQHandler
CMU_IRQHandler
VCMP_IRQHandler
MSC_IRQHandler
AES_IRQHandler

                B       .
                ENDP

                ALIGN

; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF

                END
