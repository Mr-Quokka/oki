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
}
