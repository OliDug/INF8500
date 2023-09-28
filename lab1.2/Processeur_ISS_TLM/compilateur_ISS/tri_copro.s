@firstSubstring:
					lwi r10, r1, $0		# r10 = nombre éléments
					swi r10, r6, $8192 	# envoie nombre éléments à CoPro
					addi r1, r2, $4 	# r1 = constante 4
					addi r2, r3, $0 	# r2 = compteur
					addi r4, r3, $2		# r4 = 2 état fin, tri par CoPro
@loadFirst:	
					lw r3, r2, r1		# lecture dans la mem. de données	
					swi r3, r6, $8193   # écriture au CoPro
					add r2, r2, r1		# compteur + 4
					subi r10, r10, $1	# nombre éléments - 1
					nop
					bnei r10, @loadFirst		# branchement
@waiting:
					lwi r7, r6, $8194 	# chargement état CoPro
					nop
					nop
					cmp r8, r7, r4		# comp r7 à 2		
					bnei r8, @waiting	# attendre fin tri	

					lwi r0, r6, $8195	# chargement nombre éléments du CoPro.
					swi r0, r6, $4095

@display:
					lwi r1, r6, $8196	# chargement valeur du CoPro.
					swi r1, r6, $4095 	# envoie valeur à la Console 
					subi r0, r0, $1
					nop
					bnei r0, @display	
					ret
