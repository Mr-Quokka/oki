<?php

require __DIR__ . '/src/Oki/Config/SplClassLoader.php';
require __DIR__ . '/src/Oki/Config/Config.php';

$loader = new SplClassLoader('Oki', __DIR__ . '/src');
$loader->register();
