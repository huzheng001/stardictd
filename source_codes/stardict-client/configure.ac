#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(stardict-client, 0.4, http://code.google.com/p/stardictd/)
AC_CONFIG_SRCDIR([src/stardict-client.cpp])
AM_INIT_AUTOMAKE([dist-xz])
AC_CONFIG_HEADER([config.h])

LT_INIT
PKG_PROG_PKG_CONFIG

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O


# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
DEP_MODULES="glib-2.0 >= 2.6"
PKG_CHECK_MODULES(STARDICT, $DEP_MODULES)

AC_ARG_ENABLE([deprecations],
	AS_HELP_STRING([--disable-deprecations],[Disable deprecated gtk functions (default: enabled)]),
	[enable_deprecations=$enableval],
	[enable_deprecations=yes])

if test "x$enable_deprecations" = "xno" ; then
	STARDICT_CFLAGS="$STARDICT_CFLAGS -DG_DISABLE_DEPRECATED"
	STARDICT_CFLAGS="$STARDICT_CFLAGS -DG_DISABLE_SINGLE_INCLUDES"
fi

STARDICT_CFLAGS="-Wall $STARDICT_CFLAGS"


AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT

echo "

Type \"make\" to compile stardict-client.

"

