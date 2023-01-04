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
            <img src="/images/OKI.logo.1.png"></img>
            <p>Open pacKage Installer</p>
        </a>
    </header>

    <main>
        <?= $content ?>
    </main>
</body>
</html>
