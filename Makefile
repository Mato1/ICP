all: clean src/Makefile

run: all ./blockeditor

src/Makefile:
	qmake src/icp.pro -o src/Makefile
	$(MAKE) -C src/ -s
	cp src/blockeditor .

.PHONY: clean
clean:
	rm -rf blockeditor src/blockeditor  src/Makefile src/*.o src/moc_*