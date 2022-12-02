<?php

declare(strict_types=1);

namespace Oki\Model;

class User 
{
    private int $id;
    private string $login;
    private string $password;
    private int $permissions;

    public function setId(int $id) 
    {
        $this->id = $id;
    }

    public function getId(): int 
    {
        return $this->id;
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
