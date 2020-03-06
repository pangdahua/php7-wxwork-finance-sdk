dnl $Id$
dnl config.m4 for extension wxwork_finance_sdk

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(wxwork-finance-sdk, for wxwork_finance_sdk support,
dnl Make sure that the comment is aligned:
[  --with-wxwork-finance-sdk=DIR           Include wxwork_finance_sdk support])

dnl Otherwise use enable:


if test "$PHP_WXWORK_FINANCE_SDK" != "no"; then
  dnl check wxwork finance sdk is valid


  AC_MSG_CHECKING($PHP_WXWORK_FINANCE_SDK)
  if test -f "${PHP_WXWORK_FINANCE_SDK}/WeWorkFinanceSdk_C.h" && test -f "${PHP_WXWORK_FINANCE_SDK}/libWeWorkFinanceSdk_C.so"; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_ERROR(cannot find WeWorkFinanceSdk_C.h in path ${PHP_WXWORK_FINANCE_SDK})
  fi

  PHP_ADD_INCLUDE($PHP_WXWORK_FINANCE_SDK)
  PHP_ADD_LIBRARY_WITH_PATH(WeWorkFinanceSdk_C, $PHP_WXWORK_FINANCE_SDK, WXWORK_FINANCE_SDK_SHARED_LIBADD)

  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=`$PKG_CONFIG foo --cflags`
  dnl     LIBFOO_LIBDIR=`$PKG_CONFIG foo --libs`
  dnl     LIBFOO_VERSON=`$PKG_CONFIG foo --modversion`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, WXWORK_FINANCE_SDK_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-wxwork_finance_sdk -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/wxwork_finance_sdk.h"  # you most likely want to change this
  dnl if test -r $PHP_WXWORK_FINANCE_SDK/$SEARCH_FOR; then # path given as parameter
  dnl   WXWORK_FINANCE_SDK_DIR=$PHP_WXWORK_FINANCE_SDK
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for wxwork_finance_sdk files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WXWORK_FINANCE_SDK_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WXWORK_FINANCE_SDK_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the wxwork_finance_sdk distribution])
  dnl fi

  dnl # --with-wxwork_finance_sdk -> add include path
  dnl PHP_ADD_INCLUDE($WXWORK_FINANCE_SDK_DIR/include)

  dnl # --with-wxwork_finance_sdk -> check for lib and symbol presence
  dnl LIBNAME=wxwork_finance_sdk # you may want to change this
  dnl LIBSYMBOL=wxwork_finance_sdk # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WXWORK_FINANCE_SDK_DIR/$PHP_LIBDIR, WXWORK_FINANCE_SDK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WXWORK_FINANCE_SDKLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong wxwork_finance_sdk lib version or lib not found])
  dnl ],[
  dnl   -L$WXWORK_FINANCE_SDK_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WXWORK_FINANCE_SDK_SHARED_LIBADD)

  PHP_NEW_EXTENSION(wxwork_finance_sdk, wxwork_finance_sdk.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_SUBST(WXWORK_FINANCE_SDK_SHARED_LIBADD)
fi
