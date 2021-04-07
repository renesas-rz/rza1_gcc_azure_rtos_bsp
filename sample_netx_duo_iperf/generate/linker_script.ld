MEMORY {
    CSx    : ORIGIN = 0x00000000, LENGTH = 0x18000000
    SPIBSC : ORIGIN = 0x18000000, LENGTH = 0x08000000
    RAM    : ORIGIN = 0x20000000, LENGTH = 0x00500000
}
SECTIONS
{
	.fvectors 0x20020000 : AT (0x20020000)
	{
		KEEP(*(.fvectors))
	} > RAM
	.text 0x20020100 : AT (0x20020100)
	{
		*(.text)
	} > RAM
	.rvectors : 
	{
		_rvectors_start = .;
		KEEP(*(.rvectors))
		_rvectors_end = .;
	} > RAM
	.init : 
	{
		*(.init)
	} > RAM
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		PROVIDE_HIDDEN (__exidx_start = .);
	} > RAM
	.ARM.exidx : 
	{
		*(.ARM.exidx .gnu.linkonce.armexidx.*)
		PROVIDE_HIDDEN (__exidx_end = .);
	} > RAM
	
	.fini : 
	{
		*(.fini)
	} > RAM
	.got : 
	{
		*(.got)
		*(.got.plt)
	} > RAM
	.rodata : 
	{
		*(.rodata)
		*(.rodata.*)
		_erodata = .;
	} > RAM
	.eh_frame_hdr : 
	{
		*(.eh_frame_hdr)
	} > RAM
	.eh_frame : 
	{
		*(.eh_frame)
	} > RAM
	.jcr : 
	{
		*(.jcr)
	} > RAM
	.tors : 
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		__ctors = .;
		*(.ctors)
		__ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		*(.dtors)
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
		_mdata = .;
	} > RAM
	.data 0x20060100 : AT (_mdata)
	{
		_data = .;
		*(.data)
		*(.data.*)
		_edata = .;
	} > RAM
	.bss : 
	{
		PROVIDE(__bss_start__ = .);
		_bss = .;
		*(.bss)
		*(.bss.**)
		*(COMMON)
		PROVIDE(__bss_end__ = .);
		_ebss = .;
		_end = .;
		PROVIDE(end = .);
	} > RAM
	.stack 0x20061100 (NOLOAD)  : AT (0x20061100)
	{
		_stack = .;
	} > RAM
}
