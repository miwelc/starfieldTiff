OUTPUT_FORMAT("elf32-littlemips")
OUTPUT_ARCH(mips)

ENTRY(_start)

SECTIONS
{
  . = 0x09e00000;
  .text.start : {
    *(.text.start)
  }
  .text : {
    *(.text)
  }
  .bss : {
    *(.bss)
  }
  .rodata : {
    *(.rodata)
  }
  .data : {
    *(.data)
  }
}
