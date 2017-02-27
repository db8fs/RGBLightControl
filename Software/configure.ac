##############################################################################
dnl Process this file with autoconf to produce a configure script.
##############################################################################

AC_PREREQ(2.59)
AC_INIT(	RGBLightControl, 
		0.3, 
		https://sourceforge.net/p/rgblightcontrol/tickets/,
		rgblightcontrol,
		https://rgblightcontrol.sf.net)

LT_INIT

AC_CONFIG_HEADERS(include/config.h)
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LIBTOOL
AC_PROG_GREP
AC_PROG_EGREP
AC_PROG_CXX
AC_CANONICAL_SYSTEM
AC_FUNC_ERROR_AT_LINE
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AM_INIT_AUTOMAKE()

BOOST_REQUIRE(1.36)
BOOST_ASIO	
BOOST_SYSTEM
BOOST_THREADS

case "$target" in
    *-*-linux*) 
	AC_DEFINE(HAVE_LINUX, 1, [Define to 1 if your target OS is Linux based])
	;;
esac

#AC_ARG_WITH([grabber], AS_HELP_STRING([--with-grabber], [Enables the Drembox videograbber module])

###############################################################################
dnl Dreambox Grabber support
###############################################################################

AC_ARG_ENABLE([dreamboxgrabber],
    AS_HELP_STRING([--enable-dreamboxgrabber], [Enables the Dreambox video grabber]))

AS_IF([test "x$enable_dreamboxgrabber" = "xyes"], [
    AC_DEFINE([HAVE_DREAMBOX], [1], [Dreambox video grabber enabled])
])


# TODO: configure check for linux kernel headers and disabling the framebuffer features
# if we compile for other target_os or if framebuffer is not available. 

AC_CONFIG_FILES(Makefile src/Makefile include/Makefile data/Makefile)
AC_OUTPUT

##############################################################################
