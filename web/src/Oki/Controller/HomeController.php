<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;
use Oki\Validator\PaginationValidator;

class HomeController
{
    private const PACKAGES_PER_PAGE = 10;

    public function index(DI $di, array $params): HttpResponse
    {
        $search = PaginationValidator::getSearch($_GET);
        $nbPages = intval(ceil($di->getPackageGateway()->getCount() / self::PACKAGES_PER_PAGE));
        if ($nbPages === 0){
            $nbPages = 1;
        }
        $page = intval($params['page'] ?? '1');
        $packages = $di->getPackageGateway()->listPackagesPagination($search, self::PACKAGES_PER_PAGE, $page);
        return new HtmlResponse(200, 'index', ['packages' => $packages, 'page' => $page, 'nbPages' => $nbPages]);
    }

    public function packageInfo(DI $di, array $params): HttpResponse
    {
        $packageName = $params['name'];
        $package = $di->getPackageGateway()->getPackage($packageName);
        if ($package === null) {
            return HtmlResponse::notFound('Unknown package name');
        }
        $action = $params['action'] ?? 'readme';

        $params = [
            'package' => $package,
            'action' => $action
        ];
        if ($action === 'versions') {
            $di->getPackageGateway()->getPackageVersions($package);
            if (!empty($package->getVersions())) {
                $params['version'] = $package->getVersions()[0];
            } else {
                $params['version'] = null;
            }
        } else {
            $version = $di->getPackageGateway()->getPackageVersion($package, $params['version'] ?? null);
            if ($version === null && isset($params['version'])) {
                return HtmlResponse::notFound('Unknown package version');
            }
            $params['version'] = $version;
            if ($version !== null && $action === 'dependencies') {
                $di->getPackageGateway()->getPackageDependencies($version);
            }
        }
        return new HtmlResponse(200, 'package/base', $params);
    }
}
