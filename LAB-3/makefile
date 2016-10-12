HEX= ./hex2raw
CTAR= ./ctarget
AS= as
LD= ld
LDFLAGS= -e rsp --oformat binary -Ttext 0x5561b8e8

bin/3: obj/3.o
	$(LD) $(LDFLAGS) -o $@ $<

obj/3.o: src/3.s
	$(AS) -o $@ $<

test2: bin/2
	$(CTAR) < $<

bin/2: obj/2.o
	$(LD) $(LDFLAGS) -o $@ $<

obj/2.o: src/2.s
	$(AS) -o $@ $<

test1: bin/1
	$(CTAR) < $<

bin/1: src/1.hex
	$(HEX) < $< > $@
