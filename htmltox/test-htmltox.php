<?php
$global_settings_pdf = array('out'=>'hdoj.pdf','imageQuantity'=>'92');
$object_settings_pdf = array(
    array('page'=>'acm.hdu.edu.cn'),
    array('page'=>'www.baidu.com'));
$obj = new htmltox();
$obj->htmltopdf($global_settings_pdf,$object_settings_pdf);

$global_settings_image = array('out'=>'hdoj.jpg','in'=>'acm.hdu.edu.cn');
$obj->htmltoimg($global_settings_image);

