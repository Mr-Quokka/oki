<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\Verifier\UserVerifier;
use Oki\Model\User;
use Oki\DI\DI;
use Oki\Http\HtmlResponse;
use Oki\Http\HttpResponse;

class UserController {

    private int $defaultPermission = 1;

    public function insertUser(String $login, String $password, String $password_confirmation, DI $di): void
    {
        $verifier = new UserVerifier();
        $user = new User();

        $secure = $di->getSecurity();

        if($verifier->checking($login, $password, $password_confirmation) != false){

            $user->setLogin($login);
            $user->setPassword($password);
            $user->setPerm($this->defaultPermission);
    
            $secure->register($user);
        }
        else {
            return ;
        }
    }

    public function register(DI $di): HttpResponse{

        $errors = [];

        if ($_SERVER['REQUEST_METHOD'] === 'POST') {

            print_r($_POST);

            if (empty($_POST['login']) || empty($_POST['password']) || empty($_POST['password_confirmation'])) {
                $html_code = 400;
                $errors[] = 'Fields are empty';
            } else {
                $login = $_POST['login'];
                $password = $_POST['password'];
                $password_confirmation = $_POST['password_confirmation'];

                if($this->insertUser($login, $password, $password_confirmation, $di) != 200){ 
                    $html_code = 400;
                    $errors[] = 'Failed to register';
                }
            }
            if($html_code == 200 || $html_code == 400){
                return new HtmlResponse($html_code, 'register',['register' => $errors]);
            }
        }
        return new HtmlResponse(200, 'register');
    }
}