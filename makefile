CXXFLAGS := -std=c++17
CPPFLAGS := -Iinclude

ouster-laz: ouster-laz.cc

.PHONY: clean
clean:
	rm -f ouster-laz

include makefile.compile_commands
