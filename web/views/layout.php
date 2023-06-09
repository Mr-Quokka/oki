<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="/style/layout.css">
    <link rel="icon" type="image/x-icon" href="/images/favicon.ico">
    <script src="/scripts/toggle-theme.js" defer></script>
    <title><?= $title ?? 'Package registry' ?> - OKI</title>
</head>
<body class="preload">
    <script>
        if (localStorage.getItem("pref-theme") === "dark") {
            document.body.classList.add('dark');
        } else if (localStorage.getItem("pref-theme") === "light") {
            document.body.classList.remove('dark');
        } else if (window.matchMedia('(prefers-color-scheme: dark)').matches) {
            document.body.classList.add('dark');
        }
    </script>
    <header id="header">
        <a title="Home - OKI" class="logo" href="<?= $router->url('') ?>">
            <picture class="image-logo">
                <source srcset="/images/logo/BIG-OKI-logo-1.png" media="(prefers-color-scheme: dark)" />
                <img src="/images/logo/BIG-OKI-logo-2.png" alt="The K letter surrounded by the letter O and an I" />
            </picture>
            <h1>Open pacKage Installer</h1>
        </a>
        <h2 class="top-right-menu">
            <button id="theme-toggle" accesskey="t" title="Toggles light & dark" aria-label="auto" aria-live="polite">
                <svg id="moon" xmlns="http://www.w3.org/2000/svg" width="24" height="18" viewBox="0 0 24 24"
                    fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"
                    stroke-linejoin="round">
                    <path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"></path>
                </svg>
                <svg id="sun" xmlns="http://www.w3.org/2000/svg" width="24" height="18" viewBox="0 0 24 24"
                    fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"
                    stroke-linejoin="round">
                    <circle cx="12" cy="12" r="5"></circle>
                    <line x1="12" y1="1" x2="12" y2="3"></line>
                    <line x1="12" y1="21" x2="12" y2="23"></line>
                    <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
                    <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
                    <line x1="1" y1="12" x2="3" y2="12"></line>
                    <line x1="21" y1="12" x2="23" y2="12"></line>
                    <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
                    <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
                </svg>
            </button>
            <?php if ($security->getCurrentUser() === null): ?>
                <a href="<?= $router->url('login') ?>">Login</a> -
                <a href="<?= $router->url('register') ?>">Register</a>
            <?php else: ?>
                Logged as <?= $security->getCurrentUser()->getLogin() ?> -
                <a href="<?= $router->url('logout') ?>">Logout</a>
            <?php endif; ?>
        </h2>
    </header>

    <main <?php if (isset($small)) { echo 'class="small-viewport"'; } ?>>
        <?= $content ?>
    </main>
</body>
</html>
