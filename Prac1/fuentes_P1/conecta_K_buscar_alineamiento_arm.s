	AREA codigo,CODE
		
	EXPORT conecta_K_buscar_alineamiento_arm
		
conecta_K_buscar_alineamiento_arm
	; PROLOGO
	MOV IP, SP
	STMDB SP!, {r1-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	;SUB SP, #SpaceForLocalVaribles ; No vamos a guardar variables locales en pila
	;-----------------------

	; Cargar parametros a otros registros que no sean los de pasar parametros,
	; por lo menos tablero de r0 a r5 ya que r5 se sobreescribe
	MOV r5, r0

	; Deltas en r6 y r7
	LDR r6, [fp, #4]	; fila
	LDR r7, [fp, #8]	; columna
	
	; Los parametros nos llegan perfectos para pasarlos directos a tablero_buscar_color
	; ya estan en los registros correspondientes
	
	BL tablero_buscar_color
	CMP r0,#0    	; Comparamos el resultado de tablero_buscar_color en r0 con EXITO
	BEQ Exito			; Si exito continuamos
	MOV r0, #0      		; Si no es exito cargamos 0 en r0 para devolverlo
	B Epilogo       		; Saltamos al final de la funcion

Exito
	ADD r1, r1, r6  		; nueva_fila = fila + delta_fila;
	ADD r2, r2, r7  		; nueva_columna = columna + delta_columna;

	; Cargar los delta en la pila
	STR r7,[sp,#-4]! 	; Apilamos delta_columna
	STR r6,[sp,#-4]!		; Apilamos delta_fila /// NOTA

	MOV r0, r5 			; Devolvemos tablero t a r0 para pasarlo como primer parametro

	BL conecta_K_buscar_alineamiento_arm
	
	; La subrutina deveria guardar el valor devuelto en r0
	
	ADD r0,r0,#1

Epilogo
	; Epilogo
	LDMDB FP, {r1-r10,FP,SP,PC}	; Original: LDMDB FP, {r4-r10,FP,SP,PC}
	;----------------

tablero_buscar_color
; PROLOGO
	MOV IP, SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	;SUB SP, #SpaceForLocalVaribles 		; No vamos a guardar variables locales en pila
;-----------------------
	
	CMP r1, #7										; tablero_fila_valida con NUM_FILAS hardcodeado 
	BGE tablero_buscar_color_ERROR		; Si es mayor o igiual devolvemos ERROR
	CMP r2, #7										; tablero_columna_valida con NUM_COLUMNAS hardcodeado 
	BGE tablero_buscar_color_ERROR		; Si es mayor o igiual devolvemos ERROR
	
	; col = 0
	MOV r4, #0;			
	
tablero_buscar_color_FOR
	; (col < MAX_NO_CERO) 
	CMP r4, #6			; Comparamos con MAX_NO_CERO hardcodeado
	BGE loop_exit
	; && (t->columnas[fila][col] != columna)
	MOV r6, #6
	MUL r5,r1,r6			; Cuantas filas hay que desplazarse		r5 = fila * 6
	ADD r5,r5,r4			; Desplazamiento de filas + col      	r5 = (fila * 6) + col
	ADD r5,r0,r5			; &t + desplazamiento fila,col			r5 = t + (fila * 6 + col)    (== t.columnas[fila][col])
	LDRSB r6, [r5]   	; r6 = t->columnas[fila][col]
    CMP r6, r2
	ADDEQ r4,r4,#1	; col++
    BEQ tablero_buscar_color_FOR
	
loop_exit

	;	if(col == MAX_NO_CERO) {
	;		return ERROR;
	;	 }
	CMP r4, #6										; Comparamos con MAX_NO_CERO hardcodeado
	BEQ tablero_buscar_color_ERROR		; Si col == MAX_NO_CERO devolvemos ERROR
	
	; t->no_ceros[fila][col];
	MOV r7,#6
	MOV r8,#7
	MUL r5, r7, r8		; Cuanto ocupa la parte comunas de tablero MAX_NO_CERO * 7
	
	MUL r6, r1, r7		; Desplazamiento de fila en no_ceros
	ADD r6, r6, r4		; Desplazamiento de fila + col en no_ceros
	ADD r6, r5, r6 		; desplazamiento a no_ceros + desplazamiento fila y columna
	ADD r6, r0, r6 		; &t + desplazamiento a no_ceros + desplazamiento fila y columna
	LDRB r6, [r6]   	; r6 = t->no_ceros[fila][col]
	AND r6, r6, #0x03		; celda_color(CELDA celda) {return (celda & 0x03);}
	CMP r6, r3	; celda_color(t->no_ceros[fila][col]) == color)
	BNE tablero_buscar_color_ERROR

tablero_buscar_color_EXITO
	MOV r0,#0
	B tablero_buscar_color_fin

tablero_buscar_color_ERROR
	MOV r0, #-1

tablero_buscar_color_fin
	; Epilogo
	LDMDB FP, {r4-r10,FP,SP,PC}
	;----------------

	END