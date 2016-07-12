# Makefile for CAEN digitizer gui
# Adam Anthony 7/10/16
# To build the binary
# $ make 
#
# To clean the bin/ and build/ directories
# $ make clean

#**** MACRO DEFINITIONS ****#

# Include the Makefile for ROOT-based projects
RC:=root-config
ROOTSYS:=$(shell $(RC) --prefix)
V:=$(shell $(RC) --version)
TEMP2 = $(subst ., ,$(V))
VERSION=$(word 1, $(TEMP2))

# Specify the the binary, build, and source directories
BUILDDIR = build
BINDIR = bin
SRCDIR = src
LIBDIR = lib

# Specify header files directory. Note that this must be an absolute
# path to ensure the ROOT dictionary files can find the headers
INCLDIR = $(PWD)/include

# Specify all header files
INCLS = $(INCLDIR)/*.h

# Specify all object files (to be built in the build/ directory)
SRCS = $(wildcard $(SRCDIR)/*.C)
TMP = $(patsubst %.C,%.o,$(SRCS))
OBJS = $(subst src/,build/,$(TMP))

#Get info for dictionaries
TMP1 = $(patsubst %.C,%.h,$(SRCS))
ROOTDICH = $(subst src/,$(INCLDIR)/,$(TMP1))
DICTDIR = $(BUILDDIR)/dict
ROOTDICT = dictionary
ROOTDICO = $(DICTDIR)/$(ROOTDICT).o
ROOTDICSRC = $(DICTDIR)/$(ROOTDICT).cxx

# Add various compiler flags
CXXFLAGS = $(shell $(RC) --cflags) -fPIC -std=c++11 -w -I$(INCLDIR)

SOFLAGS = -O -shared -std=c++0x

SO = $(LIBDIR)/libCAENGui.so

# Add linker flags for CAEN libraries (architecture dependent).
LDFLAGS = $(shell $(RC) --ldflags) -Llib -lCAENVME -lCAENComm -lCAENDigitizer

#Get proper compiler for TUNL
ifeq ($(VERSION), 5)
	CXX = /home/reu/aka30/gcc-4.9.3/bin/g++
else
	CXX = $(shell $(RC) --cxx)
endif



#***************#
#**** RULES ****#
#***************#

#*************************#
# Rules to build the libraries
default : $(SO)

$(BUILDDIR)/%.o : $(SRCDIR)/%.C $(INCLS)
	@echo  "\nBuilding object file '$@' ..."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SO) : $(OBJS) $(ROOTDICO)
	@echo "Building shared library"
	$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@

#***********************************************#
# Rules to generate the necessary ROOT dictionary


$(ROOTDICO) : $(ROOTDICSRC)
	@echo  "\nBuilding '$@' ..."
	$(CXX) -g $(CXXFLAGS) -c -o $@ $<

# Generate the necessary ROOT dictionaries
$(ROOTDICSRC) : $(ROOTDICH) Linkdef.h
	@echo  "\nGenerating ROOT dictionary '$@' ..."
ifeq ($(VERSION), 5)
	rootcint -f $@ -c  $^
	rlibmap -f -o gui.rootmap -l $(SO) -c Linkdef.h
else
	rootcling -f $@ -rml $(SO) -rmf gui.rootmap $^
	@cp -f $(DICTDIR)/*.pcm ./
	@rm -f $(DICTDIR)/*.pcm
endif

# Clean the directory of all build files and binaries
.PHONY: 
clean:
	@echo  "\nCleaning up the build and binary ..."
	rm -f $(BUILDDIR)/*.o *.d $(BUILDDIR)/*Dict.* $(SO) $(TARGET)
	rm -f $(BUILDDIR)/dict/*
	rm -f *Dict*	
	@echo  ""

# Useful notes for the uninitiated:
#
# <target> : <dependency list>
#  --TAB-- <rule>
#
# "$@" == subst. the word to the left of the ":"
# "$^" == subst. the words to the right of ":"
# "$<" == subst. first item in dependency list
