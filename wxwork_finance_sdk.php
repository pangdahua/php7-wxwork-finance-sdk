<?php

echo "version=", WxworkFinanceSdk::VERSION, PHP_EOL;

try {
$obj = new WxworkFinanceSdk("wwd08coe7d775abaaa", "zJ6k0naVVQ--gt9PUSSEvs03zW_nlDVmjAkPOTAfrew", [
    "proxy_host" => "hello",
    "proxy_password" => "world",
    "timeout" => -2,
]);
var_dump($obj);

var_dump(json_decode($obj->getChatData(0, 100)));
var_dump($obj->getMediaData("dddd"));


}catch(\Exception $e) {
    var_dump($e->getMessage(), $e->getCode());
}