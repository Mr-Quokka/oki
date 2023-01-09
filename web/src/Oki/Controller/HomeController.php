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
