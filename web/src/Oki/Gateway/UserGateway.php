<?php

declare(strict_types=1);

namespace Oki\Gateway;

use Oki\Config\DatabaseConfig;
use PDO;
use Oki\Model\User;

class UserGateway
{
    private DatabaseConfig $config;
    private PDO $pdo;

    public function __construct(PDO $pdo, DatabaseConfig $config)
    {
        $this->config = $config;
        $this->pdo = $pdo;
    }

    public function getById(int $id): ?User
    {
        $req = $this->pdo->prepare('SELECT * FROM registered_user WHERE id_user = :id');
        $req->execute(['id' => $id]);
        $req->setFetchMode(PDO::FETCH_CLASS, User::class);
        $user = $req->fetch();
        return $user === false ? null : $user;
    }

    public function getByLogin(string $login): ?User
    {
        $req = $this->pdo->prepare('SELECT * FROM registered_user WHERE login = :login;');
        $req->execute(['login' => $login]);
        $req->setFetchMode(PDO::FETCH_CLASS, User::class);
        $user = $req->fetch();
        return $user === false ? null : $user;
    }

    public function insert(User $user): bool
    {
        $req = $this->pdo->prepare("INSERT INTO registered_user (login, password, permissions) VALUES(:login, :password, :permissions);");
        try {
            $req->execute(['login' => $user->getLogin(), 'password' => $user->getPassword(), 'permissions' => $user->getPermissions()]);
        } catch (\PDOException $ex) {
            if ($this->config->isUniqueConstraintViolation($ex)) {
                return false;
            }
            throw $ex;
        }
        $user->setId(intval($this->pdo->lastInsertId()));
        return true;
    }

    public function update(User $user): bool
    {
        $req = $this->pdo->prepare("UPDATE registered_user SET password = :password, permissions = :permissions WHERE login = :login;");
        $req->execute([
            'login' => $user->getLogin(),
            'password' => $user->getPassword(),
            'permissions' => $user->getPermissions()
        ]);
        return true;
    }
}
