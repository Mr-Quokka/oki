<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;
use Oki\Http\RedirectResponse;
use Oki\Model\User;
use Oki\Validator\UserValidator;

class UserController
{
    public function login(DI $di): HttpResponse
    {
        $errors = [];
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && UserValidator::isValidLogin($_POST, $errors)) {
            $success = $di->getSecurity()->initLogin($_POST['login'], $_POST['password']);
            if ($success) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $errors[] = 'Invalid login or password';
        }
        return new HtmlResponse(200, 'login', ['errors' => $errors]);
    }

    public function register(DI $di): HttpResponse
    {
        $errors = [];
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && UserValidator::isValidUser($_POST,$errors)) {
            $user = $di->getSecurity()->register(User::fromRawPassword($_POST['login'], $_POST['password']));
            if ($user !== null) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $errors[] = 'Login is already taken';
        }
        return new HtmlResponse(200, 'register', ['errors' => $errors]);
    }

    public function changePassword(DI $di): HttpResponse
    {
        $di->getSecurity()->assertLogged();
        $errors = [];
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && UserValidator::isValidPasswordChange($_POST, $errors)) {
            $success = $di->getSecurity()->changeCurrentPassword($di->getSecurity()->getCurrentUser(), $_POST['password'], $_POST['new-password']);
            if ($success) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $errors[] = 'Invalid current password';
        }
        return new HtmlResponse(200, 'change-password', ['errors' => $errors]);
    }

    public function logout(DI $di): HttpResponse
    {
        $di->getSecurity()->logout();
        return new RedirectResponse($di->getRouter()->url(''));
    }
}
