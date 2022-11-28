<?php

declare(strict_types=1);

namespace Oki\Gateway;

use PDO;
use Oki\Model\User;


class UserGateway {

    private PDO $pdo;

    public function __construct(PDO $pdo)
    {
        $this->pdo = $pdo;
    }

    public function getByLogin(string $login): ?User 
    {
        $prep = $this->pdo->prepare("SELECT * FROM user WHERE login = :login");
        $prep->execute(['login' => $login]);
        $prep->setFetchMode(PDO::FETCH_CLASS, User::class);

        $user = $prep->fetch();

        if($user === false) {
            return null;
        } else {
            return $user;
        }
    }

    public function insert(User $user) 
    {
        $prep = $this->pdo->prepare("INSERT INTO user (username,password) Values(:login,:password)");
        $prep->execute(['login' => $user->getLogin(), 'password' => $user->getPassword()]);
    }
}
