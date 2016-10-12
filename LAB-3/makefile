HEX= ./hex2raw
CTAR= ./ctarget
RTAR= ./rtarget
AS= as
LD= ld
LDFLAGS= -e rsp --oformat binary -Ttext 0x5561b8e8
TARS= 1 2 3
OBJS= $(patsubst %, bin/%, $(TARS))
TESTS= $(patsubst %, test%, $(TARS))

forse_rebuild: clean main

clean:
	rm -f bin/* obj/*

main: $(OBJS)

test%: bin/%
	$(TAR) < $<

bin/%: obj/%.o
	$(LD) $(LDFLAGS) -o $@ $<

bin/%: src/%.hex
	$(HEX) < $< > $@

obj/%.o: src/%.s
	$(AS) -o $@ $<
