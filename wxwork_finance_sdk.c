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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_wxwork_finance_sdk.h"

/* If you declare any globals in php_wxwork_finance_sdk.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(wxwork_finance_sdk)
*/

/* True global resources - no need for thread safety here */
static int le_wxwork_finance_sdk;

static zend_class_entry *wxwork_finance_sdk_ce;
static zend_class_entry *wxwork_finance_sdk_exception_ce;

static WeWorkFinanceSdk_t *sdk;

/**
* options = [
      'proxy_host' => 'http://www.baidu.com',
      'proxy_password' => 'helloworld'
  ]
*/
PHP_METHOD(WxworkFinanceSdk, __construct)
{
    char *corp_id, *secret;
    size_t corp_id_len, secret_len;
    zval *option_zval = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|a", &corp_id, &corp_id_len,  &secret, &secret_len, &option_zval) == FAILURE) {
        zend_error(E_ERROR, "param error");
        return;
    }

    if (corp_id_len == 0 || secret_len == 0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "corpId and secret cannot be bull", 0);
        return;
    }

    int ret = Init(sdk, corp_id, secret);

    if (ret != 0) {
        zend_throw_exception(wxwork_finance_sdk_exception_ce, "Call WeWorkFinanceSdk_t Init error", ret);
        return;
    }

    zval *this = getThis();
    zend_class_entry *ce = Z_OBJCE_P(this);

    zend_update_property_string(ce, this, "_corpId", sizeof("_corpId") - 1, corp_id);
    zend_update_property_string(ce, this, "_secret", sizeof("_secret") - 1, secret);

    if (option_zval) {
        zval *proxy_host_zval = zend_hash_find(Z_ARR_P(option_zval), zend_string_init("proxy_host", sizeof("proxy_host") - 1, 0));

        if (proxy_host_zval != NULL) {
            zval *proxy_password_zval = zend_hash_find(Z_ARR_P(option_zval), zend_string_init("proxy_password", sizeof("proxy_password") - 1, 0));

            zend_update_property_string(ce, this, "_proxy_host", sizeof("_proxy_host") - 1, Z_STRVAL_P(proxy_host_zval));
            if (proxy_password_zval != NULL) {
                zend_update_property_string(ce, this, "_proxy_password", sizeof("_proxy_password") - 1, Z_STRVAL_P(proxy_password_zval));
            }
        }

        zval *timeout_zval = zend_hash_find(Z_ARR_P(option_zval), zend_string_init("timeout", sizeof("timeout") - 1, 0));
        if (timeout_zval != NULL) {
            zend_update_property_long(ce, this, "_timeout", sizeof("_timeout") - 1, zval_get_long(timeout_zval));
        }
    }
}
/**
    {{{ proto public WxworkFinanceSdk::getChatData(int $seq, int $limit)
*/

PHP_METHOD(WxworkFinanceSdk, getChatData)
{
    zend_long seq, limit = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &seq, &limit) == FAILURE) {
        return;
    }

    Slice_t *chat_data = NewSlice();

    if (NULL == chat_data) {
        zend_error(E_ERROR, "There is not enough  memory!");
        return;
    }

    zval *this = getThis();
    zend_class_entry *ce = Z_OBJCE_P(this);

    zval *proxy_host_zval = zend_read_property(ce, this, "_proxy_host", sizeof("_proxy_host") - 1, 0, NULL);
    zval *proxy_password_zval = zend_read_property(ce, this, "_proxy_password", sizeof("_proxy_password") - 1, 0, NULL);
    zval *timeout_zval = zend_read_property(ce, this, "_timeout", sizeof("_timeout") - 1, 0, NULL);
    int ret = GetChatData(sdk, (int)seq, (int)limit, Z_STRVAL_P(proxy_host_zval), Z_STRVAL_P(proxy_password_zval), zval_get_long(timeout_zval), chat_data);
    if (0 != ret) {
        zend_throw_exception(wxwork_finance_sdk_exception_ce, "Call WeWorkFinanceSdk_t GetChatData error", ret);
        return;
    }

    zend_string *s = zend_string_init(GetContentFromSlice(chat_data), GetSliceLen(chat_data), 0);
    RETURN_STR(s);

    zend_string_release(s);
    zval_ptr_dtor(proxy_host_zval);
    zval_ptr_dtor(proxy_password_zval);
    zval_ptr_dtor(timeout_zval);
    FreeSlice(chat_data);
}
/* }}} */

/**
    {{{ proto WxworkFinanceSdk->decryptData(string $encryptRandomKey, string $encryptData)
*/
PHP_METHOD(WxworkFinanceSdk, decryptData)
{
    zend_string *encrypt_random_key, *encrypt_data;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &encrypt_random_key, &encrypt_data) == FAILURE) {
        return;
    }

    Slice_t *msg = NewSlice();

    int ret = DecryptData(ZSTR_VAL(encrypt_random_key), ZSTR_VAL(encrypt_data), msg);
    if (ret != 0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "DecryptData data error", ret);
        return;
    }

    zend_string *return_msg = zend_string_init(GetContentFromSlice(msg), GetSliceLen(msg), 0);

    RETURN_STR(return_msg);
    FreeSlice(msg);
    zend_string_release(return_msg);
}

/**
    {{{ proto WxworkFinanceSdk->getMediaData(string $filedId, string $index='')
*/

PHP_METHOD(WxworkFinanceSdk, downloadMedia)
{
    zend_string *sdk_filedid, *file_saveto;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &sdk_filedid, &file_saveto) == FAILURE) {
        return;
    }

    zval *this = getThis();
    zend_class_entry *ce = Z_OBJCE_P(this);

    zval *proxy_host_zval = zend_read_property(ce, this, "_proxy_host", sizeof("_proxy_host") - 1, 0, NULL);
    zval *proxy_password_zval = zend_read_property(ce, this, "_proxy_password", sizeof("_proxy_password") - 1, 0, NULL);
    zval *timeout_zval = zend_read_property(ce, this, "_timeout", sizeof("_timeout") - 1, 0, NULL);

    FILE *fp = fopen(ZSTR_VAL(file_saveto), "wb");
    if (NULL == fp) {
	    zend_throw_exception(wxwork_finance_sdk_exception_ce, "Open", 0);
	    return;
    }

    MediaData_t *media_data = NewMediaData();
    if (NULL == media_data) {
         zend_error(E_ERROR, "There is not enough  memory!");
         return;
    }

    do {
        int ret = GetMediaData(sdk, GetOutIndexBuf(media_data), ZSTR_VAL(sdk_filedid), Z_STRVAL_P(proxy_host_zval), Z_STRVAL_P(proxy_password_zval), zval_get_long(timeout_zval), media_data);

        if (0 != ret) {
	    FreeMediaData(media_data);
            zend_throw_exception(wxwork_finance_sdk_exception_ce, "GetMediaData error", ret);
            return;
        }
        fwrite(GetData(media_data), GetDataLen(media_data), 1, fp);
    }while(IsMediaDataFinish(media_data) != 1);

    fclose(fp);
    FreeMediaData(media_data);

    RETURN_TRUE;
}

/* }}} */

static const zend_function_entry wxwork_finance_sdk_class_methods[] = {
    PHP_ME(WxworkFinanceSdk, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(WxworkFinanceSdk, getChatData, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(WxworkFinanceSdk, decryptData, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(WxworkFinanceSdk, downloadMedia, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry wxwork_finance_sdk_exception_methods[] = {
    PHP_FE_END
};


/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("wxwork_finance_sdk.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_wxwork_finance_sdk_globals, wxwork_finance_sdk_globals)
    STD_PHP_INI_ENTRY("wxwork_finance_sdk.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_wxwork_finance_sdk_globals, wxwork_finance_sdk_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_wxwork_finance_sdk_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_wxwork_finance_sdk_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "wxwork_finance_sdk", arg);

	RETURN_STR(strg);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_wxwork_finance_sdk_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_wxwork_finance_sdk_init_globals(zend_wxwork_finance_sdk_globals *wxwork_finance_sdk_globals)
{
	wxwork_finance_sdk_globals->global_value = 0;
	wxwork_finance_sdk_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(wxwork_finance_sdk)
{
	 /* If you have INI entries, uncomment these lines
	 REGISTER_INI_ENTRIES();
	 */

    // define WxworkFinanceSdkException
    zend_class_entry wxwork_finance_sdk_exception_def;
    INIT_CLASS_ENTRY(wxwork_finance_sdk_exception_def, "WxworkFinanceSdkException", wxwork_finance_sdk_exception_methods);
    wxwork_finance_sdk_exception_ce = zend_register_internal_class_ex(&wxwork_finance_sdk_exception_def, zend_ce_exception);

    zend_class_entry wxwork_finance_sdk_def;
    INIT_CLASS_ENTRY(wxwork_finance_sdk_def, "WxworkFinanceSdk", wxwork_finance_sdk_class_methods);
    wxwork_finance_sdk_ce = zend_register_internal_class(&wxwork_finance_sdk_def);

    zend_declare_property_string(wxwork_finance_sdk_ce, "_corpId", sizeof("_corpId") - 1, "", ZEND_ACC_PRIVATE);
    zend_declare_property_string(wxwork_finance_sdk_ce, "_secret", sizeof("_secret") - 1, "", ZEND_ACC_PRIVATE);
    // http proxy
    zend_declare_property_string(wxwork_finance_sdk_ce, "_proxy_host", sizeof("_proxy_host") - 1, "", ZEND_ACC_PRIVATE);
    zend_declare_property_string(wxwork_finance_sdk_ce, "_proxy_password", sizeof("_proxy_password") - 1, "", ZEND_ACC_PRIVATE);
    // request timeout
    zend_declare_property_long(wxwork_finance_sdk_ce, "_timeout", sizeof("_timeout") - 1, 10, ZEND_ACC_PRIVATE);

    return SUCCESS;
}

/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(wxwork_finance_sdk)
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
PHP_RINIT_FUNCTION(wxwork_finance_sdk)
{
#if defined(COMPILE_DL_WXWORK_FINANCE_SDK) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

    sdk = NewSdk();

    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(wxwork_finance_sdk)
{
    DestroySdk(sdk);
    
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wxwork_finance_sdk)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "wxwork_finance_sdk support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */



/* {{{ wxwork_finance_sdk_functions[]
 *
 * Every user visible function must have an entry in wxwork_finance_sdk_functions[].
 */
const zend_function_entry wxwork_finance_sdk_functions[] = {
	PHP_FE(confirm_wxwork_finance_sdk_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in wxwork_finance_sdk_functions[] */
};
/* }}} */

/* {{{ wxwork_finance_sdk_module_entry
 */
zend_module_entry wxwork_finance_sdk_module_entry = {
	STANDARD_MODULE_HEADER,
	"wxwork_finance_sdk",
	wxwork_finance_sdk_functions,
	PHP_MINIT(wxwork_finance_sdk),
	PHP_MSHUTDOWN(wxwork_finance_sdk),
	PHP_RINIT(wxwork_finance_sdk),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(wxwork_finance_sdk),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(wxwork_finance_sdk),
	PHP_WXWORK_FINANCE_SDK_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WXWORK_FINANCE_SDK
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(wxwork_finance_sdk)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
