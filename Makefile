# Makefile for CAEN digitizer gui
# Adam Anthony 7/10/16
# To build the shared library
# $ make 
#
# To create an executable GUI
# $ make build
# 
# To clean the bin/ and build/ directories
# $ make clean

#**** MACRO DEFINITIONS ****#

# Get info on ROOT
RC:=root-config
ROOTSYS:=$(shell $(RC) --prefix)
V:=$(shell $(RC) --version)
TEMP2 = $(subst ., ,$(V))
VERSION=$(word 1, $(TEMP2))
ROOTGLIBS = $(shell $(RC) --glibs) -lMathMore

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
OBJS = $(subst $(SRCDIR),$(BUILDDIR),$(TMP))
GUISRC = $(wildcard *.C)
TMP4 = $(patsubst %.C,%.o,$(GUISRC))
GUIO = $(addprefix $(BUILDDIR)/, $(TMP4))


#Get info for dictionary generation
TMP1 = $(patsubst %.C,%.h,$(SRCS))
ROOTDICH = $(subst src/,$(INCLDIR)/,$(TMP1))
DICTDIR = $(BUILDDIR)/dict
ROOTDICT = dictionary
ROOTDICO = $(DICTDIR)/$(ROOTDICT).o
ROOTDICSRC = $(DICTDIR)/$(ROOTDICT).cxx

# Add various compiler flags
CXXFLAGS = $(shell $(RC) --cflags) -fPIC -std=c++11 -w -I$(INCLDIR)
SOFLAGS = -O -shared -std=c++0x

#Name of generated library
MYLIB = CAENGui
SO = $(LIBDIR)/lib$(MYLIB).so

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
	@echo "Done"

$(BUILDDIR)/%.o : $(SRCDIR)/%.C $(INCLS)
	@echo  "Building object file '$@' ..."
	@$(CXX) -g $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o : %.C $(INCLS)
	@echo  "Building object file '$@' ..."
	@$(CXX) -Dstand -g $(CXXFLAGS) -c -o $@ $<

$(SO) : $(OBJS) $(ROOTDICO)
	@echo "Building shared library"
	@$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@

build : $(GUIO) $(SO)
	@echo "Compiling macro $(GUISCR)"
	$(CXX) -g -o gui.out $< -l$(MYLIB) $(LDFLAGS) $(ROOTGLIBS)

#***********************************************#
# Rules to generate the necessary ROOT dictionary


$(ROOTDICO) : $(ROOTDICSRC)
	@echo  "Building '$@' ..."
	@$(CXX) -g $(CXXFLAGS) -c -o $@ $<

# Generate the necessary ROOT dictionaries
$(ROOTDICSRC) : $(ROOTDICH) Linkdef.h
	@echo  "Generating ROOT dictionary '$@' ..."
ifeq ($(VERSION), 5)
	@rootcint -f $@ -c  $^
	@rlibmap -f -o gui.rootmap -l $(SO) -c Linkdef.h
else
	@rootcling -f $@ -rml $(SO) -rmf gui.rootmap $^
	@cp -f $(DICTDIR)/*.pcm ./
	@rm -f $(DICTDIR)/*.pcm
endif

# Clean the directory of all build files and binaries
.PHONY: 
clean:
	@echo  "Cleaning up the build and binary ..."
	@rm -f $(BUILDDIR)/*.o *.d $(BUILDDIR)/*Dict.* $(SO) $(TARGET)
	@rm -f $(BUILDDIR)/dict/*
	@rm -f *Dict*
	@echo "Done"

# Useful notes for the uninitiated:
#
# <target> : <dependency list>
#  --TAB-- <rule>
#
# "$@" == subst. the word to the left of the ":"
# "$^" == subst. the words to the right of ":"
# "$<" == subst. first item in dependency list
