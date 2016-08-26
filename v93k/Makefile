SRCDIRS := $(shell find src -type d)
SRCSRCS := $(shell find src -type f -name "*.cpp")
SRCOBJS := $(SRCSRCS:%.cpp=tmp/%.o)

TESTDIRS := $(shell find test -type d)
TESTSRCS := $(shell find test -type f -name "*.cpp")
#TESTHDRS := $(shell find test -type f -name "*.hpp")
TESTOBJS := $(TESTSRCS:%.cpp=tmp/%.o)
#TESTOBJS := $(patsubst %.cpp,%.o,$(TESTSRCS))

TMPDIRS := $(TESTDIRS:%=tmp/%) $(SRCDIRS:%=tmp/%)


#TESTDIRS=test
#SRCDIRS=origen/utils origen/tml_Origen
#OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))

.PHONY: all dist test clean

all: dist test
#	@for file in $(SRCFILES); do echo $$file; done
#	@for file in $(OBJECTS); do echo $$file; done
#	@for file in $(TMPDIRS); do echo $$file; done


dist: $(TMPDIRS) $(SRCOBJS)

test: $(TMPDIRS) $(TESTOBJS) tmp/tests
	@tmp/tests

tmp/tests: 
	@g++ $(TESTOBJS) -o tmp/tests

# Makes all src tmp objects
tmp/src/%.o: src/%.cpp Makefile
	# Compiling SRC file $<
	@g++ -c $< -o $@ -I src

# Makes all test tmp objects
tmp/test/%.o: test/%.cpp Makefile
	# Compiling TEST file $<
	@#@g++ $(CFLAGS) -MMD -MP -c $< -o $@
	@g++ -c $< -o $@ -I include

# Makes all tmp directories
tmp/%:
	@mkdir -p $@

clean:
	@rm -fr tmp
	@rm -fr binaries
