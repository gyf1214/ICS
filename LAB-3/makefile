HEX= ./hex2raw
CTAR= ./ctarget
AS= as
LD= ld
LDFLAGS= -e rsp --oformat binary -Ttext

bin/2: src/2.o
	$(LD) $(LDFLAGS) 0x5561b8e8 -o $@ $<

obj/2.o: src/2.s
	$(AS) -o $@ $<

test1: bin/1
	$(CTAR) < $<

bin/1: src/1.hex
	$(HEX) < $< > $@
