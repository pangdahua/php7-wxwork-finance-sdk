<?php

echo "version=", WxworkFinanceSdk::VERSION, PHP_EOL;

$obj = new WxworkFinanceSdk("d", "d", [
    "proxy_host" => "hello",
    "proxy_password" => "world"
]);
var_dump($obj);