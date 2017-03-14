CC = g++
LD = g++
DOCGEN = doxygen

SRCDIR = src
BINDIR = bin
OBJDIR = obj
DOCDIR = doc

CPPFLAGS = -std=c++11 -Wall -pedantic -I$(SRCDIR) -Ofast
LDFLAGS = -Ofast
DOCGENFLAGS =
UNUSED_DEBUG_FLAGS = -ggdb
DEBUGFLAGS = -g


#Source files for each search algorithm
SEARCH_SRCS = alphabeta.cpp minimax.cpp # mtdf.cpp  monte.cpp
#Source files for each heuristic method
HEURIS_SRCS = basic.cpp lin_fit.cpp better1.cpp h_solver.cpp
#Source files for each opening book
BOOK_SRCS = logistello.cpp

COMMON_SRCS = bitboard.cpp player.cpp search.cpp opening.cpp heuristic.cpp \
	transposition.cpp\
	$(addprefix search_alg/,$(SEARCH_SRCS)) \
	$(addprefix heuristics/,$(HEURIS_SRCS)) \
	$(addprefix opening_books/,$(BOOK_SRCS))
#Source files for the full AI player
ASIMOV_SRCS = wrapper.cpp

OPENING_SRCS = $(ASIMOV_SRCS) opening_gen/generator.cpp

TESTGAME_SRCS = testgame.cpp
TESTMINIMAX_SRCS = testminimax.cpp

TESTBOARD_SRCS = bitboard.cpp testboard.cpp

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
ASIMOV_OBJS = $(COMMON_OBJS) $(ASIMOV_SRCS:.cpp=.o)
OPENING_OBJS = $(COMMON_OBJS) $(OPENING_SRCS:.cpp=.o)
TESTGAME_OBJS = $(TESTGAME_SRCS:.cpp=.o)
TESTMINIMAX_OBJS = $(COMMON_OBJS) $(TESTMINIMAX_SRCS:.cpp=.o)
TESTBOARD_OBJS = $(TESTBOARD_SRCS:.cpp=.o)
PLAYSELF_OBJS = $(COMMON_OBJS) play_self.o

ML_VALUE_TRAIN_SRCS = ml-heuristic/train_value.cpp bitboard.cpp
ML_POLICY_TRAIN_SRCS = ml-heuristic/train_policy.cpp bitboard.cpp

ML_VALUE_TRAIN_OBJS = $(ML_VALUE_TRAIN_SRCS:.cpp=.o)
ML_POLICY_TRAIN_OBJS = $(ML_POLICY_TRAIN_SRCS:.cpp=.o)

BINS = asimov

all: $(BINS)

docs:
	$(DOCGEN)

#Debug target to generate debug symbols
asimov-debug: DEBUGFLAGS = $(UNUSED_DEBUG_FLAGS)
asimov-debug: asimov

#Target to build the main Othello AI
asimov: $(addprefix $(OBJDIR)/,$(ASIMOV_OBJS))
	echo $(ASIMOV_OBJS)
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

#Debug target to generate debug symbols
opening-debug: DEBUGFLAGS = -ggdb
opening-debug: asimov

#Target to build the opening book generator
opening: $(addprefix $(OBJDIR)/,$(OPENING_SRCS))
	echo $(ASIMOV_OBJS)
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)


#Program to manage and display a game between two AIs
testgame: $(addprefix $(OBJDIR)/,$(TESTGAME_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)


#Program to test the minimax algorithm
testminimax: DEBUGFLAGS = $(UNUSED_DEBUG_FLAGS)
testminimax: $(addprefix $(OBJDIR)/,$(TESTMINIMAX_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

#Program to manually verify the correctness of the bitboard
testboard: $(addprefix $(OBJDIR)/,$(TESTBOARD_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)


#Program that plays the current AI against itself
playself: DEBUGFLAGS = $(UNUSED_DEBUG_FLAGS)
playself: $(addprefix $(OBJDIR)/,$(PLAYSELF_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS) -lprofiler -ggdb

#Game data parsers and logistic regression training data generators
trainvalue: $(addprefix $(OBJDIR)/,$(ML_VALUE_TRAIN_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

trainpolicy: $(addprefix $(OBJDIR)/,$(ML_POLICY_TRAIN_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)


#target to make object files
$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.cpp)
	mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(DEBUGFLAGS) $< -o $@

clean:
	rm -rf $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*

clean-docs:
	rm -r $(DOCDIR)/*
