<?php

declare(strict_types=1);

namespace Oki\Model;

class User 
{
    private int $id_user;
    private string $login;
    private string $password;
    private int $permissions;

    public static function fromRawPassword(string $login, string $password, int $permissions = 0): User
    {
        $user = new User();
        $user->login = $login;
        $user->password = password_hash($password, PASSWORD_DEFAULT);
        $user->permissions = $permissions;
        return $user;
    }

    public function setId(int $id) 
    {
        $this->id_user = $id;
    }

    public function getId(): int 
    {
        return $this->id_user;
    }

    public function setLogin(string $login) 
    {
        $this->login = $login;
    }

    public function getLogin(): string
    {
        return $this->login;
    }

    public function setPassword(string $password) 
    {
        $this->password = password_hash($password,PASSWORD_DEFAULT);
    }

    public function getPassword(): string 
    {
        return $this->password;
    }

    public function setPerm(int $perm) 
    {
        $this->perm = $perm;
    }

    public function getPermissions(): int
    {
        return $this->permissions;
    }
}
