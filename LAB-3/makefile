HEX= ./hex2raw
CTAR= ./ctarget

test1: bin/1
	$(CTAR) < $<

bin/1: src/1.hex
	$(HEX) < $< > $@
