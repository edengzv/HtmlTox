/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_HTMLTOX_H
#define PHP_HTMLTOX_H

extern zend_module_entry htmltox_module_entry;
#define phpext_htmltox_ptr &htmltox_module_entry

#define PHP_HTMLTOX_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_HTMLTOX_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_HTMLTOX_API __attribute__ ((visibility("default")))
#else
#	define PHP_HTMLTOX_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(htmltox);
PHP_MSHUTDOWN_FUNCTION(htmltox);
PHP_RINIT_FUNCTION(htmltox);
PHP_RSHUTDOWN_FUNCTION(htmltox);
PHP_MINFO_FUNCTION(htmltox);

PHP_FUNCTION(confirm_htmltox_compiled);	/* For testing, remove later. */

typedef int(*fp)(void *settings, const char *name, const char *value);
void htmltox_set_params(void *settings,fp set_function, zval *params);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(htmltox)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(htmltox)
*/

/* In every utility function you add that needs to use variables 
   in php_htmltox_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as HTMLTOX_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define HTMLTOX_G(v) TSRMG(htmltox_globals_id, zend_htmltox_globals *, v)
#else
#define HTMLTOX_G(v) (htmltox_globals.v)
#endif

#endif	/* PHP_HTMLTOX_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
