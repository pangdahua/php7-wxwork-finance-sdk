/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
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

#ifndef PHP_WXWORK_FINANCE_SDK_H
#define PHP_WXWORK_FINANCE_SDK_H

extern zend_module_entry wxwork_finance_sdk_module_entry;
#define phpext_wxwork_finance_sdk_ptr &wxwork_finance_sdk_module_entry

#define PHP_WXWORK_FINANCE_SDK_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_WXWORK_FINANCE_SDK_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_WXWORK_FINANCE_SDK_API __attribute__ ((visibility("default")))
#else
#	define PHP_WXWORK_FINANCE_SDK_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(wxwork_finance_sdk)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(wxwork_finance_sdk)
*/

/* Always refer to the globals in your function as WXWORK_FINANCE_SDK_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define WXWORK_FINANCE_SDK_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(wxwork_finance_sdk, v)

#if defined(ZTS) && defined(COMPILE_DL_WXWORK_FINANCE_SDK)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"
#include "WeWorkFinanceSdk_C.h"

#define TRACE(fmt, ...) do { trace(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); } while (0)

static inline void trace(const char *file, int line, const char* function, const char *fmt, ...) {
    fprintf(stderr, "%s(%s:%d) - ", function, file, line);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

#endif	/* PHP_WXWORK_FINANCE_SDK_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
