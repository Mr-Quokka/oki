<nav class="pagination" role="navigation" aria-label="pagination">
    <?php if ($params['page'] > 1) : ?>
        <a class="pagination-previous" href="<?= $router->url($paginationBase . ($params['page'] - 1)) . $get ?>"><</a>
    <?php else : ?>
        <p class="pagination-previous is-current"><</p>
    <?php endif; ?>
    <ul class="pagination-list">
        <?php if ($params['page'] > 2) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page 1" href="<?= $router->url($paginationBase . '1') . $get ?>">1</a>
            </li>
            <?php if ($params['page'] > 3) : ?>
                <li>
                    <span class="pagination-ellipsis">&hellip;</span>
                </li>
            <?php endif; ?>
        <?php endif; ?>
        <?php if ($params['page'] > 1) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page <?= $params['page'] - 1 ?>" href="<?= $router->url($paginationBase . ($params['page'] - 1)) . $get ?>"><?= $params['page'] - 1 ?></a>
            </li>
        <?php endif; ?>
        <li>
            <a class="pagination-link is-current" aria-label="Page <?= $params['page'] ?>" aria-current="page"><?= $params['page'] ?></a>
        </li>
        <?php if ($params['page'] < ($params['nbPages'] - 1)) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page <?= $params['page'] + 1 ?>" href="<?= $router->url($paginationBase . ($params['page'] + 1)) . $get ?>"><?= $params['page'] + 1 ?></a>
            </li>
            <?php if ($params['page'] < ($params['nbPages'] - 2)) : ?>
                <li>
                    <span class="pagination-ellipsis">&hellip;</span>
                </li>
            <?php endif; ?>
        <?php endif; ?>
        <?php if ($params['page'] < $params['nbPages']) : ?>
            <li>
                <a class="pagination-link" aria-label="Goto page <?= $params['nbPages'] ?>" href="<?= $router->url($paginationBase . $params['nbPages']) . $get ?>"><?= $params['nbPages'] ?></a>
            </li>
        <?php endif; ?>
    </ul>
    <?php if ($params['page'] < $params['nbPages']) : ?>
        <a class="pagination-next" href="<?= $router->url($paginationBase . ($params['page'] + 1)) . $get ?>">></a>
    <?php else : ?>
        <p class="pagination-next is-current">></p>
    <?php endif; ?>
</nav>
