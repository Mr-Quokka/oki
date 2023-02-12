<?php
/**
 * @var \Oki\Model\Package $package
 */
$package = $params['package'];
/**
 * @var \Oki\Model\PackageVersion? $version
 */
$version = $params['version'];
$title = $package->getName();
$base = $router->url('package/' . $package->getName());
?>
<div class="package-resume">
    <h2>
        <?= $package->getName() ?>
        <?php if ($version !== null): ?>
            <small>v<?= $version->getIdentifier() ?></small>
        <?php endif; ?>
    </h2>
    <p class="package-description"><?= $package->getDescription() ?></p>
</div>

<nav class="tabs">
    <ul>
        <li <?php if ($params['action'] === 'readme') { echo 'aria-selected="true"'; } ?>>
            <a href="<?= $base ?>">Readme</a>
        </li>
        <li <?php if ($params['action'] === 'versions') { echo 'aria-selected="true"'; } ?>>
            <a href="<?= $base . '/versions' ?>">Versions</a>
        </li>
        <?php if ($version !== null): ?>
            <li <?php if ($params['action'] === 'dependencies') { echo 'aria-selected="true"'; } ?>>
                <a href="<?= $base . '/' . $version->getIdentifier() . '/dependencies' ?>">Dependencies</a>
            </li>
        <?php endif; ?>
    </ul>
</nav>

<div class="package-info">
    <div class="package-readme">
        <?php if ($params['action'] === 'versions'): ?>
            <?php require 'versions.php' ?>
        <?php elseif ($params['action'] === 'dependencies'): ?>
            <?php require 'dependencies.php' ?>
        <?php else: ?>
            <?= $readme ?? 'No Readme was provided' ?>
        <?php endif; ?>
    </div>
    <aside class="package-sidebar">
        <div class="package-metadata">
            <h3>Metadata</h3>
            <?php if ($version === null): ?>
                <p>No version was found.</p>
            <?php else: ?>
                <div class="version-date">
                    <relative-time datetime="<?= $version->getPublishedDate() ?>">
                        <?= $version->getPublishedDate() ?>
                    </relative-time>
                </div>
                <div class="version-size">
                    <?= \Oki\Util\FileHelper::formatBytes($version->getFileSize()) ?>
                </div>
            <?php endif; ?>
        </div>
        <div class="package-metadata">
            <h3>Install</h3>
            <?php if ($version === null): ?>
                <p>This package doesn't have any version.</p>
            <?php else: ?>
                <p>Run the <code>install</code> command:</p>
                <code>
                    oki install <?= $package->getName() ?>
                </code>
                <p>Or add the following line to your <code>oki.toml</code> file:</p>
                <code>
                    "<?= $package->getName() ?>" = "<?= $version->getIdentifier() ?>"
                </code>
            <?php endif; ?>
        </div>
        <div class="package-owners">
            <h3>Owners</h3>
            <?php if (empty($params['owners'])): ?>
                <p>This package doesn't have any owner.</p>
            <?php else: ?>
                <ul>
                    <?php foreach ($params['owners'] as $owner): ?>
                        <li><?= $owner->getLogin() ?></li>
                    <?php endforeach; ?>
                </ul>
            <?php endif; ?>
        </div>
    </aside>
</div>

<script src="https://cdn.jsdelivr.net/npm/@github/relative-time-element@4.1.5/dist/index.js" type="module" crossorigin="anonymous"></script>
