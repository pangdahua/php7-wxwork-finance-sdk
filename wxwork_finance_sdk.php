<?php

echo "version=", WxworkFinanceSdk::VERSION, PHP_EOL;


try {
$obj = new WxworkFinanceSdk("wwd08coe7d775abaaa", "zJ6k0naVVQ--gt9PUSSEvs03zW_nlDVmjAkPOTAfrew", [
    "proxy_host" => "hello",
    "proxy_password" => "world",
    "timeout" => -2,
]);
// 私钥地址
$privateKey = file_get_contents('private.pem');

$chats = json_decode($obj->getChatData(0, 100), true);

foreach ($chats['chatdata'] as $val) {
    $decryptRandKey = null;
    openssl_private_decrypt(base64_decode($val['encrypt_random_key']), $decryptRandKey, $privateKey, OPENSSL_PKCS1_PADDING);
    $j = json_decode($obj->decryptData($decryptRandKey, $val['encrypt_chat_msg']));
    var_dump($j);exit();
}


}catch(\WxworkFinanceSdkException $e) {
    var_dump($e->getMessage(), $e->getCode());
}