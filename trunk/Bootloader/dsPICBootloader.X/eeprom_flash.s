;Calling ASM code from C convention :
;See TABLE 4-4 summary, p. 74 of MPLAB C30 User's guide for register summary

;PARAMETERS
;Registers W0-W15 are used for parameter passing.
;W0-W7 are scratch registers (not saved)
;W8-W15 are preserved across ordinary function calls
;For interrupt functions W0-W15 and RCOUNT are preserved

;RETURN VALUES
;return in W0 for 8 or 16 bits scalars
;return in W1:W0 for 32 bits scalars
;return in W3:W2:W1:W0 for 64 bits scalars


; EACH FLASH ROW IS 96 bytes = 32 * 24 bits instructions
; THEY MUST BE WRITTEN IN A SINGLE SHOT, FILLING LATCHES BEFORE WRITING
; 

.include "p30f5015.inc"

.equ FLASH_MEM_PAGE, 0x00
.equ EEPROM_MEM_PAGE, 0x7F
.equ CONFIG_MEM_PAGE, 0xF8


.global _ReadMem
.global _WriteMem


;w0 = page w1 = offset
;return long = W1:W0 (data)
_ReadMem:
	push TBLPAG             ; Save TBLPAG
        mov W0, TBLPAG          ; W0 = high address word
        			; W1 = low address word
	mov W1, W2		; W2 = W1
        tblrdh [W2], W1         ; Read high word to W1
	tblrdl [W2], W0         ; Read low word to W0
	pop TBLPAG		; Restore TBLPAG
	return

;w0 = page
;w1 = offset
;w2 = ptr to data
;w3 = size of data
;return unsigned int = W0 (write size)
_WriteMem:
    push TBLPAG                 ; Save TBLPAG
    mov W0, TBLPAG              ; W0 = high address word, latch loading
    mov W0, NVMADRU 		; Set high address
    mov W1, NVMADRL 		; Set low address
    mov #0, W4                  ; W4 = 0 -->Bytes written counter

    mov  #FLASH_MEM_PAGE, W0 	; Check if destination is FLASH memory
    cp.b TBLPAG
    btsc SR, #Z
    goto DestFLASH

    mov  #EEPROM_MEM_PAGE, W0 	; Check if destination is EEPROM memory
    cp.b TBLPAG
    btsc SR, #Z
    goto DestEEPROM

    mov  #CONFIG_MEM_PAGE, W0 	; Check if destination is Config Memory
    cp.b TBLPAG
    btsc SR, #Z
    goto DestCONFIG

    goto TerminateWrite		; Default -- Nothing to do

DestCONFIG:
    ;Not allowing user to do that right now...
    goto TerminateWrite

DestEEPROM:

EEPROMWord:
		mov #1, W0		; Do we have a single word to write?
		cp.b W0,W3
		btss SR, #Z
		goto EEPROMRow		; Test for a row to write
		
		mov #0x4044,W0		; Erase EEPROM word code
		rcall WriteKey
		tblwtl [w2],[w1]	; Write data to latch
		mov #0x4004,W0		; Write EEPROM word code
		rcall WriteKey
		inc W4, W4		; Wrote 1 word
		goto TerminateWrite
EEPROMRow:
		mov #16, W0		; Do we have a complete row to write (16 words) ?
		cp.b W0, W3
		btss SR, #Z
		goto TerminateWrite


		mov #0x4045, W0		; Erase EEPROM Row code
		rcall WriteKey

					; Copy data to latches
LoadEELatch:
		tblwtl [w2++], [w1++]		
		inc W4, W4		; One more word written
		dec W3, W3					
		bra NZ, LoadEELatch	; Repeat until whole row is loaded
		mov #0x4005, W0		; Write EEPROM Row code
		rcall WriteKey		
		goto TerminateWrite	; Terminate write

DestFLASH:
		mov #64, W0		; Do we have a complete row to write (32 * 24 bits instructions (32 bits used))  = 64 words) ?
		cp.b W0, W3
		btss SR, #Z
		goto TerminateWrite
		mov #0x4041, W0		; Erase FLASH Row code
		rcall WriteKey																		

					; Copying data to latches
LoadFlashLatch:
		tblwtl	[w2++],[w1]     ; load low word latch
		tblwth	[w2++],[w1++]   ; load high word latch
		inc2 W4, W4		; Two more words written
                dec2 W3, W3
                bra NZ, LoadFlashLatch	; Repeat until whole row is loaded
		mov #0x4001, W0		; Write Flash Row code
		rcall WriteKey
		goto TerminateWrite	; Terminate write


TerminateWrite:
		mov W4, W0				  	; Return number of bytes written
		pop TBLPAG				  	; Restore TBLPAG
		return

;W0 is the configuration 
WriteKey:
    PUSH SR
    PUSH W0
    MOV #0x00E0,W0
    IOR SR              ; Disable interrupts
    POP W0
    mov W0, NVMCON
    mov #0x55, W0
    mov W0, NVMKEY
    mov #0xAA, W0
    mov W0, NVMKEY
    bset NVMCON, #WR    ; Start Writing
    nop
    nop

WaitWriting:
        btsc NVMCON, #WR          	; WR or WREN - Wait until operation is finished
        bra  WaitWriting	
	pop  SR						; re-enable interrupts
        return

; End of program code in this file
.end
