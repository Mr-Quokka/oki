<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\Model\User;
use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;
use Oki\Validation\UserValidation;

const DEFAULT_PERMISSIONS = 1;

class UserController
{
    public function insertUser(string $login, string $password, string $password_confirmation, DI $di): void
    {
        $user = new User();

        $secure = $di->getSecurity();

        if (UserValidation::isValidAuth($login, $password, $password_confirmation) != false) {

            $user->setLogin($login);
            $user->setPassword($password);
            $user->setPerm($this->defaultPermission);

            $secure->register($user);
        } else {
            return;
        }
    }

    public function login(DI $di): HttpResponse
    {
        $fail = false;
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $success = $di->getSecurity()->initLogin($_POST['login'], $_POST['password']);
            if ($success) {
                //HttpResponse::redirect($di->getRouter()->url(''));
            }
            $fail = !$success;
        }
        return new HtmlResponse(200, 'login', ['fail' => $fail]);
    }

    public function register(DI $di): HttpResponse
    {
        $fail = false;
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $user = $di->getSecurity()->register(User::fromRawPassword($_POST['login'], $_POST['password']));
            if ($user !== null) {
                //HttpResponse::redirect($di->getRouter()->url(''));
            }
            $fail = $user === null;
        }
        return new HtmlResponse(200, 'register', ['fail' => $fail]);
    }
}
