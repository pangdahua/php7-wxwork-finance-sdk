<?php


try {
$obj = new WxworkFinanceSdk("wwd08coe7d775abaaa", "zJ6k0naVVQ--gt9PUSSEvs03zW_nlDVmjAkPOTAfrew", [
    "proxy_password" => "world",
    "timeout" => -2,
]);
// 私钥地址
$privateKey = file_get_contents('private.pem');

$chats = json_decode($obj->getChatData(0, 100), true);
var_dump($chats);
foreach ($chats['chatdata'] as $val) {
    $decryptRandKey = null;
    openssl_private_decrypt(base64_decode($val['encrypt_random_key']), $decryptRandKey, $privateKey, OPENSSL_PKCS1_PADDING);
    $obj->downloadMedia($sdkFileId, "/tmp/download");
}


}catch(\WxworkFinanceSdkException $e) {
    var_dump($e->getMessage(), $e->getCode());
}
