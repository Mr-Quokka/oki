<?php 

declare(strict_types=1);

namespace Oki\Verifier;

class UserVerifier{

    public function checking(String $login, String $password, String $password_confirmation): bool
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