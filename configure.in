dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/SQLAllocHandle.c)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(oraodbc, 0.5.1)
AM_CONFIG_HEADER(config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AM_PROG_LEX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl Checks for libraries.
AC_CHECK_LIB(pthread, pthread_mutex_lock)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(sys/types.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl set up libtool
AM_PROG_LIBTOOL

dnl set up lib version
LIB_VERSION="1:0:0"
AC_SUBST(LIB_VERSION)

dnl look for unixODBC
unixODBC="/usr/local"
AC_ARG_WITH( unixODBC,
[  --with-unixODBC[=DIR]   Use unixODBC located in DIR [default=/usr/local]
  --without-unixODBC      Use alternative driver manager],
[ unixODBC="$withval"])

if test x$withval = xno
then
 AC_MSG_WARN(skipping unixODBC search)
else
 odbcdir=""
 AC_MSG_CHECKING(for unixODBC files)
 for d in $unixODBC /usr /usr/local
 do
 if test -f $d/lib/libodbc.so && test -f $d/lib/libodbcinst.so
 then
    AC_MSG_RESULT(found unixODBC in $d)
    odbcdir=$d
    break
 fi
 done
 
 if test x$odbcdir = x
 then
     AC_MSG_ERROR( unixODBC not found )
 fi
 
 ODBCIDIR=${odbcdir}/include
 ODBCLDIR=${odbcdir}/lib
 ODBCLIB=libodbc.so
 ODBCINSTLIB=libodbcinst.so
 LD_ODBCINSTLIB="-L$ODBCLDIR -lodbcinst"

 AC_SUBST(ODBCIDIR)
 AC_SUBST(ODBCLDIR)
 AC_SUBST(ODBCLIB)
 AC_SUBST(ODBCINSTLIB)
 AC_SUBST(LD_ODBCINSTLIB)
fi

dnl look for ORACLE_HHOME
if test "x$ORACLE_HOME" = "x"
then
    AC_MSG_WARN( ORACLE_HOME not set )
    oraclehome="/usr/local/share/orasoft"
else
    oraclehome="$ORACLE_HOME"
fi

AC_ARG_WITH( orahome,
[  --with-oraclehome[=DIR] Look in DIR for the ORACLE files [default=/usr/local/share/orasoft]],
[ oraclehome="$withval"])

orahdir=""
AC_MSG_CHECKING(for oci.h)
for d in $oraclehome/rdbms/demo $oraclehome/rdbms/public $oraclehome/oci/include /usr/include /usr/local/include $oraclehome/include
do
if test -f $d/oci.h
then
   AC_MSG_RESULT(found oci.h in $d)
   orahdir=$d
   break
fi
done

if test x$orahdir = x
then
    AC_MSG_ERROR( oci.h not found )
fi

orandir=""
AC_MSG_CHECKING(for nzt.h)
for d in $oraclehome/network/public $oraclehome/oci/include /usr/include /usr/local/include $oraclehome/public $oraclehome/rdbms/public $oraclehome/include
do
if test -f $d/nzt.h
then
   AC_MSG_RESULT(found nzt.h in $d)
   orandir=$d
   break
fi
done

if test x$orandir = x
then
    AC_MSG_ERROR( nzt.h not found )
fi

orapdir=""
AC_MSG_CHECKING(for ociextp.h)
for d in $oraclehome/rdbms/public $oraclehome/rdbms/demo $oraclehome/plsql/public $oraclehome/oci/include /usr/include /usr/local/include $oraclehome/include
do
if test -f $d/ociextp.h
then
    AC_MSG_RESULT(found ociextp.h in $d)
    orapdir=$d
    break
fi
done

if test x$orapdir = x
then
   AC_MSG_ERROR(ociextp.h not found)
fi

oraldir=""
AC_MSG_CHECKING(for libclntsh.so )
for d in $oraclehome/lib /usr/lib /usr/local/lib 
do
if test -f $d/libclntsh.so || test -f $d/libclntsh.a
then
   AC_MSG_RESULT(found libclntsh.so in $d)
   oraldir=$d
   break
fi
done

ORAIDIR=${orahdir}
ORANDIR=${orandir}
ORAPDIR=${orapdir}
ORALDIR=${oraldir}
    
AC_SUBST(ORAIDIR)
AC_SUBST(ORANDIR)
AC_SUBST(ORAPDIR)
AC_SUBST(ORALDIR)
if test x$oraldir = x
then
    if test -f $oraclehome/bin/oci.dll 
	then
	    AC_MSG_RESULT( oci.dll. See README.cygwin)
	else
        AC_MSG_ERROR( libclntsh.so and oci.dll not found )
	fi
else

    AC_CHECK_LIB(clntsh,OCIEnvCreate,AC_DEFINE(LIBCLNTSH8),,
    "-L$oraldir")

    ORALIB="-lclntsh"
    AC_SUBST(ORALIB)
    
    dnl Compile time options.
    
    dnl Undefined symbol problems (slpmprodstab) with libclntsh.so. If linking 
    dnl against it with a function that it should contain whatever version it
    dnl is fails, then there is a good chance that the symbol hack is required.
    AC_CHECK_LIB(clntsh,OCIInitialize,[ symbolcheck="no" ] ,[ symbolcheck="yes" ],
    "-L$oraldir")
    AC_MSG_CHECKING(whether to enable libclntsh.so undefined symbol hack)
    AC_ARG_ENABLE( symbol-hack, 
    [  --enable-symbol-hack[=yes|no] Enable libclntsh.so undefined symbol workaround [default=no] ],
    [ symbolval=$enableval ],[ symbolval="default" ] )
    if test x$symbolval = xyes 
    then
        AC_DEFINE(EXPORT_SLPMPRODSTAB)
        AC_MSG_RESULT(yes (autodetection overridden))
    else
        if test x$symbolval = xdefault
	    then
	        if test x$symbolcheck = xyes
		    then
                AC_MSG_RESULT(yes (autodetected))
                AC_DEFINE(EXPORT_SLPMPRODSTAB)
		    else
                AC_MSG_RESULT(no (autodetected))
		    fi
	    else
            AC_MSG_RESULT(no (autodetection overridden))
	    fi
    fi
fi

dnl AC_ARG_WITH( ora_version,
dnl 	[  --with-oracle_version[ =major ] default = 9,
dnl 	[ ora_version="$withval" ] )
dnl if test x$withval = xno
dnl then




AC_ARG_ENABLE(binary_float,
[  --enable-binary-float[=yes|no]  support Oracle 10 IEEE 754 floating point numbers [default=no]],
[if test x$enableval = xno
then    	
    AC_MSG_RESULT(disabling BINARY_FLOAT support )	
else
    AC_DEFINE(IEEE_754_FLT)
    AC_MSG_RESULT(enabling BINARY_FLOAT support)	
fi
])

AC_MSG_CHECKING(whether to enable ODBC tracing)
AC_ARG_ENABLE( trace, 
[  --enable-trace[=yes|no]          Include ODBC tracing (slow) [default=no] ],
[ traceval=$enableval ],[ traceval="no" ] )
if test x$traceval = xyes
then
    AC_DEFINE(ENABLE_TRACE)
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(whether to use just user or all catalog)
AC_ARG_ENABLE( user-catalog,
[  --enable-user-catalog[=yes|no]   User user catalog for catalog functions ],
[ catalog=$enableval ],[ catalog="no" ] )
if test x$catalog = xno || test x$catalog = xall
then
    AC_MSG_RESULT(all catalog)
else
    AC_DEFINE(ENABLE_USER_CATALOG)
	AC_MSG_RESULT(user catalog)
fi

AC_OUTPUT( Makefile test/Makefile src/Makefile  )
