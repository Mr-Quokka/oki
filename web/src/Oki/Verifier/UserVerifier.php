<?php 

declare(strict_types=1);

namespace Oki\Verifier;

class UserVerifier{

    public function checking(string $login, string $password, string $password_confirmation): bool
    {
        if($password != $password_confirmation){
            echo '<p style="color: red">Passwords are not same</p>';
            return false;
        }
        else {
            return true;
        }
	}
}

?>