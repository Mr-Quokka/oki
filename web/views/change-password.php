<?php
$title = 'Change password';
$small = true;
?>
<link rel="stylesheet" href="/style/form-layout.css">
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <h1>Change password</h1>
    <?php require 'errors.php'; ?>
    <div class="field">
        <label class="label" for="password">Current password</label>
        <div class="control">
            <input class="input" type="password" id="password" name="password" required autocomplete="current-password">
        </div>
    </div>
    <div class="field">
        <label class="label" for="new-password">New password</label>
        <div class="control">
            <input class="input" type="password" id="new-password" name="new-password" required autocomplete="new-password">
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
            <button class="button is-link">Login</button>
        </div>
    </div>
</form>
