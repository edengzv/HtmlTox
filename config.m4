dnl $id$
dnl config.m4 for extension htmltox

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(wkhtmltox-dir, for htmltox support,
Make sure that the comment is aligned:
[  --with-wkhtmltox-dir=[DIR]             Include htmltox support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(htmltox, whether to enable htmltox support,
dnl Make sure that the comment is aligned:
dnl [  --enable-htmltox           Enable htmltox support])

if test "$PHP_WKHTMLTOX_DIR" != "no"; then
  dnl Write more examples of tests here...

  # --with-htmltox -> check with-path
  SEARCH_PATH="$PHP_WKHTMLTOX_DIR"     # you might want to change this
  SEARCH_FOR="include/wkhtmltox/pdf.h"  # you most likely want to change this
  dnl if test -r $PHP_HTMLTOX/$SEARCH_FOR; then # path given as parameter
  dnl   HTMLTOX_DIR=$PHP_HTMLTOX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for htmltox files in default path])
  for i in $SEARCH_PATH ; do
     if test -r $i/$SEARCH_FOR; then
       HTMLTOX_DIR=$i
       AC_MSG_RESULT(found in $i)
     fi
  done
  dnl fi
  dnl
  if test -z "$HTMLTOX_DIR" -o -z "HTMLTOX_DIR/include/wkhtmltox"; then
    AC_MSG_RESULT([$HTMLTOX_DIR not found 1])
    AC_MSG_ERROR([$SEARCH_PATH || $HTMLTOX_DIR || $PHP_HTMLTOX || Please reinstall the htmltox distribution])
  fi

  # --with-htmltox -> add include path
  PHP_ADD_INCLUDE($HTMLTOX_DIR/include)

  # --with-htmltox -> check for lib and symbol presence
  LIBNAME=wkhtmltox # you may want to change this
  LIBSYMBOL=wkhtmltopdf_init # you most likely want to change this

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HTMLTOX_DIR/lib, HTMLTOX_SHARED_LIBADD)
   AC_DEFINE(HAVE_HTMLTOXLIB,1,[libwkhtmltox yes ])
  ],[
   AC_MSG_ERROR([$HTMLTOX_DIR/lib not found 2])
  ],[
    -L$HTMLTOX_DIR/lib -lm
  ])
  
  PHP_SUBST(HTMLTOX_SHARED_LIBADD)

  PHP_NEW_EXTENSION(htmltox, htmltox.c, $ext_shared)
fi
