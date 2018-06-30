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
###############################
##### User specified crap #####
###############################

# Specify the the binary, build, and source directories
BUILDDIR = build
BINDIR   = bin
SRCDIR   = src
LIBDIR   = lib
#Need full path for rooot5
INCLDIR  = $(PWD)/include

#Set the name of the shared library and dictionary
LIBNAME = CAENGui


#################################
##### System Specified crap #####
#################################


# Get info on ROOT
RC:=root-config
ROOTSYS:=$(shell $(RC) --prefix)
V:=$(shell $(RC) --version)
TEMP2 = $(subst ., ,$(V))
VERSION=$(word 1, $(TEMP2))
ROOTGLIBS = $(shell $(RC) --glibs) -lMathMore

# Specify all header files
INCLS = $(INCLDIR)/*.h


# Specify all object files (to be built in the build/ directory)
SRCS = $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(SRCDIR)/gui/*.cpp)
SRCS += $(wildcard $(SRCDIR)/*.cc)

# Create object files
OBJS = $(patsubst $(SRCDIR)%, $(BUILDDIR)%, $(filter %.o, $(SRCS:.cpp=.o )))

SO = $(LIBDIR)/lib$(LIBNAME).so

#Gui source and object
GUISRC = gui.cc
TMP4 = $(patsubst %.cc,%.o,$(GUISRC))
GUIO = $(addprefix $(BUILDDIR)/, $(TMP4))

#Test source and object
TESTSRC = test.cc
TMP5 = $(patsubst %.cc,%.o,$(TESTSRC))
TESTO = $(addprefix $(BUILDDIR)/, $(TMP5))


#Get info for dictionary generation
ROOTDICH = $(patsubst $(SRCDIR)%, $(INCLDIR)%, $(filter %.h, $(SRCS:.cpp=.h )))	
DICTDIR = $(BUILDDIR)/dict
ROOTDICT = dictionary
ROOTDICO = $(DICTDIR)/$(ROOTDICT).o
ROOTDICSRC = $(DICTDIR)/$(ROOTDICT).cxx

# Add various compiler flags
CXXFLAGS = $(shell $(RC) --cflags --libs) -std=c++11 -fPIC -w -I$(INCLDIR) -I$(INCLDIR)/gui
SOFLAGS  = --shared --std=c++11
LDFLAGS  = $(shell $(RC) --ldflags --libs) -L$(LIBDIR) -lCAENComm -lCAENDigitizer -lCAENVME
EXEFLAGS = $(shell $(RC) --cflags --libs) -std=c++11 -w -I$(INCLDIR) -I$(INCLDIR)/gui -L$(LIBDIR) -lCAENComm -lCAENDigitizer -lCAENVME 

#Get proper compiler for TUNL
ifeq ($(VERSION), 5)
	CXX = ${HOME}/programs/gcc-4.9.4/bin/g++
#CXX = /usr/bin/$(shell $(RC) --cxx)
else
	CXX = $(shell $(RC) --cxx)
endif


debug: CXXFLAGS += -DDEBUG -g
debug: SOFLAGS += -DDEBUG -g
debug: LDFLAGS += -DDEBUG -g

debug: all

default: all

#***************#
#**** RULES ****#
#***************#

#*************************#
# Rules to build the libraries
all: $(SO)  $(BINDIR)/gui $(BINDIR)/test $(BINDIR)/unpacker

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp $(INCLS)
	@echo  "Building object file '$@' ..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o : $(SRCDIR)/%.cc $(INCLS)
	@echo  "Building object file '$@' ..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SO) : $(OBJS) $(ROOTDICO)
	@echo "Building shared library"
	@$(CXX) $(SOFLAGS) $(LDFLAGS) $(ROOTGLIBS) $^ -o $@

$(BINDIR)/unpacker: $(SRCDIR)/unpacker.cc $(BUILDDIR)/BinaryLoader.o
	@echo "Building unpacker $@..."
	@$(CXX) $(CXXFLAGS) -o $@ $^


$(BINDIR)/%: $(SRCDIR)/%.cc $(OBJS) $(ROOTDICO)
	@echo "Building $@..."
	@$(CXX) $(EXEFLAGS) $(ROOTGLIBS) -DSTANDALONE  -o $@ $^ 

#%(BINDIR)/% :  $(BUILDDIR)/%.o $(SO)
#	@echo "Linking $@"
#	@$(CXX) -DSTANDALONE -o $@ $< -l$(MYLIB) $(LDFLAGS) $(ROOTGLIBS)



#***********************************************#
# Rules to generate the necessary ROOT dictionary


$(ROOTDICO) : $(ROOTDICSRC)
	@echo  "Building '$@' ..."
	@$(CXX) -g $(CXXFLAGS) -c -o $@ $<

# Generate the necessary ROOT dictionaries
$(ROOTDICSRC) : $(ROOTDICH) Linkdef.h
	@echo  "Generating ROOT dictionary '$@' ..."
ifeq ($(VERSION), 5)
	@rootcint -f $@ -c -p $^
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
	@rm -f $(BUILDDIR)/dict/*.o $(BUILDDIR)/gui/*.o
	@rm -f *Dict*
	@rm -f *.rootmap
	@rm -f $(BINDIR)/*
	@echo "Done"

# Useful notes for the uninitiated:
#
# <target> : <dependency list>
#  --TAB-- <rule>
#
# "$@" == subst. the word to the left of the ":"
# "$^" == subst. the words to the right of ":"
# "$<" == subst. first item in dependency list
