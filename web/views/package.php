<?php
$title = $params['package']->getName();
?>
<div class="center-content">
    <h2><?= $params['package']->getName() ?></h2>
    <h3 class="package-detail-description"><?= $params['package']->getDescription() ?></h3>
</div>
<hr/>

<h3>Versions</h3>
<?php if (empty($params['package']->getVersions())): ?>
    <p>This package doesn't have any version</p>
<?php else: ?>
    <ul>
        <?php foreach ($params['package']->getVersions() as $version): ?>
            <li><a href="<?= $version->getDownloadUrl() ?>"><?= $version->getIdentifier() ?> (<?= $version->getPublishedDate() ?>)</a></li>
        <?php endforeach ?>
    </ul>
<?php endif ?>
