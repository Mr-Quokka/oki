<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\Http\RedirectResponse;
use Oki\Model\User;
use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;
use Oki\Validation\UserValidation;

class UserController
{
    public function login(DI $di): HttpResponse
    {
        $fail = false;
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && !($fail = !UserValidation::isValidLogin($_POST))) {
            $success = $di->getSecurity()->initLogin($_POST['login'], $_POST['password']);
            if ($success) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $fail = !$success;
        }
        return new HtmlResponse(200, 'login', ['fail' => $fail]);
    }

    public function register(DI $di): HttpResponse
    {
        $fail = false;
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && !($fail = !UserValidation::isValidUser($_POST))) {
            $user = $di->getSecurity()->register(User::fromRawPassword($_POST['login'], $_POST['password']));
            if ($user !== null) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $fail = $user === null;
        }
        return new HtmlResponse(200, 'register', ['fail' => $fail]);
    }
}
