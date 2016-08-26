# With thanks to this great tutorial: http://wiki.osdev.org/Makefile
# Also good: https://www.youtube.com/watch?v=Q1Lnp_Xx7z4

SRCDIRS := $(shell find src -type d)
SRCSRCS := $(shell find src -type f -name "*.cpp")
SRCOBJS := $(SRCSRCS:%.cpp=tmp/%.o)
SRCDEPS := $(SRCSRCS:%.cpp=tmp/%.d)

TESTDIRS := $(shell find test -type d)
TESTSRCS := $(shell find test -type f -name "*.cpp")
TESTOBJS := $(TESTSRCS:%.cpp=tmp/%.o)
TESTDEPS := $(TESTSRCS:%.cpp=tmp/%.d)

TMPDIRS := $(TESTDIRS:%=tmp/%) $(SRCDIRS:%=tmp/%)

.PHONY: all dist test clean

all: dist test
#	@for file in $(SRCFILES); do echo $$file; done
#	@for file in $(OBJECTS); do echo $$file; done
#	@for file in $(TMPDIRS); do echo $$file; done

-include $(SRCDEPS) $(TESTDEPS)

dist: $(TMPDIRS) $(SRCOBJS)

test: $(TMPDIRS) $(TESTOBJS) tmp/tests
	@tmp/tests

tmp/tests: 
	@g++ $(TESTOBJS) -o tmp/tests

# Makes all src tmp objects
tmp/src/%.o: src/%.cpp Makefile
	# Compiling SRC file $<
	@g++ -MMD -MP -c $< -o $@ -I src

# Makes all test tmp objects
tmp/test/%.o: test/%.cpp Makefile
	# Compiling TEST file $<
	@#@g++ $(CFLAGS) -MMD -MP -c $< -o $@
	@g++ -MMD -MP -c $< -o $@ -I include -I src

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
