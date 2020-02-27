<?php

echo "version=", WxworkFinanceSdk::VERSION, PHP_EOL;

try {
$obj = new WxworkFinanceSdk("d", "d", [
    "proxy_host" => "hello",
    "proxy_password" => "world",
    "timeout" => -2,
]);
var_dump($obj);

var_dump(json_decode($obj->getChatData(0, 100)));
}catch(\Exception $e) {
    var_dump($e->getMessage(), $e->getCode());
}