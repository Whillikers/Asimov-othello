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

COMMON_SRCS =
ASIMOV_SRCS = main.cpp

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
ASIMOV_OBJS = $(COMMON_OBJS) $(ASIMOV_SRCS:.cpp=.o)

BINS = asimov

all: $(BINS)

docs:
	$(DOCGEN)

asimov-debug: DEBUGFLAGS = -g
asimov-debug: asimov

asimov: $(addprefix $(OBJDIR)/,$(ASIMOV_OBJS))
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/$(notdir %.cpp)
	$(CC) -c $(CPPFLAGS) $(DEBUGFLAGS) $< -o $@

clean:
	rm -f $(addprefix $(BINDIR)/,$(BINS)) $(OBJDIR)/*.o

clean-docs:
	rm -r $(DOCDIR)/*
