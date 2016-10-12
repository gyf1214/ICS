rsp = 0x5561b8e8;
ENTRY(rsp);
SECTIONS {
    output: {
        . = rsp;
        *(.text)
    }
}
