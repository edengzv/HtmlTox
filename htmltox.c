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
  | Author: edeng.zheng@gmail.com                                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_htmltox.h"
#include "wkhtmltox/pdf.h"
#include "wkhtmltox/image.h"

#define PHP_WKHTMLTOPDF_
// If you declare any globals in php_htmltox.h uncomment this:
// ZEND_DECLARE_MODULE_GLOBALS(htmltox)
//

/* True global resources - no need for thread safety here */
static int le_htmltox;

zend_class_entry *htmltox_ce;


/* {{{ htmltox_functions[]
 *
 * Every user visible function must have an entry in htmltox_functions[].
 */
const zend_function_entry htmltox_functions[] = {
	//PHP_FE(confirm_htmltox_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in htmltox_functions[] */
};
/* }}} */

/* {{{ htmltox_module_entry
 */
zend_module_entry htmltox_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"htmltox",
	htmltox_functions,
	PHP_MINIT(htmltox),
	PHP_MSHUTDOWN(htmltox),
	PHP_RINIT(htmltox),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(htmltox),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(htmltox),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_HTMLTOX_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HTMLTOX
ZEND_GET_MODULE(htmltox)
#endif

/**
 * methods for htmltox class
 *
 */

ZEND_METHOD(htmltox,htmltopdf)
{
	zval **data;
	HashTable *params_hash;
	HashPosition position;
	int params_count;
	int result;
	zval *global_params;
	zval *object_params;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"a|a",&global_params,&object_params) == FAILURE){
		return;
	}

	//operate the global settings
	wkhtmltopdf_global_settings *global_settings = wkhtmltopdf_create_global_settings();

	htmltox_set_params((void*)global_settings,(fp)wkhtmltopdf_set_global_setting,global_params);
	//create the pdf converter with global setting params
	wkhtmltopdf_converter *c = wkhtmltopdf_create_converter(global_settings);

	//operate the object settings
	//the object setting params is a 2d php array
	params_hash = Z_ARRVAL_P(object_params);
	//the count(array)
	params_count = zend_hash_num_elements(params_hash);
	for(zend_hash_internal_pointer_reset_ex(params_hash,&position);zend_hash_get_current_data_ex(params_hash,(void**)&data,&position) == SUCCESS;zend_hash_move_forward_ex(params_hash,&position)){
		zval tmp = **data;
		//seperate zval
		zval_copy_ctor(&tmp);
		if(Z_TYPE(tmp) == IS_ARRAY){
			//add the object array with wkhtmltopdf_add_object()
			wkhtmltopdf_object_settings *object_settings = wkhtmltopdf_create_object_settings();
			htmltox_set_params((void*)object_settings,(fp)wkhtmltopdf_set_object_setting,&tmp);
			wkhtmltopdf_add_object(c,object_settings,NULL);
		}
		//del zval
		zval_dtor(&tmp);
	}
	//do the conversion
	result = wkhtmltopdf_convert(c);
	//destroy the converter
	wkhtmltopdf_destroy_converter(c);
	//return the result (boolean)
	RETVAL_BOOL(result);
}

ZEND_METHOD(htmltox,htmltoimg)
{
	zval **data;
	int result;
	zval *global_params;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"a",&global_params) == FAILURE){
		return;
	}
	//operate the global settings,(the image converter has no object settings)
	wkhtmltoimage_global_settings *global_settings = wkhtmltoimage_create_global_settings();
	htmltox_set_params((void*)global_settings,(fp)wkhtmltoimage_set_global_setting,global_params);
	//create the converter
	wkhtmltoimage_converter *c = wkhtmltoimage_create_converter(global_settings,NULL);
	//do the conversion
	result = wkhtmltoimage_convert(c);
	//destroy the converter
	wkhtmltoimage_destroy_converter(c);
	//return the result (boolean)
	RETVAL_BOOL(result);
	
}

void htmltox_set_params(void *settings, fp set_function, zval *params){
	zval **data;
	zval tmp;
	HashTable *params_hash;
	HashPosition pointer;
	int params_count;
	
	char *key;
	int key_len;
	long index;

	params_hash = Z_ARRVAL_P(params);
	params_count = zend_hash_num_elements(params_hash);
	for(zend_hash_internal_pointer_reset_ex(params_hash,&pointer);
		zend_hash_get_current_data_ex(params_hash,(void**)&data,&pointer)==SUCCESS;zend_hash_move_forward_ex(params_hash,&pointer)){
		tmp = **data;
		zval_copy_ctor(&tmp);
		//set the settings
		if(zend_hash_get_current_key_ex(params_hash,&key,&key_len,&index,0,&pointer) == HASH_KEY_IS_STRING){
			if(Z_TYPE(tmp) == IS_BOOL){
				set_function(settings,key,Z_BVAL(tmp)?"true":"false");
			}else{
				convert_to_string(&tmp)
				set_function(settings,key,Z_STRVAL(tmp));
			}
		}
	}
	zval_dtor(&tmp);
}

ZEND_METHOD(htmltox,__construct)
{
	wkhtmltopdf_init(0);
}

ZEND_METHOD(htmltox,__destruct)
{
	wkhtmltopdf_deinit();
}

PHP_METHOD(htmltox,htmltopdf);
PHP_METHOD(htmltox,htmltoimg);
PHP_METHOD(htmltox,__construct);
PHP_METHOD(htmltox,__destruct);

static zend_function_entry htmltox_method[] = {
	ZEND_ME(htmltox,htmltopdf,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME(htmltox,htmltoimg,NULL,ZEND_ACC_PUBLIC)
	ZEND_ME(htmltox,__construct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	ZEND_ME(htmltox,__destruct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL,NULL,NULL}
};

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("htmltox.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_htmltox_globals, htmltox_globals)
    STD_PHP_INI_ENTRY("htmltox.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_htmltox_globals, htmltox_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_htmltox_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_htmltox_init_globals(zend_htmltox_globals *htmltox_globals)
{
	htmltox_globals->global_value = 0;
	htmltox_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(htmltox)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce,"htmltox",htmltox_method);
	htmltox_ce = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(htmltox)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(htmltox)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(htmltox)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(htmltox)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "htmltox support", "enabled");
	php_info_print_table_header(2, "version", wkhtmltopdf_version());
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
