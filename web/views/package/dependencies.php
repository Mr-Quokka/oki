<?php foreach ($params['version']->getDependencies() as $dep => $range): ?>
    <div class="package">
        <div class="package-prompt">
            <a class="package-name" href="<?= $router->url('package/' . $dep) ?>">
                <?= $dep ?>
            </a>
            <small><?= $range ?></small>
        </div>
    </div>
<?php endforeach; ?>
