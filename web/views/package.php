<?php $title = "Package (" . $params['package']->getName() . ")" ?> 

<div class="center-content">
    <h2><?= $params['package']->getName() ?></h2>
    <h3 class="package-description"><?= $params['package']->getDescription() ?></h3>
</div>
<hr/>

<?php if (empty($params['package']->getVersions())): ?>
    <p class="error">This package doesn't have any version</p>
<?php else: ?>
    <?php foreach ($params['package']->getVersions() as $version): ?>
        <div class="package">
            <h3>
                <a class="package-name" href="<?= $version->getDownloadUrl() ?>"><?= $version->getIdentifier() ?></a>
            </h3>
            <p class="package-description"> Published the <?= $version->getPublishedDate() ?></p>
        </div>
    <?php endforeach ?>
<?php endif ?>
