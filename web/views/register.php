<?php
$title = 'Register';
require 'errors.php';
?>
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <div class="field">
        <label class="label" for="login">Login</label>
        <div class="control">
            <input class="input" type="text" id="login" name="login" required minlength="4" maxlength="64" autocomplete="username">
        </div>
    </div>
    <div class="field">
        <label class="label" for="password">Password</label>
        <div class="control">
            <input class="input" type="password" id="password" name="password" required autocomplete="new-password">
        </div>
    </div>
    <div class="field">
        <label class="label" for="password-confirmation">Password confirmation</label>
        <div class="control">
            <input class="input" type="password" id="password-confirmation" name="password-confirmation" autocomplete="new-password">
        </div>
    </div>

    <div class="field">
        <div class="control">
            <button class="button is-link">Submit</button>
        </div>
    </div>
</form>
