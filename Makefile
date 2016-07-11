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
ROOTMAKE:=$(ROOTSYS)/etc/Makefile.arch
include $(ROOTMAKE)

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
TMP = $(patsubst %.C,%.so,$(SRCS))
SOS =  $(subst src/,lib/,$(TMP))

# Add various compiler flags
CXXFLAGS += -w -I$(INCLDIR)

SOFLAGS = -O -shared


# Add linker flags for CAEN libraries (architecture dependent).
LDFLAGS+=-Llib -lCAENVME -lCAENComm -lCAENDigitizer





#***************#
#**** RULES ****#
#***************#

#*************************#
# Rules to build the libraries
default : $(SOS)

$(BUILDDIR)/%.o : $(SRCDIR)/%.C $(INCLS) 
	@echo  "\nBuilding object file '$@' ..."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIBDIR)/%.so : $(BUILDDIR)/%.o $(BUILDDIR)/%_dictionary.oo
	@echo "Building shared libraries"
	$(CXX) $(SOFLAGS) $^ -o $@

#***********************************************#
# Rules to generate the necessary ROOT dictionary


$(BUILDDIR)/%_dictionary.oo : $(BUILDDIR)/%_dictionary.cxx
	@echo  "\nBuilding '$@' ..."
	$(CXX) -g $(CXXFLAGS) -c -o $@ $<

# Generate the necessary ROOT dictionaries
$(BUILDDIR)/%_dictionary.cxx : $(INCLDIR)/%.h Linkdef.h
	@echo  "\nGenerating ROOT dictionary '$@' ..."
	rootcling -f $@  $^
	@cp $(BUILDDIR)/*.pcm ./
	@rm $(BUILDDIR)/*.pcm

# Clean the directory of all build files and binaries
.PHONY: 
clean:
	@echo  "\nCleaning up the build and binary ..."
	rm -f $(BUILDDIR)/*.o *.d $(BUILDDIR)/*Dict.* $(SOS) $(TARGET)
	@echo  ""

# Useful notes for the uninitiated:
#
# <target> : <dependency list>
#  --TAB-- <rule>
#
# "$@" == subst. the word to the left of the ":"
# "$^" == subst. the words to the right of ":"
# "$<" == subst. first item in dependency list
