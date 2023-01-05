<?php $title="Login" ?>
<link rel="stylesheet" href="/style/form-layout.css">
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
<form action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
    <div class="field">
        <label class="label" for="login">Login</label>
        <div class="control">
            <input class="input" type="text" id="login" name="login">
        </div>
    </div>
    <div class="field">
        <label class="label" for="password">Password</label>
        <div class="control">
            <input class="input" type="password" id="password" name="password" autocomplete="current-password">
        </div>
    </div>

    <div class="field">
        <div class="control">
            <button class="button is-link">Submit</button>
        </div>
    </div>
</form>
