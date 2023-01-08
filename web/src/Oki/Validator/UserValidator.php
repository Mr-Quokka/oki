<?php

declare(strict_types=1);

namespace Oki\Validator;

final class UserValidator
{
    public static function isValidLogin(array $post, array &$errors): bool
    {
        if (empty($post['login']) || empty(trim($post['login']))) {
            $errors[] = 'Empty login';
        }
        if (empty($post['password'])) {
            $errors[] = 'Empty password';
        }
        return empty($errors);
    }

    public static function isValidUser(array &$post, array &$errors): bool
    {
        if (!self::isValidLogin($post, $errors)) {
            return false;
        }
        if (empty($post['password-confirmation'])) {
            $errors[] = 'Empty password confirmation';
            return false;
        }
        $len = strlen($post['login']);
        if ($len < 4) {
            $errors[] = 'Login must contain at least 4 characters';
        }
        if ($len > 64) {
            $errors[] = 'Login must contain less than 64 characters';
        }
        $post['login'] = htmlspecialchars($post['login']);
        if ($post['password'] !== $post['password-confirmation']) {
            $errors[] = 'Password confirmation must match the original password';
        }
        return empty($errors);
    }
}
