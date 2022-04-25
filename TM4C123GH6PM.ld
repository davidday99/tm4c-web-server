/* Linker Script for the TM4C123 MCU. */

MEMORY 
{
    ROM (rx)    : ORIGIN = 0x00000000, LENGTH = 256K 
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 32K 
}

STACK_SIZE = 0x2000;

SECTIONS
{
    PROVIDE(_stack_ptr = ORIGIN(RAM) + LENGTH(RAM));

    .text :
    {
        _text = .;
        KEEP(*(.vector_table))
        *(.text*)
        *(.rodata*) 
        _etext = .;
    } > ROM
    
    .data :
    {
        . = ALIGN(4);
        _data = .;
        *(.data*);
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > ROM

    .bss (NOLOAD) :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM
    
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
    }
}

