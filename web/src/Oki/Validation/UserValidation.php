<?php

declare(strict_types=1);

namespace Oki\Validation;

final class UserValidation
{
    public static function isValidLogin(array $post): bool
    {
        return !(empty($post['login']) || empty($post['password']));
    }

    public static function isValidUser(array $post): bool
    {
        return !(empty($post['login']) || empty($post['password']) || empty($post['password_confirmation']) || $post['password'] != $post['password_confirmation']);
    }
}
