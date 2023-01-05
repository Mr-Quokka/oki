<?php

declare(strict_types=1);

namespace Oki\Security;

final class HttpAuth
{
    public static function authRequest(Security $security): bool
    {
        if (!empty($_SERVER['PHP_AUTH_USER']) && !empty($_SERVER['PHP_AUTH_PW'])) {
            return $security->initLogin($_SERVER['PHP_AUTH_USER'], $_SERVER['PHP_AUTH_PW']);
        }
        return true;
    }
}
