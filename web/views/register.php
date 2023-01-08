<?php
$title = "Register";
$small = true;
?>
<link rel="stylesheet" href="/style/form-layout.css">
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <h1>Create an account</h1>
    <?php if ($params['fail']) : ?>
        <article class="message is-danger">
            <div class="message-header">
                <p>Registration failed</p>
            </div>
            <div class="message-body">
                Login is already taken.
            </div>
        </article>
    <?php endif ?>

    <div class="field">
        <label class="label" for="login">Login</label>
        <div class="control">
            <input class="input" type="text" id="login" name="login" autocomplete="username">
        </div>
    </div>
    <div class="field">
        <label class="label" for="password">Password</label>
        <div class="control">
            <input class="input" type="password" id="password" name="password" autocomplete="new-password">
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
