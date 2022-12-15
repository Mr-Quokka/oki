<h2>
    <?php if ($security->getCurrentUser() == null): ?>
        <a href="<?= $router->url('login') ?>">Login</a>
    <?php else: ?>
        Logged as <?= $security->getCurrentUser()->getLogin() ?> -
        <a href="<?= $router->url('logout') ?>">Logout</a>
    <?php endif; ?>
</h2>
<hr />
<?php if (empty($params['packages'])) : ?>
    <p style="text-align: center">ERROR : there is no package</p>
<?php else : ?>
    <?php foreach ($params['packages'] as $pkg) : ?>
            <div>
                <h3>
                    <a href="/package/<?= $pkg->getShortName() ?>" class="package-name">
                        <?= $pkg->getShortName() ?>
                        <?php if(!empty($pkg->getVersions())): ?>
                            <?= " v" . $pkg->getVersions()[0]->getIdentifier() ?>
                        <?php endif ?>
                    </a>
                </h3>
                <p class="package-description"><?= $pkg->getDescription() ?></p>
            </div>
    <?php endforeach ?>
<?php endif ?>
