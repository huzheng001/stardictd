bin_PROGRAMS = stardict-client

AM_CPPFLAGS =

stardict_client_CPPFLAGS= $(STARDICT_CFLAGS)
stardict_client_LDFLAGS =
stardict_client_LDADD = $(STARDICT_LIBS)

stardict_client_SOURCES = \
	stardict-client.cpp \
	net.cpp net.h \
	command.cpp command.h \
	printdata.cpp printdata.h \
	md5.c md5.h \
	codes.h
