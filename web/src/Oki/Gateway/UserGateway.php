<?php

declare(strict_types=1);

namespace Oki\Gateway;

use PDO;
use Oki\Model\User;

class UserGateway
{
    private PDO $pdo;

    public function __construct(PDO $pdo)
    {
        $this->pdo = $pdo;
    }

    public function getById(int $id): ?User
    {
        $req = $this->pdo->prepare('SELECT * FROM user WHERE id_user = :id');
        $req->execute(['id' => $id]);
        $req->setFetchMode(PDO::FETCH_CLASS, User::class);
        $user = $req->fetch();
        return $user === false ? null : $user;
    }

    public function getByLogin(string $login): ?User
    {
        $req = $this->pdo->prepare('SELECT * FROM user WHERE login = :login;');
        $req->execute(['login' => $login]);
        $req->setFetchMode(PDO::FETCH_CLASS, User::class);
        $user = $req->fetch();
        return $user === false ? null : $user;
    }

    public function insert(User $user)
    {
        $prep = $this->pdo->prepare("INSERT INTO user (login, password, perm) Values(:login, :password, :perm)");
        $prep->execute(['login' => $user->getLogin(), 'password' => $user->getPassword(), 'perm' => $user->getPermissions()]);
    }
}
