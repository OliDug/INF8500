	addi r1, r0, $70	# F
	addi r2, r0, $105	# i
	addi r3, r0, $110	# n
	addi r4, r0, $10	# \n
	swi r1, r0, $4095	# écriture vers la console 'F'
	swi r2, r0, $4095	# écriture vers la console 'i'
	swi r3, r0, $4095	# écriture vers la console 'n'
	swi r4, r0, $4095	# écriture vers la console '\n'
	nop					# Latence des étages
	nop					# Latence des étages
	nop					# Latence des étages
	nop					# Latence des étages
	ret					# Fin du programme