EXPORT conecta_K_buscar_alineamiento_arm
	
conecta_K_buscar_alineamiento_arm:
// PROLOGO
	MOV IP, SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	//SUB SP, #SpaceForLocalVaribles // No vamos a guardar variables locales en pila
//-----------------------

	// Cargar parametros a otros registros que no sean los de pasar parametros,
	// por lo menos tablero de r0 a r5 ya que r5 se sobreescribe
	MOV r5, r0

	// Deltas en r6 y r7
	LDR r6, [fp, #4]
	LDR r7, [fp, #8]

	// Los parametros nos llegan perfectos para pasarlos directos a tablero_buscar_color
	// ya estan en los registros correspondientes
	
	BL tablero_buscar_color
	CMP r0,EXITO    	// Comparamos el resultado de tablero_buscar_color en r0 con EXITO
	BEQ Exito			// Si exito continuamos
	LDR r0, #0      		// Si no es exito cargamos 0 en r0 para devolverlo
	B Epilogo       		// Saltamos al final de la funcion

Exito:
	ADD r2, r2, r6  		// nueva_fila = fila + delta_fila;
	ADD r3, r3, r7  		// nueva_columna = columna + delta_columna;

	// Cargar los delta en la pila
	STR r6,[sp,#-4]!		// Apilamos delta_fila
	STR r7,[sp,#-4]! 	// Apilamos delta_columna

	MOV r0, r5 			// Devolvemos tablero t a r0 para pasarlo como primer parametro

	BL busacarAlineamiento
	
	// La subrutina deveria guardar el valor devuelto en r0
	
	ADD r0,r0,#1

Epilogo:
	// Epilogo
	LDMIA FP, {r4-r10,FP,SP,PC}
	//----------------







tablero_buscar_color:
// PROLOGO
	MOV IP, SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	SUB SP, #SpaceForLocalVaribles 		// No vamos a guardar variables locales en pila
//-----------------------
	
	CMP r1, #7										// tablero_fila_valida con NUM_FILAS hardcodeado 
	BGE tablero_buscar_color_ERROR		// Si es mayor o igiual devolvemos ERROR
	CMP r3, #7										// tablero_columna_valida con NUM_COLUMNAS hardcodeado 
	BGE tablero_buscar_color_ERROR		// Si es mayor o igiual devolvemos ERROR
	
	
	MOV r4, #0;
tablero_buscar_color_FOR:
	// (col < MAX_NO_CERO) 
	CMP r4, #6			// Comparamos con MAX_NO_CERO hardcodeado
	BGE tablero_buscar_color_FORexit
	// && (t->columnas[fila][col] != INVALIDO)
	MUL r5,r1,#7			// Cuantas filas hay que desplazarse
	ADD r5,r5,r3			// Desplazamiento de filas + columna
	ADD r5,r0,r5			// t + desplazamiento fila,columna
	LDR r6, [r5]   		// Load t->columnas[fila][col]
    CMP r6, #255
    BEQ loop_exit
	
	ADD r4,r4,#1			
	B tablero_buscar_color_FOR
	
	
	
	CMP r4, #6			// Comparamos con MAX_NO_CERO hardcodeado
	BEQ tablero_buscar_color_ERROR		// Si col == MAX_NO_CERO devolvemos ERROR
 
	

tablero_buscar_color_EXITO:
	MOV r0,#0
	B tablero_buscar_color_fin

tablero_buscar_color_ERROR:
	MOV r0, #-1

tablero_buscar_color_fin:
// Epilogo
LDMIA FP, {r4-r10,FP,SP,PC}
//----------------

