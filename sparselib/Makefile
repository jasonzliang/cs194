INCDIR =
DEFINE =
LIBS   = -lm
CC     = g++
CFLAGS = -g0 -pedantic -Wall -O2

SRCS = \
sparselib.test.cc \
sparselib.testtrid.cc \
sparselib.testv.cc \
sparselib.timing.cc

OBJS = $(SRCS:%.cc=%.o)

FILES=$(SRCS) license Makefile sparselib.pdf *.hh

all:	compile

doc:
	pdflatex sparselib
	pdflatex sparselib
	pdflatex sparselib

html:
	latex2html -split 0 -init_file sparselib.perl -noinfo -image_type gif -white -local_icons -no_navigation -no_reuse sparselib	

compile:  $(OBJS)
	$(CC) $(OBJS) -o test $(LIBS)

clean:
	rm -rf $(OBJS) *~ *.log *.aux *.toc *.dvi sparselib

tgz:
	rm -rf sparselib
	mkdir sparselib
	cp $(FILES) sparselib
	tar --gzip --create --file=sparselib.tgz sparselib
	
.cc.o:
	@echo COMPILING $<
	@$(CC) $(CFLAGS) $(INCDIR) $(DEFINE) -c $< -o $@


depend:
	mkdep $(CFLAGS) $(INCDIR) $(DEFINE) $(SRCS)

#makedepend $(INCDIR)  -- $(CFLAGS) -- $(SRCS)
