<?php
$title = 'Register';
$small = true;
?>
<link rel="stylesheet" href="/style/form-layout.css">
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <h1>Create an account</h1>
    <?php require 'errors.php'; ?>
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
            <button class="button is-link">Register</button>
        </div>
    </div>
</form>
