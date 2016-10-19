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
PSIM = sim/pipe/psim
PIPE = full
NCOPY = sim/pipe/ncopy.ys

MISCTAR = $(patsubst %, bin/misc/%.yo, $(MISC))
MISCTEST = $(patsubst %, test/misc/%.yo, $(MISC))
SEQTAR = sim/seq/seq-$(SEQ).hcl
PIPETAR = sim/pipe/pipe-$(PIPE).hcl
SEQTEST = $(patsubst %, test/seq/%.yo, $(SEQYO))

empty :

pipe : $(PSIM)

clean-pipe :
	make -C sim/pipe clean

test-pipe-psim : pipe
	make -C sim/ptest SIM=../pipe/psim TFLAGS=-i
	make -C sim/y86-code testpsim

test-pipe-correctness : pipe
	(cd sim/pipe; ./correctness.pl -p)
	make -C sim/pipe ncopy.yo
	sim/pipe/check-len.pl < sim/pipe/ncopy.yo

test-pipe-bench : pipe
	(cd sim/pipe; ./benchmark.pl)

test-pipe : pipe test-pipe-correctness test-pipe-psim test-pipe-bench

seq : $(SSIM)

clean-seq :
	make -C sim/seq clean

test-seq-bench : seq
	make -C sim/y86-code testssim

test-seq-small : seq $(SEQTEST)

test-seq : seq test-seq-small test-pipe

misc : $(MISCTAR)

test-misc : misc $(MISCTEST)

.PHONY : misc test-misc seq test-seq-small test-seq-bench pipe empty

$(PSIM) : $(PIPETAR) $(NCOPY)
	make -C sim/pipe VERSION=$(PIPE)

test/seq/% : sim/y86-code/% seq
	$(SSIM) -t $< | grep ISA

$(SSIM) : $(SEQTAR)
	make -C sim/seq VERSION=$(SEQ)

sim/% : src/%
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

clean: clean-seq clean-pipe
	rm -f *~ *.o
	rm -f bin/**/*.yo
