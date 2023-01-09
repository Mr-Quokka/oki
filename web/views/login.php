<?php
$title = 'Login';
$small = true;
?>
<link rel="stylesheet" href="/style/form-layout.css">
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <h1>Connection</h1>
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
            <input class="input" type="password" id="password" name="password" required autocomplete="current-password">
        </div>
    </div>

    <div class="field">
        <div class="control">
            <button class="button is-link">Login</button>
        </div>
    </div>
</form>
