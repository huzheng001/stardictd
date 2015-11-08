SUBDIRS= lib tools

bin_PROGRAMS = stardictd

AM_CPPFLAGS = $(MYSQL_INCLUDES)

stardictd_DEPENDENCIES = lib/libstardict.a
stardictd_CPPFLAGS = $(STARDICTD_CFLAGS) $(MYSQL_CFLAGS)
stardictd_LDADD = $(STARDICTD_LIBS) lib/libstardict.a $(MYSQL_LIBS)

stardictd_SOURCES = \
	main.cpp	\
	net.h net.cpp	\
	codes.h	\
	daemon.h daemon.cpp	\
	conf.h conf.cpp \
	log.h log.cpp \
	md5.h md5.c \
	db.h db.cpp \
	servicemain.h servicemain.cpp	\
	stardictdmain.h stardictdmain.cpp
