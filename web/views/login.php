<?php
$title = "Login";
$small = true;
?>
<link rel="stylesheet" href="/style/form-layout.css">
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <h1>Connection</h1>
    <?php if ($params['fail']) : ?>
        <article class="message is-danger">
            <div class="message-header">
                <p>Auth failed</p>
            </div>
            <div class="message-body">
                Login and/or password is invalid.
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
        <div class="control">
            <button class="button is-link">Login</button>
        </div>
    </div>
</form>
