HEX= ./hex2raw
CTAR= ./ctarget
RTAR= ./rtarget
TAR:=
AS= as
LD= ld
LDFLAGS= -e rsp --oformat binary -Ttext 0x5561b8e8
CBINS= 1 2 3
RBINS= 4 5
OBJS= $(patsubst %, bin/%, $(CBINS) $(RBINS))
CTESTS= $(patsubst %, test%, $(CBINS))
RTESTS= $(patsubst %, test%, $(RBINS))

forse_rebuild: clean main

clean:
	rm -f bin/* obj/*

main: $(OBJS)

test1: TAR += $(CTAR)

$(CTESTS): test%: bin/%
	$(CTAR) < $<

$(RTESTS): test%: bin/%
	$(RTAR) < $<

bin/%: obj/%.o
	$(LD) $(LDFLAGS) -o $@ $<

bin/%: src/%.hex
	$(HEX) < $< > $@

obj/%.o: src/%.s
	$(AS) -o $@ $<
