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

    public function publish(DI $di, array $params): HttpResponse
    {
        $manifest = json_decode($_POST['manifest'],true);
        $packageName = $manifest['name'];
        $packageDescription = $manifest['description'];
        $packageVersion = $manifest['version'];
        $packageDependencies = $manifest['dependencies'];

        /*if(empty($packageName)){
            return JsonResponse::badRequest('A name parameter is required');
        }
        if (empty($packageDescription)){
            return JsonResponse::badRequest('A description parameter is required');
        }
        if (empty($packageVersion)){
            return JsonResponse::badRequest('A version parameter is required');
        }

        if (!preg_match('^[a-zA-Z0-9_-]+$', $params['name'])){
            return JsonResponse::badRequest('Unmatched name format');
        }
        if (!preg_match('^[a-zA-Z0-9 _-]+$', $params['description'])){
            return JsonResponse::badRequest('Unmatched description format');
        }*/ # aller voir publish.php
        # $_POST -> ['manifest'] -> (validation +) $var = json_decode()
        # tab[3][1]('name' - packageReference, 'version' - packageVersion, 'dependencies' - [^<>])

        $packageInfo = $di->getPackageGateway()->getPackageInfo($packageName);

        if($packageInfo===null){
            return JsonResponse::notFound("Unknown package name");
        }

        $packageId = $packageInfo->getId();
        switch($di->getPackageGateway()->insertVersion($packageId, $packageDescription, $packageVersion)){
            
            case 200:
                return new JsonResponse(201, "Version successfully published");

            case 409:
                return new JsonResponse(409, "Conflict with another version");
        
            default:
                return new JsonResponse(500, "Version not published");
        }
    }
}
