	AREA datos, DATA
		
; vectores de deltas
dta_fila	DCB	0, -1, -1, 1
dta_columna	DCB	-1, 0, -1, -1
	
	AREA codigo, CODE
		
	EXPORT conecta_K_hay_linea_arm_arm
	IMPORT conecta_K_buscar_alineamiento_arm
		PRESERVE8 {TRUE}
			
	; NOTA: FALSE = 0
	; NOTA: N_DELTAS = 4  (K_SIZE)
		
conecta_K_hay_linea_arm_arm
	
	; PROLOGO
	MOV IP, SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	;-----------------------
	
	MOV r4, r0	; cuadr�cula
	MOV r5, r1	; fila
	MOV r6, r2	; columna
	MOV r7, r3	; color
	
	; Inicio	
	
	MOV r8, #0 	; unsigned int i = 0
	MOV r9, #0	; uint8_t linea = 0
	MOV r10, #0	; uint8_t long_linea = 0
	
	; for(i=0; (i < 4) && (linea == FALSE); ++i)
bucle_for

	; Condici�n de bucle 
	; (i < N_DELTAS) && (linea == FALSE)
	CMP r8, #4
	BGE return
	CMP r9, #0
	BNE return
	
	; Apilar deltas como par�metros
	
	; @ de vectores de deltas
	LDR r1, =dta_fila
	LDR r2, =dta_columna
	
	; lectura de la i-�sima componente
	; de cada vector
	LDRSB r1, [r1, r8]	; dta_fila[i]
	LDRSB r2, [r2, r8]	; dta_columna[i]
	STMDB sp!, {r1, r2}	
	
	; par�metros fila y columna originales
	MOV r1, r5	; r1 = fila_original
	MOV r2, r6	; r2 = columna_original
	
	BL conecta_K_buscar_alineamiento_arm
	MOV r10, r0	; long_linea = conecta_K_buscar_alineamiento_arm()
	
	; 
	CMP r10, #4		; long_linea >= K_SIZE
	MOVGE r9, #1	
	MOVLT r9, #0
	BGE sigue_bucle_for
	
	; Buscar sentido inverso
	
	; @ de vectores de deltas
	LDR r1, =dta_fila
	LDR r2, =dta_columna
	
	; lectura de la i-�sima componente
	; de cada vector
	LDRSB r1, [r1, r8]	; dta_fila[i]
	LDRSB r2, [r2, r8]	; dta_columna[i]
	
	; -deltas_fila[i]
	NEG r1, r1
	; -deltas_columna[i]
	NEG r2, r2
	; apilamos los deltas negados (par�metros 5 y 6)
	STMDB sp!, {r1, r2}	
	
	; fila + (- delta_fila[i])
	ADD r1, r5, r1
	
	; columna + (- delta_columna[i])
	ADD r2, r6, r2	
	
	; par�metros r0-r3
	MOV r0, r4  ; cuadricula
	MOV r3, r7  ; color
	
	BL conecta_K_buscar_alineamiento_arm
	
	ADD r10, r10, r0 ; long_linea += conecta..._arm()
	
	CMP r10, #4		;  linea = long_linea >= K_SIZE
	MOVGE r9, #1
	MOVLT r9, #0
	
sigue_bucle_for

	; i++ y volver a inicio del bucle
	; para comprobar condiciones
	ADD r8, r8, #1
	MOV r0, r4	; recuperar cuadr�cula
	B bucle_for

return

	MOV r0, r9 ; r0 = linea
	
	; Epilogo
	LDMDB FP, {r4-r10,FP,SP,PC}
	;----------------
	
	END
	