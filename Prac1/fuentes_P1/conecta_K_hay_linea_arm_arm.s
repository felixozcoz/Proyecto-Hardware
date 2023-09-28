EXPORT conecta_K_hay_linea_arm_arm

conecta_K_buscar_alineamiento_arm:
// PROLOGO
	MOV IP, SP
	STMDB SP!, {r4-r10,FP,IP,LR,PC}
	SUB FP, IP, #4
	SUB SP, #SpaceForLocalVaribles // No vamos a guardar variables locales en pila
//-----------------------
	
	MOV r5, r0		// Quitamos el primer parametro de r0 ya que ahi ira el resultado
	
	// Deltas fila
	MOV r6,#0
	STR r6,[sp,#-4]!			// sp +28
	MOV r6,#-1
	STR r6,[sp,#-4]!			// sp +24
	MOV r6,#-1
	STR r6,[sp,#-4]!			// sp + 20
	MOV r6,#1
	STR r6,[sp,#-4]!			// sp +16
	
	// Deltas columna
	MOV r6,#-1
	STR r6,[sp,#-4]!			// sp+12
	MOV r6,#0
	STR r6,[sp,#-4]!			// sp+8
	MOV r6,#-1
	STR r6,[sp,#-4]!			// sp+4
	MOV r6,#-1
	STR r6,[sp,#-4]!			// sp

	MOV r6,#0				// i = 0
	MOV r7,#0				// linea = FALSE
	MOV r8,#0				// long_linea = 0

	

	






Epilogo:
	// Epilogo
	LDMIA FP, {r4-r10,FP,SP,PC}
	//----------------
