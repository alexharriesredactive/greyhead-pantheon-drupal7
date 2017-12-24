<?php
$libraries['jquery-doubletaptogo'] = array(
    'name' => 'jQuery doubletaptogo',
    'vendor url' => 'https://github.com/dachcom-digital/jquery-doubletaptogo',
    'download url' => 'https://github.com/dachcom-digital/jquery-doubletaptogo/releases/latest',
    'version arguments' => array(
        'file' => 'dist/jquery.dcd.doubletaptogo.js',
        'pattern' => '/@version ([0-9a-zA-Z\.-]+)/',
    ),
    'files' => array(
        'js' => array(
            'dist/jquery.dcd.doubletaptogo.js'
        ),
    ),
    'variants' => array(
        'minified' => array(
            'files' => array(
                'js' => array(
                    'dist/jquery.dcd.doubletaptogo.min.js'
                ),
            ),
        ),
    ),
);
