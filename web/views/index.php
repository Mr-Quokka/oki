<?php $title = "Home" ?>

<?php if (empty($params['packages'])) : ?>
    <h2 class="error">ERROR : there is no package</h2>
<?php else : ?>
    <?php foreach ($params['packages'] as $pkg) : ?>
            <div class="package">
                <div class="package-prompt">
                    <a class="package-name" href="/package/<?= $pkg->getName() ?>">
                        <?= $pkg->getName() ?>
                    </a>
                    <?php if($pkg->getLatestVersion() !== null): ?>
                        <?= "v" . $pkg->getLatestVersion() ?>
                    <?php endif ?>
                </div>
                <p class="package-description"><?= $pkg->getDescription() ?></p>
            </div>
    <?php endforeach ?>
<?php endif ?>
