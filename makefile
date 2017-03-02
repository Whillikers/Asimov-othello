CC = g++
LD = g++
DOCGEN = doxygen

SRCDIR = src
BINDIR = bin
OBJDIR = obj
DOCDIR = doc

CPPFLAGS = -std=c++11 -Wall -pedantic -I$(SRCDIR)
LDFLAGS =
DOCGENFLAGS =
DEBUGFLAGS =

COMMON_SRCS = board.cpp wrapper.cpp player.cpp
ASIMOV_SRCS = search.cpp opening.cpp heuristic.cpp
TESTGAME_SRCS = testgame.cpp
TESTMINIMAX_SRCS = testgame.cpp

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
ASIMOV_OBJS = $(COMMON_OBJS) $(ASIMOV_SRCS:.cpp=.o)
TESTGAME_OBJS = $(ASIMOV_OBJS) $(TESTGAME_SRCS:.cpp=.o)
TESTMINIMAX_OBJS = $(ASIMOV_OBJS) $(TESTMINIMAX_SRCS:.cpp=.o)

BINS = asimov

all: $(BINS)

docs:
	$(DOCGEN)

asimov-debug: DEBUGFLAGS = -ggdb
asimov-debug: asimov

asimov: $(addprefix $(OBJDIR)/,$(ASIMOV_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

testgame: $(addprefix $(OBJDIR)/,$(TESTGAME_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

testminimax: $(addprefix $(OBJDIR)/,$(TESTMINIMAX_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.cpp)
	$(CC) -c $(CPPFLAGS) $(DEBUGFLAGS) $< -o $@

clean:
	rm -f $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

clean-docs:
	rm -r $(DOCDIR)/*
