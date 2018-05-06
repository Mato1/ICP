all: clean src/Makefile

src/Makefile:
	qmake src/icp.pro -o src/Makefile
	$(MAKE) -C src/ -s
	cp src/blockbuilder.app/Contents/MacOS/blockbuilder .

.PHONY: clean
clean:
	rm -rf blockbuilder src/blockbuilder.app/Contents/MacOS/blockbuilder  src/Makefile src/*.o src/moc_*