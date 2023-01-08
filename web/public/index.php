<?php

require __DIR__ . '/../autoload.php';

$home = new \Oki\Controller\HomeController();
$api = new \Oki\Controller\ApiController();
$user = new \Oki\Controller\UserController();

$router = new \Oki\Router\Router($_SERVER['REQUEST_URI'], $_SERVER['REQUEST_METHOD']);
$router->get('/^api\/list$/', [$api, 'listPackages']);
$router->post('/^api\/publish$/', [$api, 'publish']);
$router->get('/^api\/info\/(?<name>[\w-]+)$/', [$api, 'packageInfo']);
$router->get('/^api\/version\/(?<package>[\w-]+)$/', [$api, 'versionInfo']);
$router->get('/^$/', [$home, 'index']);
$router->get('/^list\/(?<page>\d+)$/', [$home, 'index']);
$router->get('/^package\/(?<name>[\w-]+)$/', [$home, 'packageInfo']);
$router->match('/^login$/', [$user, 'login']);
$router->match('/^register$/', [$user, 'register']);
$router->match('/^logout/', [$user, 'logout']);

$di = new \Oki\DI\DI();
$di->setRouter($router);
$router->run($di)->render($di, __DIR__ . '/../views');
