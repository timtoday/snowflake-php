/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:tim.today@gmail.com                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_snowflake.h"

#include "sid.h"


ZEND_DECLARE_MODULE_GLOBALS(snowflake)


/* True global resources - no need for thread safety here */
static int le_snowflake;

/* {{{ PHP_INI
 */
 
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("snowflake.server_id","1", PHP_INI_ALL, OnUpdateLong, snowflake_server_id, zend_snowflake_globals, snowflake_globals)
    //STD_PHP_INI_ENTRY("snowflake.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_snowflake_globals, snowflake_globals)
PHP_INI_END()
 
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_snowflake_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_snowflake_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "snowflake", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(sid)
{
	if (zend_parse_parameters_none() == FAILURE)
	{
		return;
	}
	long datacenterId = SNOWFLAKE_G(snowflake_server_id);
	set_workid(get_pid());
	set_datacenterId(datacenterId);
	uint64_t unquie;
	unquie = get_twitter_id();
	RETURN_LONG(unquie);
}
PHP_FUNCTION(snowflake_serverid)
{
	if (zend_parse_parameters_none() == FAILURE)
	{
		return;
	}
	long datacenterId = SNOWFLAKE_G(snowflake_server_id);
	RETURN_LONG(datacenterId);
}
/* {{{ php_snowflake_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_snowflake_init_globals(zend_snowflake_globals *snowflake_globals)
{
	snowflake_globals->global_value = 0;
	snowflake_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(snowflake)
{
 
	REGISTER_INI_ENTRIES();
 
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(snowflake)
{
 
	UNREGISTER_INI_ENTRIES();
 
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(snowflake)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(snowflake)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(snowflake)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "snowflake support", "enabled");
	php_info_print_table_row(2, "author", "Tim.wang");
	php_info_print_table_row(2, "version", PHP_SNOWFLAKE_VERSION);
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ snowflake_functions[]
 *
 * Every user visible function must have an entry in snowflake_functions[].
 */
const zend_function_entry snowflake_functions[] = {
	PHP_FE(confirm_snowflake_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(sid, NULL)
	PHP_FE(snowflake_serverid, NULL)
	PHP_FE_END	/* Must be the last line in snowflake_functions[] */
};
/* }}} */

/* {{{ snowflake_module_entry
 */
zend_module_entry snowflake_module_entry = {
	STANDARD_MODULE_HEADER,
	"snowflake",
	snowflake_functions,
	PHP_MINIT(snowflake),
	PHP_MSHUTDOWN(snowflake),
	PHP_RINIT(snowflake),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(snowflake),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(snowflake),
	PHP_SNOWFLAKE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SNOWFLAKE
ZEND_GET_MODULE(snowflake)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
