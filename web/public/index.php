<?php

require __DIR__ . '/../src/Oki/Config/SplClassLoader.php';
require __DIR__ . '/../src/Oki/Config/Config.php';

$loader = new SplClassLoader('Oki', __DIR__ . '/../src');
$loader->register();

$home = new \Oki\Controller\HomeController();
$api = new \Oki\Controller\ApiController();

$router = new \Oki\Router\Router($_SERVER['REQUEST_URI'], $_SERVER['REQUEST_METHOD']);
$router->get('/^api\/list$/', [$api, 'listPackages']);
$router->get('/^$/', [$home, 'index']);
$router->run(new \Oki\DI\DI())->render(__DIR__ . '/../views');
