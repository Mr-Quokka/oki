<?php

declare(strict_types=1);

namespace Oki\Security;

use Oki\Gateway\UserGateway;
use Oki\Model\User;
use Oki\DI\DI;

class Security {

    private UserGateway $userGateway;
    private $_SESSION;

    public function __construct(UserGateway $userGateway, $session)
    {
        $this->userGateway = $userGateway;
        $this->$_SESSION = $session;
    }
    
    public function register(User $user): void
    {
        $this->userGateway->insert($user);
    }

    public function initLogin(string $login, string $password): void
    {
        //Création de la $_SESSION
    }

    public function checkPerms(int $level): void
    {
        //Vérification des permissions de l'utilisateur dans $_SESSION
    }

    public function logout()
    {
        //Destruction de la $_SESSION
    }
}

?>