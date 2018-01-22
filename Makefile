# -*- mode: make; tab-width: 8; -*-
#
#
# Makefile for libpulsatile
#
#



#
# Get R compilation details
#
# Location of R executable
R_HOME := 		$(shell R RHOME)

# include headers and libraries for R
RCPPFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --cppflags)
RLDFLAGS  := 		$(shell $(R_HOME)/bin/R CMD config --ldflags)
RBLAS     := 		$(shell $(R_HOME)/bin/R CMD config BLAS_LIBS)
RLAPACK   := 		$(shell $(R_HOME)/bin/R CMD config LAPACK_LIBS)

# if you need to set an rpath to R itself, also uncomment
#RRPATH :=		-Wl,-rpath,$(R_HOME)/lib

# include headers and libraries for RcppArmadillo classes
RCPPARMAINCL := 		$(shell echo 'RcppArmadillo:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)

# include headers and libraries for Rcpp interface classes
RCPPINCL := 		$(shell echo 'Rcpp:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RCPPLIBS := 		$(shell echo 'Rcpp:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)

# include headers and libraries for RInside embedding classes
RINSIDEINCL := 		$(shell echo 'RInside:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RINSIDELIBS := 		$(shell echo 'RInside:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)

#
# Combine compiler flags
#
CPPFLAGS := 		-Wall $(shell $(R_HOME)/bin/R CMD config CPPFLAGS)
CXXFLAGS := 		$(RCPPFLAGS) $(RCPPINCL) $(RCPPARMAINCL) $(RINSIDEINCL) $(shell $(R_HOME)/bin/R CMD config CXXFLAGS)
LDLIBS := 		$(RLDFLAGS) $(RRPATH) $(RBLAS) $(RLAPACK) $(RCPPLIBS) $(RINSIDELIBS)


#
# Original working makefile
#
#CXX := g++ # This is the main compiler
#CXX := clang++ #--analyze # and comment out the linker last line for sanity
CXX := $(shell $(R_HOME)/bin/R CMD config CXX)
SRCDIR := src
BUILDDIR := build
SRCEXT := cpp
TARGET := bin/libpulsatile

SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
LIB := $(LDLIBS)
INC := -I include $(CPPFLAGS) $(CXXFLAGS) -std=c++11


TESTSRCDIR := tests
TESTBUILDDIR := buildtests
TESTTARGET := bin/tests
TESTSOURCES := $(shell find $(TESTSRCDIR) -type f -name *.$(SRCEXT))
TESTOBJECTS := $(patsubst $(TESTSRCDIR)/%,$(TESTBUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))

#
# Recipes
#

all: $(TARGET) $(TESTTARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CXX) $^ -o $(TARGET) $(LIB)"; $(CXX) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

$(TESTTARGET): $(TESTOBJECTS)
	@echo " Linking tests..."
	$(CXX) $^ -o $(TESTTARGET) $(LIB)

$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.$(SRCEXT)
	@mkdir -p $(TESTBUILDDIR)
	$(CXX)  $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	@echo " $(RM) -r $(TESTBUILDDIR) $(TESTTARGET)"; $(RM) -r $(TESTBUILDDIR) $(TESTTARGET)

# 	$(CXX) $(CFLAGS) -I%CATCH_SINGLE_INCLUDE% $(INC) -c tests/tests.cpp tests/proposalvariance_tests.cpp tests/utils_tests.cpp
# 	$(CXX) $(CFLAGS) -I%CATCH_SINGLE_INCLUDE% $(INC) -o bin/tests tests/tests.o tests/proposalvariance_tests.o tests/utils_tests.o && bin/tests --success
# 
# # Tests
# tester: $(TESTOBJECTS)
# 	echo " Building testfile";
# ##	$(CXX) $(CFLAGS) tests/proposalvariance_tests.cpp $(INC) $(LIB) -o bin/proposalvariance_tests
# # 	$(CXX) $(CFLAGS) -I%CATCH_SINGLE_INCLUDE% tests/proposalvariance_tests.cpp $(INC) $(LIB) -c bin/proposalvariance_tests
# # 	$(CXX) $(CFLAGS) -I%CATCH_SINGLE_INCLUDE% tests/proposalvariance_tests.cpp $(INC) $(LIB) -o bin/proposalvariance_tests
# 	$(CXX) $(CFLAGS) -I%CATCH_SINGLE_INCLUDE% $(INC) -c tests/tests.cpp tests/proposalvariance_tests.cpp tests/utils_tests.cpp
# 	$(CXX) $(CFLAGS) -I%CATCH_SINGLE_INCLUDE% $(INC) -o bin/tests tests/tests.o tests/proposalvariance_tests.o tests/utils_tests.o && bin/tests --success

.PHONY: clean

########################################
