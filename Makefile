binaries = build/*/julipp/*.o

julipp:
	@ $(MAKE) -f c.make NAME=julipp MODE=debug SOURCE_DIR=.
	cp build/julipp.exe julipp.exe

julipp_alt:
	gcc julipp.cpp -o julipp.exe

clean:
	rm -f $(binaries)
