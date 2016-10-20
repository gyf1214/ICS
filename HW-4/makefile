YAS = sim/misc/yas
YIS = sim/misc/yis
VERSION = full

empty :

all : 4.47.yo 4.49.yo sim/seq/ssim

test-all : test/4.47 test/4.49 test/ssim

test/ssim : sim sim/seq/ssim
	! ($< -t sim/y86-code/asumi.yo | grep Fails)
	! (make -C sim/y86-code testssim | grep Fails)
	! (cd sim/ptest; make SIM=../seq/ssim TFLAGS=-i | grep Fails)

sim/seq/ssim : sim sim/seq/seq-$(VERSION).hcl
	make -C sim/seq VERSION=$(VERSION) GUIMODE=

sim/seq/% : %
	cp $< $@

test/% : %.yo sim
	$(YIS) $<

%.yo : %.ys sim
	$(YAS) $<

sim :
	make -C sim/misc
	make -C sim/y86-code

clean :
	rm -f *.o *.yo

.PHONY : clean sim all test-all
