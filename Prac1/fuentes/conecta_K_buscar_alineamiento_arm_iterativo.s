; conecta_K_buscar_alineamiento_arm_iterativo.s
	
	AREA codigo,CODE
		
	EXPORT conecta_K_buscar_alineamiento_arm_iter

; Funcionamiento:
; Esta función es encargada de devolver la longitud de 
; la linea más larga entre las 4 posibles direcciones 
; en total.
; Dada la posición de una ficha, devuelve la suma de
; en un sentido y otro del número de fichas consecutivas 
; del mismo color
; El valor que devuelve esta función se utiliza en 
; conceta_K_hay_linea para verificar si hay una línea de K 
; fichas del mismo color

; Se ha decido eliminar la recursividad como 
; parte de la optimización.

; Parámetros:
; 	- Cuadrícula
; 	- Fila
; 	- Columna
;	- delta_fila (parámetro en pila)
;	- delta_columna (parámetro en pila)



conecta_K_buscar_alineamiento_arm_iter
	; PROLOGO
	MOV IP, SP
	STMDB SP!, {r1-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	;-----------------------

	; resultado
	MOV r9, #0	; longitud de cadena
	
inicio

	B tablero_buscar_color
	; resultado devuelto en r8
		
sigue_buscar_alineamiento
	
	; check resultado de tablero_buscar_color()
	CMP r8, #-1
	BEQ FIN_buscar_alineamiento
	
	; Desapilar Parámetros Deltas
	LDR r4, [fp, #4]	; delta_fila
	LDR r5, [fp, #8]	; delta_columna
	
	; Calcular nueva fila y nueva columna 
	ADD r1, r1, r4  	; fila + delta_fila
	ADD r2, r2, r5  	; columna + delta_columna

	ADD r9, r9, #1	; return 1 + ...
	
	B	inicio	; otra "llamada"
	
FIN_buscar_alineamiento

	MOV r0, r9	; guardara resultado en parámetro de devolución
	
	; restaurar marco de pila
	LDMDB FP, {r1-r10,FP,SP,PC}
	; FIN de buscar_alineamiento()



; // -----------------------------------------

; Etiqueta que referencia el comienzo de la 
; función tablero_buscar_color()
; Devuelve en r8 el resultado
; devuelve ERROR = -1 ó EXITO = 0
tablero_buscar_color

	MOV r8, #-1		; r8 = ERROR
	
	; fila < NUM_FILAS
	CMP r1, #7			
	BGE FIN_tablero_buscar_color	; termina función tablero_buscar_color()
	CMP r1, #0
	BLT FIN_tablero_buscar_color	; "   "    "
	
	; columna < NUM_COLUMNAS
	CMP r2, #7										
	BGE FIN_tablero_buscar_color
	CMP r2, #0
	BLT FIN_tablero_buscar_color
	
	; size_t col = 0
	MOV r4, #0
	
	; fila * MAX_NO_CERO
	MOV r5, #6	
	MUL r5, r1, r5	

FOR_tablero_buscar_color

	; col < MAX_NO_CERO 
	CMP r4, #6			
	BGE FOR_EXIT_tablero_buscar_color
	
	; calcular @ de t->columnas[fila][col]
	ADD r6, r5, r4		; (fila * 6) + col
	LDRSB r7, [r0, r6]  ; t->columnas[fila][col]
	
	; t->columnas[fila][col] != columna
    CMP r7, r2
	ADDNE r4, r4, #1				; col++
    BNE FOR_tablero_buscar_color 	; iterar
	
	
FOR_EXIT_tablero_buscar_color

	; col == MAX_NO_CERO
	CMP r4, #6	
	BEQ FIN_tablero_buscar_color	; fin tablero_buscar_color()

	; sigue tablero_buscar_color()
	
	; (fila * 6 + col) + (MAX_NO_CERO * NUM_LINEAS)
	ADD r5, r6, #42
	; @tablero + desplazamiento = t->no_cero[fila][col]
	LDRSB r5, [r0, r5] 			
	
	; check color celda
	AND r5, r5, #0x03
	
	; celda_color(t->no_ceros[fila][col]) == color)
	CMP r5, r3
	MOVEQ r8, #0	; return EXITO

FIN_tablero_buscar_color

	B sigue_buscar_alineamiento


	END