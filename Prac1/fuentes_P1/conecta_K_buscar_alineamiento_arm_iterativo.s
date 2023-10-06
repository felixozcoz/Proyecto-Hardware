	AREA codigo,CODE
		
	EXPORT conecta_K_buscar_alineamiento_arm
		
conecta_K_buscar_alineamiento_arm
	; PROLOGO
	MOV IP, SP
	STMDB SP!, {r1-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	;-----------------------

	; Desapilar Parámetros Deltas
	LDR r4, [fp, #4]	; delta_fila
	LDR r5, [fp, #8]	; delta_columna
	
	; tablero_buscar_color
	
	; fila < NUM_FILAS
	CMP r1, #7			
	BGE FINtab	; termina función tablero_buscar_color()
	CMP r1, #0
	BLT FINtab	; "   "    "
	
	; columna < NUM_COLUMNAS
	CMP r2, #7										
	BGE FINtab
	CMP r2, #0
	BLT FINtab
	
	; size_t col = 0
	MOV r6, #0
	
	; r6 = fila * MAX_NO_CERO
	MOV r7, #6	
	MUL r7, r1, r7	

FORtab

	; col < MAX_NO_CERO 
	CMP r6, #6			
	BGE loop_exit
	
	; ... && (t->columnas[fila][col] != columna)

	; calcular @ de la celda
	ADD r8, r7, r6		; r8 = (fila * 6) + col
	LDRSB r9, [r0, r8]     ; r8 = t->columnas[fila][col]
	
	; t->columnas[fila][col] != columna ? col++ e iterar : salir bucle
    CMP r9, r2
	ADDNE r6, r6, #1	; col++
    BNE FORtab ; iterar
	
loop_exit

	; col == MAX_NO_CERO ? ERROR : seguir bucle
	CMP r6, #6	
	MOVEQ r8, #1-	; return ERROR
	BEQ FINtab

	; sigue tablero_buscar_color()
	
	;(fila * 6) + col 
	ADD r7, r7, r6
	; (fila * 6 + col) + (MAX_NO_CERO * NUM_LINEAS)
	ADD r7, r7, #42
	; @tablero + desplazamiento = t->no_cero[fila][col]
	LDRSB r7, [r0, r7] 			
	
	; check color celda
	AND r7, r7, #0x03
	
	; celda_color(t->no_ceros[fila][col]) == color)
	CMP r7, r3
	MOVNE r8, #-1	; return ERROR
	MOVEQ r8, #0

	; r0 == EXITO ?
	CMP r8, #0    	
	MOVNE r8, #0     ; return 
	BNE epilogo_buscar_alineamiento     
	
sigue_buscar_alineamiento

	; Calcular fila y columna nueva
	ADD r1, r1, r6  	; fila + delta_fila
	ADD r2, r2, r7  	; columna + delta_columna

	; Cargar deltas en pila
	STR r7, [sp,#-4]! 	; delta_columna
	STR r6, [sp,#-4]!	; delta_fila

	; restaurar tablero a r0
	MOV r0, r5 	

	BL conecta_K_buscar_alineamiento_arm
	
	ADD r0,r0,#1	; return 1 + conecta_K_buscar_alineamiento_arm()
	B epilogo_buscar_alineamiento
	
tablero_buscar_color
	; PROLOGO
	MOV IP, SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	;-----------------------
	
	; Verificar condiciones de fila y columna
	; sobre dimensiones de tablero
	
	; fila < NUM_FILAS
	CMP r1, #7			
	BGE tablero_buscar_color_ERROR
	CMP r1, #0
	BLT tablero_buscar_color_ERROR
	
	; columna < NUM_COLUMNAS
	CMP r2, #7										
	BGE tablero_buscar_color_ERROR
	CMP r2, #0
	BLT tablero_buscar_color_ERROR
	
	; size_t col = 0
	MOV r4, #0
	
	; r5 = fila * MAX_NO_CERO
	MOV r5, #6	;
	MUL r5, r1, r5

tablero_buscar_color_FOR

	; condición 1: col < MAX_NO_CERO 
	CMP r4, #6			
	BGE loop_exit
	
	; condición 2: ... && (t->columnas[fila][col] != columna)

	; calcular @ de la celda
	ADD r6, r5, r4		; r6 = (fila * 6) + col
	LDRSB r7, [r0, r6]     ; r5 = t->columnas[fila][col]
	
	; t->columnas[fila][col] != columna ? col++ e iterar : salir bucle
    CMP r7, r2
	ADDNE r4, r4, #1	; col++
    BNE tablero_buscar_color_FOR ; iterar
	
loop_exit

	; col == MAX_NO_CERO ? ERROR : seguir bucle
	CMP r4, #6		
	BEQ tablero_buscar_color_ERROR

sigue
	
	; r7 = NUM_FILAS * MAX_NO_CERO
	MOV r7, #42	
	
	; extrar valor de celda
	ADD r7, r6, r7		; r7 + @columna[fila][col]
	LDRSB r7, [r0, r7] 	; @tablero + r7 = t->no_cero[fila][col]		
	
	; check color celda
	AND r7, r7, #0x03	; celda_color(CELDA celda) {return (celda & 0x03);}
	
	CMP r7, r3	; celda_color(t->no_ceros[fila][col]) == color)
	BNE tablero_buscar_color_ERROR

	; exito en tablero_buscar_color
	MOV r0,#0
	B epilogo_tablero_buscar_color

tablero_buscar_color_ERROR
	MOV r0, #-1

epilogo_tablero_buscar_color

	LDMDB FP, {r4-r10,FP,SP,PC}	


epilogo_buscar_alineamiento

	; restaurar marco de pila
	LDMDB FP, {r1-r10,FP,SP,PC}



	END