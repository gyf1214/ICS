####################################################
# Students' Makefile for the CS:APP Architecture Lab
####################################################

# IMPORTANT: Change the USER variable to your unique UserID
USER = changeme

# start gyf1214 settings

YAS = sim/misc/yas
YIS = sim/misc/yis
MISC = sum rsum copy

SEQ = full
SSIM = sim/seq/ssim
SEQYO = asumi asuml

MISCTAR = $(patsubst %, bin/misc/%.yo, $(MISC))
MISCTEST = $(patsubst %, test/misc/%.yo, $(MISC))
SEQTAR = sim/seq/seq-$(SEQ).hcl
SEQTEST = $(patsubst %, test/seq/%.yo, $(SEQYO))

seq : $(SSIM)

clean-seq :
	make -C sim/seq clean

test-seq-small : seq $(SEQTEST)

misc : $(MISCTAR)

test-misc : misc $(MISCTEST)

.PHONY: misc seq

test/seq/% : sim/y86-code/% seq
	$(SSIM) -t $< | grep ISA

$(SSIM) : $(SEQTAR)
	make -C sim/seq VERSION=$(SEQ)

sim/seq/% : src/seq/%
	cp $< $@

test/misc/% : bin/misc/%
	$(YIS) $<

bin/%.yo : src/%.yo
	mv $< $@

src/%.yo : src/%.ys
	$(YAS) $<

# end

sim:
	(cd sim; make)

# Use this rule to build the handin file for Part A ("make handin-parta")
handin-parta:
	cp src/misc/sum.ys $(USER)-sum.ys
	cp src/misc/rsum.ys $(USER)-rsum.ys
	cp src/misc/copy.ys $(USER)-copy.ys
	tar cf - $(USER)-sum.ys $(USER)-rsum.ys $(USER)-copy.ys > $(USER)-parta.tar
	rm -f $(USER)-sum.ys $(USER)-rsum.ys $(USER)-copy.ys

# Use this rule to build the handin file for Part B ("make handin-partb")
handin-partb:
	cp src/seq/seq-full.hcl $(USER)-seq-full.hcl
	tar cf - $(USER)-seq-full.hcl > $(USER)-partb.tar
	rm -f $(USER)-seq-full.hcl

# Use this rule to build the handin file for Part C ("make handin-partc")
handin-partc:
	cp src/pipe/ncopy.ys $(USER)-ncopy.ys
	cp src/pipe/pipe-full.hcl $(USER)-pipe-full.hcl
	tar cf - $(USER)-ncopy.ys $(USER)-pipe-full.hcl > $(USER)-partc.tar
	rm -f $(USER)-ncopy.ys $(USER)-pipe-full.hcl

clean: clean-seq
	rm -f *~ *.o
	rm -f bin/**/*.yo
