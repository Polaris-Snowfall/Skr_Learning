OPTIMIZER := libDFA.so
OBJs := $(subst .cpp,.o,$(wildcard lib/*.cpp lib/DFA/Domain/*.cpp lib/4-LCM/*.cpp))
inc := /mnt/include/

LLVM_VERSION ?= 16

CXXFLAGS := $(shell llvm-config-$(LLVM_VERSION) --cxxflags) -fPIC -I${inc} -g

all: $(OPTIMIZER) 

$(OPTIMIZER): $(OBJs)
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OPTIMIZER) $(OBJs)