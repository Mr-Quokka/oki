<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <link rel="stylesheet" href="style/layout.css">
    <title><?= $title ?? 'Package registry' ?> - OKI</title>
</head>
<body>
    <header>
        <a href="index.php" class="logo">
            <pre>
 ██████╗ ██╗  ██╗██╗
██╔═══██╗██║ ██╔╝██║
██║   ██║█████╔╝ ██║
██║   ██║██╔═██╗ ██║
╚██████╔╝██║  ██╗██║
 ╚═════╝ ╚═╝  ╚═╝╚═╝
</pre>
        </a>
    </header>

    <main>
        <?= $content ?>
    </main>
</body>
</html>