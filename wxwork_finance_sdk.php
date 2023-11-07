<?php
gc_enable();
$startMemUsed = memory_get_usage();
function cal_mem()
{
        global $startMemUsed;
        echo "memory,", ceil((memory_get_usage() - $startMemUsed) / 1024), 'KB', PHP_EOL;
}
echo "start mypid=", getmypid(), PHP_EOL;
sleep(2);
$sdk = new WxworkFinanceSdk("1", "2");
$privateKey = <<<EOF
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
EOF;
$seq = 0;
$str = str_repeat('X', 1024 * 1024);

do{

        echo "sync seq={$seq} ==>", cal_mem(), PHP_EOL;
        gc_mem_caches();
        gc_collect_cycles();
        $wxChat = $sdk->getChatData($seq, 100);
        $chats = json_decode($wxChat, true);
        $chatRows = $chats['chatdata'];
        foreach ($chatRows as $val) {
                $decryptRandKey = null;
                $decryptData = openssl_private_decrypt(base64_decode($val['encrypt_random_key']), $decryptRandKey, $privateKey, OPENSSL_PKCS1_PADDING);
                $decryptChatRawContent = $sdk->decryptData($decryptRandKey, $val['encrypt_chat_msg']);
                $j2 = json_decode($decryptChatRawContent, true);
                $msgType = $j2['msgtype'];
                     if (in_array($msgType, ['image', 'video'])) {
                        try {
                                $sdk->downloadMedia($j2[$msgType]['sdkfileid'], "/tmp/download/{$j2[$msgType]['md5sum']}");
                        }catch(\Exception $e) {
                                var_dump($e);
                                var_dump($e->getMessage(), $e->getCode());
                                sleep(1);
                        }
                }
                unset($decryptRandKey);
        }
        echo "loop done ===>", cal_mem();
        unset($chatRows, $wxChat, $chats);
        gc_collect_cycles();gc_collect_cycles();
        $seq = $val['seq'];
}while(true);
cal_mem();