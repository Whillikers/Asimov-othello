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

#Source files for each search algorithm
SEARCH_SRCS = mtdf.cpp
#Source files for each heuristic method
HEURIS_SRCS = basic.cpp lin_fit.cpp

#Source files for the full AI player
ASIMOV_SRCS = search.cpp opening.cpp heuristic.cpp transposition.cpp\
	$(addprefix search_alg/,$(SEARCH_SRCS)) \
	$(addprefix heuristics/,$(HEURIS_SRCS))

OPENING_SRCS = $(ASIMOV_SRCS) opening_gen/generator.cpp

TESTGAME_SRCS = testgame.cpp
TESTMINIMAX_SRCS = testgame.cpp

TESTBOARD_SRCS = board.cpp testboard.cpp

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
ASIMOV_OBJS = $(COMMON_OBJS) $(ASIMOV_SRCS:.cpp=.o)
OPENING_OBJS = $(COMMON_OBJS) $(OPENING_SRCS:.cpp=.o)
TESTGAME_OBJS = $(ASIMOV_OBJS) $(TESTGAME_SRCS:.cpp=.o)
TESTMINIMAX_OBJS = $(ASIMOV_OBJS) $(TESTMINIMAX_SRCS:.cpp=.o)
TESTBOARD_OBJS = $(TESTBOARD_SRCS:.cpp=.o)

BINS = asimov

all: $(BINS)

docs:
	$(DOCGEN)

#Debug target to generate debug symbols
asimov-debug: DEBUGFLAGS = -ggdb
asimov-debug: asimov

asimov: $(addprefix $(OBJDIR)/,$(ASIMOV_OBJS))
	echo $(ASIMOV_OBJS)
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

#Debug target to generate debug symbols
opening-debug: DEBUGFLAGS = -ggdb
opening-debug: asimov

opening: $(addprefix $(OBJDIR)/,$(OPENING_SRCS))
	echo $(ASIMOV_OBJS)
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

testgame: $(addprefix $(OBJDIR)/,$(TESTGAME_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

testminimax: $(addprefix $(OBJDIR)/,$(TESTMINIMAX_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

testboard: $(addprefix $(OBJDIR)/,$(TESTBOARD_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.cpp)
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(DEBUGFLAGS) $< -o $@

clean:
	rm -rf $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

clean-docs:
	rm -r $(DOCDIR)/*
