<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HttpResponse;
use Oki\Http\JsonResponse;
use Oki\Validator\PaginationValidator;
use Oki\Validator\PublicationValidator;
use Oki\Security\HttpAuth;

class ApiController
{
    public function listPackages(DI $di): HttpResponse
    {
        $limit = PaginationValidator::getLimit($_GET);
        $page = PaginationValidator::getPage($_GET);
        $total = $di->getPackageGateway()->getCount();
        $packages = $di->getPackageGateway()->listPackagesPagination($limit, $page);
        return new JsonResponse(200, [
            'pagination' => [
                'count' => count($packages),
                'total' => $total
            ],
            'packages' => $packages
        ]);
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
        if (!HttpAuth::authRequest($di->getSecurity())) {
            return JsonResponse::error(401, 'Invalid credentials');
        }
        $user = $di->getSecurity()->getCurrentUser();
        if ($user === null) {
            return JsonResponse::authNeeded('Authentification needed');
        }
        $errors = [];
        $manifest = PublicationValidator::validateJson($_POST, $errors);
        if (!empty($errors)) {
            return JsonResponse::badRequest($errors[0]);
        }
        if (!PublicationValidator::validateVersionContent($_FILES, $errors)) {
            return JsonResponse::badRequest($errors[0]);
        }
        $packageRes = $di->getPackageGateway()->getOrCreatePackage($user, $manifest);
        if ($packageRes->isError()) {
            return $packageRes->asJson();
        }
        $packageId = $packageRes->getDataId();
        if (!$di->getOwnershipGateway()->grantedAccess($user->getId(), $packageId)) {
            return JsonResponse::error(403, 'Insufficient permissions');
        }
        $manifest->setPackageId($packageId);

        $res = $di->getPackageGateway()->insertVersion($manifest);
        if ($res->isSuccess()) {
            move_uploaded_file(
                $_FILES['package']['tmp_name'],
                __DIR__ . '/../../../public/packages/' . $manifest->getName() . '_' . $manifest->getVersion() . '.zip'
            );
            $res->validate();
        }
        return $res->asJson();
    }
}
