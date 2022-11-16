<?php

class User {

    private int $id;
    private string $login;
    private string $password;
    private int $perm;

    public function setId(int $id) {
        $this->id = $id;
    }

    public function getId(): int {
        return $this->id;
    }

    public function setLogin(string $login) {
        $this->login = $login;
    }

    public function getLogin(): string{
        return $this->login;
    }

    public function setPassword(string $password) {
        $this->password = $password;
    }

    public function getPassword(): string {
        return $this->password;
    }

    public function setPerm(int $perm) {
        $this->perm = $perm;
    }

    public function getPerm(): int {
        return $this->perm;
    }
}


?>