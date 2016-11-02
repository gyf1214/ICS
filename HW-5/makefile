lf : sim/pipe/pipe-lf.hcl
	make -C sim/pipe clean
	make -C sim/pipe psim VERSION=lf

nt : sim/pipe/pipe-nt.hcl
	make -C sim/pipe clean
	make -C sim/pipe psim VERSION=nt

test-lf : lf
	(cd sim/ptest; make SIM=../pipe/psim | grep Check)
	make -C sim/y86-code testpsim | grep Check

test-nt : nt
	(cd sim/ptest; make SIM=../pipe/psim | grep Check)
	make -C sim/y86-code testpsim | grep Check

sim/pipe/%.hcl : %.hcl
	cp $< $@
