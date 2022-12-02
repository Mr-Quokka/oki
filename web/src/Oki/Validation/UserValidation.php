<?php

declare(strict_types=1);

namespace Oki\Validation;

final class UserValidation
{
    public static function isValidAuth(string $login, string $password, string $passwordConfirmation): bool
    {
        return $password === $passwordConfirmation;
    }
}
