HEX= ./hex2raw
CTAR= ./ctarget
AS= as

obj/2.o: src/2.s
	$(AS) $< -o $@

test1: bin/1
	$(CTAR) < $<

bin/1: src/1.hex
	$(HEX) < $< > $@
