# HtmlTox
## introduction
- A PHP extension, which convert HTML to pdf and image.
- This extension is based on [wkhtmltopdf](http://wkhtmltopdf.org/) and inspired by [zhtmltopdf](https://github.com/shenzhe/zhtmltopdf)

## Install

### First 
```
git clone https://github.com/KanTian/HtmlTox
```

### Second
```
cd Htmltox
phpize
./configure --with-wkhtmltox-dir=./
make
sudo make install
```
> now,you will find the htmltox.so in `Htmltox/modules/`
> so you can also move it to your own php extension dir manually

### Third
in your php.ini file,add the following line,just as you install other php modules:
```
extension="htmltox.so"
```

### Forth
>php -m

if you can see `htmltox` ,it means that the htmltox module has been successfully installed,enjoy it!

## Usage
### convert html to pdf
```php
<?php
$foo = new htmltox();
$global_settings_pdf = array('out'=>'hdoj.pdf','imageQuantity'=>'92');
$object_settings_pdf = array(
    array('page' => 'acm.hdu.edu.cn'),
    array('useLocalLinks' => true),
    array('useExternalLinks' => true)
);
$foo->htmltopdf($global_settings_pdf,$object_settings_pdf);
```

### convert html to image
```php
<?php
$foo = new htmltox();
$global_settings_image = array('out'=>'hdoj.jpg','in'=>'acm.hdu.edu.cn');
$foo->htmltoimg($global_settings_image);
```

## Important
The above examples just provide a basic usage of this module,in order to make full use of it,you may refer to [wkhtmltopdf docs](http://wkhtmltopdf.org/libwkhtmltox/pagesettings.html) to get more global settings and object settings -:)



