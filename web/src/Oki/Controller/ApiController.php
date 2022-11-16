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
            return new JsonResponse(404, ['error' => 'Unknown package name']);
        }
        return new JsonResponse(200, $package);
    }
}
