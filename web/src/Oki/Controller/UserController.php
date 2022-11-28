<?php

declare(strict_types=1);

namespace Oki\Controller;

use Oki\Verifier\UserVerifier;
use Oki\Model\User;
use Oki\DI\DI;

class UserController {

    private int $defaultPermission = 1;
    private DI $di;

    public function register(String $login, String $password, String $password_confirmation): void
    {
        $verifier = new UserVerifier();
        $user = new User();

        $secure = $this->di->getSecurity();

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
}