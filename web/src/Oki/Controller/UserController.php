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
        $status = HttpResponse::OK;
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && UserValidator::isValidLogin($_POST, $errors)) {
            $success = $di->getSecurity()->initLogin($_POST['login'], $_POST['password']);
            if ($success) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $errors[] = 'Invalid login or password';
            $status = HttpResponse::UNAUTHORIZED;
        }
        return (new HtmlResponse($status, 'login'))->errors($errors);
    }

    public function register(DI $di): HttpResponse
    {
        $errors = [];
        $status = HttpResponse::OK;
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && UserValidator::isValidUser($_POST,$errors)) {
            $user = $di->getSecurity()->register(User::fromRawPassword($_POST['login'], $_POST['password']));
            if ($user !== null) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $errors[] = 'Login is already taken';
            $status = HttpResponse::CONFLICT;
        }
        return (new HtmlResponse($status, 'register'))->errors($errors);
    }

    public function changePassword(DI $di): HttpResponse
    {
        $di->getSecurity()->assertLogged();
        $errors = [];
        $status = HttpResponse::OK;
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && UserValidator::isValidPasswordChange($_POST, $errors)) {
            $success = $di->getSecurity()->changeCurrentPassword($di->getSecurity()->getCurrentUser(), $_POST['password'], $_POST['new-password']);
            if ($success) {
                return new RedirectResponse($di->getRouter()->url(''));
            }
            $errors[] = 'Invalid current password';
            $status = HttpResponse::UNAUTHORIZED;
        }
        return (new HtmlResponse($status, 'change-password'))->errors($errors);
    }

    public function logout(DI $di): HttpResponse
    {
        $di->getSecurity()->logout();
        return new RedirectResponse($di->getRouter()->url(''));
    }
}
