<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;

class HomeController
{
    private const PACKAGES_PER_PAGE = 10;

    public function index(DI $di, array $params): HttpResponse
    {
        $nbPages = intval(ceil($di->getPackageGateway()->getCount() / self::PACKAGES_PER_PAGE));
        if ($nbPages === 0){
            $nbPages = 1;
        }
        $page = intval($params['page'] ?? '1');
        $packages = $di->getPackageGateway()->listPackagesPagination(self::PACKAGES_PER_PAGE, $page);
        return new HtmlResponse(200, 'index', ['packages' => $packages, 'page' => $page, 'nbPages' => $nbPages]);
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
