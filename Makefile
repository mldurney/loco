SRCDIR = src
OBJDIR = obj
BINDIR = bin

CXX		 = g++
CXXFLAGS = -std=c++1z -I$(SRCDIR) -ggdb -Wall -Wextra -pedantic
COMMON_SRCS = matrix.cpp loco.cpp
TEST_MATRIX_SRCS = matrix.cpp test_matrix.cpp
TEST_MATRIX_OBJS = $(TEST_MATRIX_SRCS:.cpp=.o)
TEST_MATRIX_BINS = test_matrix
TEST_LOCO_SRCS = $(COMMON_SRCS) test_loco.cpp
TEST_LOCO_OBJS = $(TEST_LOCO_SRCS:.cpp=.o)
TEST_LOCO_BINS = test_loco
BINS = $(TEST_MATRIX_BINS) $(TEST_LOCO_BINS)

all : $(BINS)

test_loco : $(addprefix $(SRCDIR)/,$(TEST_LOCO_SRCS))
	cd $(OBJDIR); $(CXX) -c $(CXXFLAGS) $(addprefix ../$(SRCDIR)/,$(TEST_LOCO_SRCS)); cd ..
	$(CXX) -o $(BINDIR)/test_loco $(addprefix $(OBJDIR)/,$(TEST_LOCO_OBJS)) $(CXXFLAGS)

test_matrix : $(addprefix $(SRCDIR)/,$(TEST_MATRIX_SRCS))
	cd $(OBJDIR); $(CXX) -c $(CXXFLAGS) $(addprefix ../$(SRCDIR)/,$(TEST_MATRIX_SRCS)); cd ..
	$(CXX) -o $(BINDIR)/test_matrix $(addprefix $(OBJDIR)/,$(TEST_MATRIX_OBJS)) $(CXXFLAGS)

clean:
	rm -f $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

.PHONY : all clean
