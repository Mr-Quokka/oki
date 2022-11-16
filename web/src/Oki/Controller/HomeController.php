<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;

class HomeController
{
    public function index(DI $di): HttpResponse
    {
        $packages = $di->getPackageGateway()->listPackages();
        return new HtmlResponse(200, 'index', ['packages' => $packages]);
    }

    public function packageInfo(DI $di, array $params): HttpResponse
    {
        $packageName = $params['name'];
        $package = $di->getPackageGateway()->getPackageInfo($packageName);
        if ($package === null) {
            return HtmlResponse::notFound('Unknown package name');
        }
        return new HtmlResponse(200, 'package', ['package' => $package]);
    }
}
