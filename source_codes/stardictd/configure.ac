#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(stardictd, 0.5, http://stardictd.sourceforge.net)
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE([dist-xz])
AC_CONFIG_HEADER([config.h])

LT_INIT
PKG_PROG_PKG_CONFIG

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O

AC_FUNC_MMAP

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
DEP_MODULES="glib-2.0 >= 2.6"
PKG_CHECK_MODULES(STARDICTD, $DEP_MODULES)

AC_ARG_ENABLE([deprecations],
	AS_HELP_STRING([--disable-deprecations],[Disable deprecated gtk functions (default: enabled)]),
	[enable_deprecations=$enableval],
	[enable_deprecations=yes])

if test "x$enable_deprecations" = "xno" ; then
	STARDICTD_CFLAGS="$STARDICTD_CFLAGS -DG_DISABLE_DEPRECATED"
	STARDICTD_CFLAGS="$STARDICTD_CFLAGS -DG_DISABLE_SINGLE_INCLUDES"
fi

# mysqlclient
AC_ARG_WITH(mysql-config, 
[  --with-mysql-config=PATH   The path to mysql-config if not in $PATH], [
  MYSQLCONFIG=$withval
], [
  MYSQLCONFIG=mysql-config-was-not-found-by-configure
])

if test "x$MYSQLCONFIG" = "xmysql-config-was-not-found-by-configure"; then
	AC_PATH_PROG( MYSQLCONFIG, mysql_config, mysql_config-was-not-found-by-configure,
              $PATH:/usr/bin:/usr/local/bin)
fi

AC_SUBST(MYSQLCONFIG)
if test "x$MYSQLCONFIG" = "xmysql_config-was-not-found-by-configure"; then
  AC_MSG_ERROR([mysql_config wasn't found please install http://www.mysql.com ])
else
  MYSQL_CFLAGS=`$MYSQLCONFIG --cflags`
  MYSQL_INCLUDES=`$MYSQLCONFIG --include`
  MYSQL_LIBS=`$MYSQLCONFIG --libs_r`
fi
AC_SUBST(MYSQL_CFLAGS)
AC_SUBST(MYSQL_INCLUDES)
AC_SUBST(MYSQL_LIBS)

AC_MSG_CHECKING(for MySQL CFLAGS)
AC_MSG_RESULT($MYSQL_CFLAGS)
AC_MSG_CHECKING(for MySQL includes)
AC_MSG_RESULT($MYSQL_INCLUDES)
AC_MSG_CHECKING(for MySQL libraries)
AC_MSG_RESULT($MYSQL_LIBS)


STARDICTD_CFLAGS="-Wall $STARDICTD_CFLAGS"

AC_CHECK_LIB(z,zlibVersion,STARDICTD_LIBS="$STARDICTD_LIBS -lz",[AC_MSG_ERROR([zlib not found])])



AC_CONFIG_FILES([
Makefile
doc/Makefile
etc/Makefile
src/Makefile
src/lib/Makefile
src/tools/Makefile
])
AC_OUTPUT

echo "

Type \"make\" to compile stardictd.

"
