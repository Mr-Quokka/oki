<?php 
$get = $_SERVER['QUERY_STRING'] ?? '';
if (!empty($get)) {
    $get = '?' . $get;
}
?>

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
                    <a href="/package/<?= $pkg->getName() ?>" class="package-name">
                        <?= $pkg->getName() ?>
                        <?php if($pkg->getLatestVersion() !== null): ?>
                            <?= " v" . $pkg->getLatestVersion() ?>
                        <?php endif ?>
                    </a>
                </h3>
                <p class="package-description"><?= $pkg->getDescription() ?></p>
            </div>
    <?php endforeach ?>
<?php endif ?>

<nav class="pagination" role="navigation" aria-label="pagination">
    <?php if ($params['page'] > 1) : ?>
        <a class="pagination-previous" href="<?= $router->url('recent/' . ($params['page'] - 1)) . $get ?>">Previous</a>
    <?php endif; ?>
    <?php if ($params['page'] < $params['nbPages']) : ?>
        <a class="pagination-next" href="<?= $router->url('recent/' . ($params['page'] + 1)) . $get ?>">Next page</a>
    <?php endif; ?>
    <ul class="pagination-list">
        <?php if ($params['page'] > 2) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page 1" href="<?= $router->url('recent/1') . $get ?>">1</a>
            </li>
            <?php if ($params['page'] > 3) : ?>
                <li>
                    <span class="pagination-ellipsis">&hellip;</span>
                </li>
            <?php endif; ?>
        <?php endif; ?>
        <?php if ($params['page'] > 1) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page <?= $params['page'] - 1 ?>" href="<?= $router->url('recent/' . ($params['page'] - 1)) . $get ?>"><?= $params['page'] - 1 ?></a>
            </li>
        <?php endif; ?>
        <li>
            <a class="pagination-link is-current" aria-label="Page <?= $params['page'] ?>" aria-current="page"><?= $params['page'] ?></a>
        </li>
        <?php if ($params['page'] < ($params['nbPages'] - 1)) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page <?= $params['page'] + 1 ?>" href="<?= $router->url('recent/' . ($params['page'] + 1)) . $get ?>"><?= $params['page'] + 1 ?></a>
            </li>
            <?php if ($params['page'] < ($params['nbPages'] - 2)) : ?>
                <li>
                    <span class="pagination-ellipsis">&hellip;</span>
                </li>
            <?php endif; ?>
        <?php endif; ?>
        <?php if ($params['page'] < $params['nbPages']) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page <?= $params['nbPages'] ?>" href="<?= $router->url('recent/' . $params['nbPages']) . $get ?>"><?= $params['nbPages'] ?></a>
            </li>
        <?php endif; ?>
    </ul>
</nav>

