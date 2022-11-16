<?php

require __DIR__ . '/../Controller/Database.php';

class UserGateway {
    
    public function getByLogin(string $login):User {
        $db = new Database();

        $req = $db->query('SELECT login FROM user');
    }
}

?>