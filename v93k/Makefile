# With thanks to this great tutorial: http://wiki.osdev.org/Makefile
# Also good: https://www.youtube.com/watch?v=Q1Lnp_Xx7z4

# All sources
SRCDIRS := $(shell find src -type d)
SRCSRCS := $(shell find src -type f -name "*.cpp")
SRCOBJS := $(SRCSRCS:%.cpp=tmp/%.o)
SRCDEPS := $(SRCSRCS:%.cpp=tmp/%.d)

# Sources for building liborigen_v93k.so
ORIGENSRCS = $(shell find src/liborigen_v93k -type f -name "*.cpp")
ORIGENOBJS = $(ORIGENSRCS:%.cpp=tmp/%.o)

# Sources for building libtml_origen.so


# All tests
TESTDIRS := $(shell find test -type d)
TESTSRCS := $(shell find test -type f -name "*.cpp")
TESTOBJS := $(TESTSRCS:%.cpp=tmp/%.o)
TESTDEPS := $(TESTSRCS:%.cpp=tmp/%.d)

TMPDIRS := $(TESTDIRS:%=tmp/%) $(SRCDIRS:%=tmp/%)


.PHONY: all dist test clean debug

all: dist test
#	@for file in $(SRCFILES); do echo $$file; done
#	@for file in $(OBJECTS); do echo $$file; done
#	@for file in $(TMPDIRS); do echo $$file; done

-include $(SRCDEPS) $(TESTDEPS)

dist: $(TMPDIRS) $(SRCOBJS) binaries/liborigen_v93k.so

test: tmp/tests
	@LD_LIBRARY_PATH=binaries tmp/tests

debug: tmp/tests
	@LD_LIBRARY_PATH=binaries gdb tmp/tests

binaries:
	@mkdir binaries

binaries/liborigen_v93k.so: binaries $(ORIGENOBJS)
	@g++ -shared $(ORIGENOBJS) -o binaries/liborigen_v93k.so

tmp/tests: $(TMPDIRS) $(TESTOBJS)
	@g++ $(TESTOBJS) -o tmp/tests -L binaries -l origen_v93k

# Makes all src tmp objects
tmp/src/%.o: src/%.cpp Makefile
	# Compiling SRC file $<
	@# -fPIC here is necessary since these will end up in a .so
	@# -g means include symbols for debugger
	@g++ -MMD -MP -c -g -fPIC $< -o $@ -I src/liborigen_v93k -I src/libtml_origen

# Makes all test tmp objects
tmp/test/%.o: test/%.cpp Makefile
	# Compiling TEST file $<
	@g++ -MMD -MP -c -g $< -o $@ -I include -I src/liborigen_v93k -I src/libtml_origen -I test

# The dependency files are created as an offshoot of the object files,
# this is a dummy rule to stop them being picked up by the tmp directory rule
tmp/%.d:
	@# $@

# Makes all tmp directories
tmp/%:
	@mkdir -p $@

clean:
	@rm -fr tmp
	@rm -fr binaries
