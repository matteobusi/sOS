bits 32
org 0xc0004038

SECTION .data                   ; Data section

msg:	db	"Hello, world!\n",0	        ; a C string, "string"

SECTION .text                   ; Code section.

	global	main		        ; "C" main program 
main:				        ; label, start of main program
	 
        push    dword msg		; address of format string
        call    0x100a17			; Call C function
        add     esp, 40			; pop stack 10*4 bytes

        mov     eax, 0			; exit code, 0=normal
       	push eax
	call 0x10199b
        ret
