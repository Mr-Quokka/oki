<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;
use Oki\Router\RouteNotFoundException;
use Oki\Router\Router;
use Oki\Security\SecurityException;

class FrontController
{
    public static function run(Router $router, DI $di)
    {
        $viewPath = __DIR__ . '/../../../views';
        try {
            $router->run($di)->render($di, $viewPath);
        } catch (SecurityException | RouteNotFoundException $e) {
            (new HtmlResponse(HttpResponse::NOT_FOUND, "404")) // Ne donne pas d'information sur les routes privées
                ->render($di, $viewPath);
        }
    }
}
