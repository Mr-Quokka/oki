<?php

require __DIR__ . '/../Model/User.php';
require __DIR__ . '/../Model/UserGateway.php';

class Security {
    
    public function initLogin(User $user) {
        return true;
    }
}

?>