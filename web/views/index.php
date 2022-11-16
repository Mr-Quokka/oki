<h2 style="text-align: center"><a href="publish.php">Publish a package</a></h2>
<hr />
<?php if (empty($params['packages'])) : ?>
    <p style="text-align: center">ERROR : there is no package</p>
<?php else : ?>
    <?php foreach ($params['packages'] as $pkg) : ?>
        <div>
            <h3><a href="<?= $pkg->getShortName() ?>" class="package-name"><?= $pkg->getShortName() ?></a></h3>
            <p class="package-description"><?= $pkg->getDescription() ?></p>
        </div>
    <?php endforeach ?>
<?php endif ?>