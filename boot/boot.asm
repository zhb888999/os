global _start

section .text
bits 32
_start:
    jmp header_end
    align 8
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    dd - (0xe85250d6 + (header_end - header_start))
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:
    mov esp, stack_top
    mov [multiboot_info], ebx       ; move Multiboot info pointer to edi

    call setup_page_tables
    call enable_paging

    lgdt [gdt_ptr]

    jmp 0x8:long_mode_start

setup_page_tables:
    mov eax, p3_table
    or eax, 0b11 ; present + writable
    mov [p4_table], eax

    mov eax, 0x0
    or eax, 0x83 ; present + writable
    mov [p3_table], eax

    ret

enable_paging:
    mov eax, p4_table
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

bits 64
long_mode_start:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rax, 0x2f592f412f4b2f41
    mov qword [0xb8040], rax
    hlt

section .bss
multiboot_info:
    dd 0
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096

stack_bottom:
    resb 4096 * 4
stack_top:

section .rodata
gdt64:
    dq 0 ; zero entry
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
gdt_ptr:
    dw 0x10 - 1
    dd gdt64
