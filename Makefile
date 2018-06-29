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
SRCS = $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(SRCDIR)/gui/*.cpp)
TMP = $(patsubst %.cpp,%.o,$(SRCS))
OBJS = $(subst $(SRCDIR),$(BUILDDIR),$(TMP))

#Gui source and object
GUISRC = gui.C
TMP4 = $(patsubst %.C,%.o,$(GUISRC))
GUIO = $(addprefix $(BUILDDIR)/, $(TMP4))

#Test source and object
TESTSRC = test.C
TMP5 = $(patsubst %.C,%.o,$(TESTSRC))
TESTO = $(addprefix $(BUILDDIR)/, $(TMP5))


#Get info for dictionary generation
TMP1 = $(patsubst %.cpp,%.h,$(SRCS))
ROOTDICH = $(subst src/,$(INCLDIR)/,$(TMP1))
DICTDIR = $(BUILDDIR)/dict
ROOTDICT = dictionary
ROOTDICO = $(DICTDIR)/$(ROOTDICT).o
ROOTDICSRC = $(DICTDIR)/$(ROOTDICT).cxx



# Add various compiler flags
CXXFLAGS = $(shell $(RC) --cflags) -fPIC -std=c++11 -w -I$(INCLDIR) -I$(INCLDIR)/gui
SOFLAGS = -O -shared -std=c++0x

#Name of generated library
MYLIB = CAENGui
SO = $(LIBDIR)/lib$(MYLIB).so

# Add linker flags for CAEN libraries (architecture dependent).
LDFLAGS = $(shell $(RC) --ldflags) -Llib -lCAENVME -lCAENComm -lCAENDigitizer

#Get proper compiler for TUNL
ifeq ($(VERSION), 5)
	CXX = ${HOME}/programs/gcc-4.9.4/bin/g++
else
	CXX = $(shell $(RC) --cxx)
endif

debug: CXXFLAGS += -DDEBUG -g
test: CXXFLAGS += -DDEBUG -g
debug: build

#***************#
#**** RULES ****#
#***************#

#*************************#
# Rules to build the libraries
default : $(SO)
	@echo "Done"

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp $(INCLS)
	@echo  "Building object file '$@' ..."
	@$(CXX) -g $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o : %.C $(INCLS)
	@echo  "Building object file '$@' ..."
	@$(CXX) -DSTANDALONE -g $(CXXFLAGS) -c -o $@ $<

$(SO) : $(OBJS) $(ROOTDICO)
	@echo "Building shared library"
	@$(CXX) $(SOFLAGS) $(LDFLAGS) $^ -o $@

build : $(GUIO) $(SO)
	@echo "Compiling macro ${GUISCR}"
	@$(CXX) -DSTANDALONE -g -o gui.out $< -l$(MYLIB) $(LDFLAGS) $(ROOTGLIBS)
	@echo "Done"

test: $(TESTO) $(SO)
	@echo "Compiling test macro $(TESTSCR)"
	@$(CXX) -DSTANDALONE -g -o test.out $< -l$(MYLIB) $(LDFLAGS) $(ROOTGLIBS)
	@echo "Done"

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
	@echo "Done"

# Useful notes for the uninitiated:
#
# <target> : <dependency list>
#  --TAB-- <rule>
#
# "$@" == subst. the word to the left of the ":"
# "$^" == subst. the words to the right of ":"
# "$<" == subst. first item in dependency list
