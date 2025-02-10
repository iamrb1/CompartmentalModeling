build: clean
	$(MAKE) -C lib -f Makefile build
	$(MAKE) -C bin -f Makefile build

debug: clean
	$(MAKE) -C lib -f Makefile debug
	$(MAKE) -C bin -f Makefile debug

clean:
	$(MAKE) -C lib -f Makefile clean
	$(MAKE) -C bin -f Makefile clean

run: build
	LD_LIBRARY_PATH=lib/target/release bin/target/release/app

rundebug: debug
	LD_LIBRARY_PATH=lib/target/debug bin/target/debug/app
