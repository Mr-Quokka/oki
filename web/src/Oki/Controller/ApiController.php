<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HttpResponse;
use Oki\Http\JsonResponse;
use Oki\Validator\PublicationValidator;

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

    public function publish(DI $di): HttpResponse
    {
        $errors = [];
        $manifest = PublicationValidator::validateJson($_POST, $errors);
        if (!empty($errors)) {
            return JsonResponse::badRequest($errors[0]);
        }
        if (!PublicationValidator::validateVersionContent($_FILES, $errors)) {
            return JsonResponse::badRequest($errors[0]);
        }
        $packageId = $di->getPackageGateway()->getPackageId($manifest->getName());
        if ($packageId === null) {
            return JsonResponse::notFound('Unknown package name');
        }
        $manifest->setPackageId($packageId);

        switch ($di->getPackageGateway()->insertVersion($manifest)) {
            case 200:
                move_uploaded_file(
                    $_FILES['package']['tmp_name'],
                    __DIR__ . '/../../../public/packages/' . $manifest->getName() . '_' . $manifest->getVersion() . '.zip'
                );
                return JsonResponse::success(201, 'The version has been successfully published');
            case 409:
                return JsonResponse::conflict('This version already exists');
            default:
                return new JsonResponse(500, 'Version not published');
        }
    }
}
