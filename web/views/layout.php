<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <link rel="stylesheet" href="/style/layout.css">
    <link rel="icon" type="image/x-icon" href="/images/favicon.ico">
    <title><?= $title ?? 'Package registry' ?> - OKI</title>
</head>
<body>
    <header>
        <a class="logo" href="<?= $router->url('') ?>">
            <img src="/images/logo/BIG-OKI-logo-1.png"></img>
            <p>Open pacKage Installer</p>
        </a>
        <h2 class="connection">
            <?php if ($security->getCurrentUser() === null): ?>
                <a href="<?= $router->url('login') ?>">Login</a> -
                <a href="<?= $router->url('register') ?>">Register</a>
            <?php else: ?>
                Logged as <?= $security->getCurrentUser()->getLogin() ?> -
                <a href="<?= $router->url('logout') ?>">Logout</a>
            <?php endif; ?>
        </h2>
    </header>

    <main>
        <?= $content ?>
    </main>
</body>
</html>
