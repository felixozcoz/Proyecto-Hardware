;/*****************************************************************************/
;/* SWI.S: SWI Handler                                                        */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/

T_Bit        	EQU     0x0020
F_Bit			EQU     0x0040
I_Bit			EQU		0x0080 ; bit7 de la CPSR, si 1 inhibe IRQ

                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  SWI_Handler
SWI_Handler   

                STMFD   SP!, {R12, LR}         ; Store R12, LR
                MRS     R12, SPSR              ; Get SPSR
                STMFD   SP!, {R8, R12}         ; Store R8, SPSR
                TST     R12, #T_Bit            ; Check Thumb Bit
                LDRNEH  R12, [LR,#-2]          ; Thumb: Load Halfword
                BICNE   R12, R12, #0xFF00      ; Extract SWI Number
                LDREQ   R12, [LR,#-4]          ; ARM:   Load Word
                BICEQ   R12, R12, #0xFF000000  ; Extract SWI Number

; add code to enable/disable the global IRQ flag
				CMP     R12,#0xFF              
                BEQ     __enable_irq

				CMP     R12,#0xFE              
                BEQ     __disable_irq
				
				CMP     R12,#0xFD              
                BEQ     __disable_fiq

                CMP     R12,#0xFC              
                BEQ     __read_IRQ_bit
				

				LDR     R8, SWI_Count
                CMP     R12, R8
                BHS     SWI_Dead               ; Overflow
                ADR     R8, SWI_Table
                LDR     R12, [R8,R12,LSL #2]   ; Load SWI Function Address
                MOV     LR, PC                 ; Return Address
                BX      R12                    ; Call SWI Function 
				
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

SWI_Dead        B       SWI_Dead               ; None Existing SWI

SWI_Cnt         EQU    (SWI_End-SWI_Table)/4
SWI_Count       DCD     SWI_Cnt

                IMPORT  __SWI_0
;				IMPORT  __SWI_1
;				IMPORT  __SWI_2
;				IMPORT  __SWI_3
SWI_Table
                DCD     __SWI_0                ; SWI 0 Function Entry
;               DCD     __SWI_1                ; SWI 1 Function Entry
;               DCD     __SWI_2                ; SWI 2 Function Entry
;				DCD     __SWI_3                ; SWI 3 Function Entry

;               ...
SWI_End



;------------------------------- enable_irq --------------------------------------------------
__enable_irq	
				MRS		R8, SPSR				; Leer CPSR del programa
				ORR 	R8,R8,#I_Bit 			; Escribe 1 en bit I de SPSR (disable interrupt IRQ bit)
				MSR		SPSR_cxsf, R8				; Copiamos el nuevo estado en el registro de estado

				; Epilogo SWI
                LDMFD   SP!, {R8, R12}          ; Load R8, SPSR
                MSR     SPSR_cxsf, R12        	; Set SPSR
                LDMFD   SP!, {R12, PC}^        	; Restore R12 and Return
				;------------------------------
;-------------------------------------------------------------------------------------------------------


;------------------------------- disable_irq --------------------------------------------------
__disable_irq	
				MRS		R8, SPSR			   ; Leer CPSR del programa
				ORR 	R8,R8,#I_Bit 		   ; Escribe 1 en bit I de SPSR (disable interrupt IRQ bit)
				MSR		SPSR_cxsf, R8			   ; Copiamos el nuevo estado en el registro de estado

				; Epilogo SWI
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
				;------------------------------
;-------------------------------------------------------------------------------------------------------


;------------------------------- disable_fiq --------------------------------------------------
__disable_fiq	
				MRS		R8, SPSR			   ; Leer CPSR del programa
				ORR 	R8,R8,#F_Bit 		   ; Escribe 1 en bit F de SPSR (disable interrupt FIQ bit)
				MSR		SPSR_cxsf, R8			   ; Copiamos el nuevo estado en el registro de estado

				; Epilogo SWI
                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
				;------------------------------
;-------------------------------------------------------------------------------------------------------


;------------------------------- read_IRQ_bit --------------------------------------------------
				EXTERN bit_irq [DATA, SIZE=4]
	
__read_IRQ_bit	
				MRS			R8, SPSR		; Leer CPSR del programa
				TST			R8, #I_Bit		; check bit I (disable IRQ interrupt bit)
				MOVNE		R8, #1						
				MOVEQ 		R8, #0
				
				LDR 		R12, =bit_irq
				STR 		R8, [R12]		; retornar resultado	

				; Epilogo SWI
                LDMFD   SP!, {R8, R12}      ; Load R8, SPSR
                MSR     SPSR_cxsf, R12      ; Set SPSR
                LDMFD   SP!, {R12, PC}^     ; Restore R12 and Return
				
				END
;-------------------------------------------------------------------------------------------------------

;                EXTERN shared_var [DATA,SIZE=4]

;__decrease_var
;                LDR R8, =shared_var;
;				 LDR R12, [r8]
;                SUB R12, R12, #1
;                STR R12, [R8]
;				
;				; Epilogo SWI
;                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
;                MSR     SPSR_cxsf, R12         ; Set SPSR
;                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return
;				;------------------------------

;                END

