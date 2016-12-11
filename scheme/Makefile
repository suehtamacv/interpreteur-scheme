TARGET=scheme
ROOTDIR=sfsn
DIRNAME=`basename $(PWD)`
CC=`which gcc`
LD=`which gcc`
RM=`which rm` -f
CP=`which cp` -R
CD=`which cd`

SRCDIR=src
INCDIR=include
TESTDIR=testing
LIBDIR=lib

GARBAGE=*~ $(SRCDIR)/*~ $(INCDIR)/*~ $(TESTDIR)/*~

INCLUDE=-I$(INCDIR)

# Pour activer les sorties INFO_MSG, ajouter -DVERBOSE aux CFLAGS 
CFLAGS=-Wall -ansi $(INCLUDE)
LFLAGS=-lreadline -lm

CFLAGS_DBG=$(CFLAGS) -g -DDEBUG -Wall
CFLAGS_RLS=$(CFLAGS)
CFLAGS_VBS=$(CFLAGS) -g -DDEBUG -DVERBOSE -Wall

SRC=$(wildcard $(SRCDIR)/*.c)

OBJ_DBG=$(SRC:.c=.dbg)
OBJ_RLS=$(SRC:.c=.rls)
OBJ_VBS=$(SRC:.c=.vbs)

all : 
	@echo "in " $(DIRNAME)
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make debug   => build DEBUG   version"
	@echo "make release => build RELEASE version"
	@echo "make verbose => build VERBOSE version"
	@echo "make clean   => clean everything"
	@echo "make tarball => produce archive"

debug   : $(OBJ_DBG)
	$(LD) $^ $(LFLAGS) -o $(TARGET)
	$(CP) $(LIBDIR) ../$(TESTDIR)

release : $(OBJ_RLS)
	$(LD) $^ $(LFLAGS) -o $(TARGET)
	$(CP) $(LIBDIR) ../$(TESTDIR)

verbose : $(OBJ_VBS)
	$(LD) $^ $(LFLAGS) -o $(TARGET)
	$(CP) $(LIBDIR) ../$(TESTDIR)

%.dbg : %.c
	$(CC) $< $(CFLAGS_DBG) -c -o $(basename $<).dbg

%.rls : %.c
	$(CC) $< $(CFLAGS_RLS) -c -o $(basename $<).rls

%.vbs : %.c
	$(CC) $< $(CFLAGS_VBS) -c -o $(basename $<).vbs

clean : 
	$(RM) $(TARGET) $(SRCDIR)/*.orig $(SRCDIR)/*.dbg $(SRCDIR)/*.rls $(SRCDIR)/*.vbs $(GARBAGE) scheme-`whoami`-*.tgz

tarball : 
	make clean 
	cd .. && tar -czvf scheme-`whoami`-`date +%d-%m-%H-%M`.tgz $(DIRNAME) && cd $(DIRNAME) && mv ../scheme-`whoami`-*.tgz .

