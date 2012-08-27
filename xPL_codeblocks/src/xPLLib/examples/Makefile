#
# Makefile for xPLLib
#

#
# For LINUX, use the following
CCOPTS = -g -DLINUX -pedantic -Wall
LIBS = -g -lm 

# For UnixWare systems, use the following
# CCOPTS = -O2 -DUNIXWARE
# LIBS = -lnsl -lsocket -lm

#
# For Alpha/DEC OSF/Tru64
#
# CCOPTS = -O2 -DTRU64
# LIBS = -lm

# *******************************************************
# ******* No more customizations from here down *********
# *******************************************************

LDOPTS	= -O
CC	=	cc $(CCOPTS)
LD	= 	cc $(LDOPTS)

CMD_LIST = xPL_Hub xPL_Logger xPL_Clock xPL_ConfigClock xPLSend

.c.o:	
	$(CC) -c $<

.o:
	$(LD) -o $@ $< ../xPL.a $(LIBS) 

.c:	
	$(CC) -c $<
	$(LD) -o $@ $< ../xPL.a $(LIBS)


all:	${CMD_LIST} statichub

statichub: xPL_Hub.o
	$(LD) -static -o xPL_Hub_static xPL_Hub.o ../xPL.a $(LIBS)

clean:
	rm -f *.o *.a core ${CMD_LIST}


hubdist: rebuild
	rm -f ../../web/xPLHub.tgz
	tar czf ../../web/xPLHub.tgz xPL_Hub xPL_Hub_static xPLHub_INSTALL.txt

rebuild: clean all





