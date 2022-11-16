<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HttpResponse;
use Oki\Http\JsonResponse;

class ApiController
{
    public function listPackages(DI $di): HttpResponse
    {
        $packages = $di->getPackageGateway()->listPackages();
        return new JsonResponse(200, ['packages' => $packages]);
    }

    public function packageInfo(DI $di, array $params): HttpResponse
    {
        $packageName = $params['name'];
        $package = $di->getPackageGateway()->getPackageInfo($packageName);
        if ($package === null) {
            return JsonResponse::notFound('Unknown package name');
        }
        return new JsonResponse(200, $package);
    }

    public function versionInfo(DI $di, array $params): HttpResponse
    {
        $packageName = $params['package'];
        if (empty($_GET['version'])) {
            return JsonResponse::badRequest('A version parameter is required');
        }
        $packageVersion = $_GET['version'];
        $package = $di->getPackageGateway()->getPackageVersion($packageName, $packageVersion);
        if ($package === null) {
            return JsonResponse::notFound('Unknown package name or version');
        }
        return new JsonResponse(200, $package);
    }
}
