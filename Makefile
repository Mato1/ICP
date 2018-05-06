all: clean src/Makefile

src/Makefile:
	qmake src/icp.pro -o src/Makefile
	$(MAKE) -C src/ -s
	cp src/blockbuilder .

.PHONY: clean
clean:
	rm -rf blockbuilder src/blockbuilder src/Makefile src/*.o src/moc_*