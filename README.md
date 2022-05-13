# wxwork_finance_sdk_wrapper

企业微信-会话内容存档PHP扩展


## 依赖
企业微信提供的sdk;

PHP VERSION >= 7.0

openssl扩展

## 安装步骤及要求
```
       $INSATLL_PHP_PATH/bin/phpize
        
       ./configure --with-php-config=$INSTALL_PHP_PATH/php-config --with-wxwork-finance-sdk=$WXWORK_FINANCE_C_SDK_PATH
       
        make && make install
```
    php.ini 增加 extension=wxwork_finance_sdk.so
    
## API
```php
    WxworkFinanceSdkExcption::__construct();
```

```php
    WxworkFinanceSdk::__construct(string $corpId, string $secret, array $options);
    string $corpId 企业号

    string $secret 秘钥

    array $options = [ // 可选参数
        'proxy_host' => string,
        'proxy_password' => string,
        'timeout' => 10, // 默认超时时间为10s
    ]
```

```php
   string WxworkFinanceSdk::getChatData(int $seq, int $limit);
    * 拉取聊天数据
    $seq 起始位置
    $limit 获取条数
``` 

```php
   bool WxworkFinanceSdk::downloadMedia(string $sdkfileid, string $saveTo)
   * 下载资源
   $sdkfileid 资源id。来自chat 中的数据sdkfileid
   $saveTo 本地保存的路径
```

```php
   array WxworkFinanceSdk::getMediaData(string $sdkfileid, string $indexBuf='');
   * 拉取静态资源数据，用于可以支持追加模式的三方存储平台
   返回的数据结构体
   $ret = [
       'data' => '' // string 返回的数据
       'nextIndex' => 'ddd' // string 获取下一段数据的句柄
       'isFinished' => int // 1 数据已拉取完毕 
   ];
   
   
```
* 感谢 @zhanzhaopeng1 PR


```php
       string WxworkFinanceSdk::decryptData(string $randomKey, string $encryptStr);
       * 解密数据
       $randomKey 通过openssl解密后的key
       $encryptStr chats 的加密数据
```



## 已知问题
      1. free(): invalid pointer
       * 定位intl扩展的冲突问题. php -m |grep intl 建议重新编译php 取消intl扩展
       
      2. 与swoole的扩展冲突
       * 优先加载wxwork_finance_sdk.so 后加载swoole 【@tisswb】
       
 ## 示例
 
  wxwork_finance_sdk.php

 Alipay
 
 <img src="https://github.com/pangdahua/php7-wxwork-finance-sdk/blob/sponsor/imgs/Alipay.png" width="230" height="230" />
