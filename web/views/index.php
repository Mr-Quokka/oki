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

<ul>
    <?php 

    if($params['page'] < 1){
        $params['page'] = 1;
    }

    if ($params['page'] > 1) { ?>
        <li><a href="<?= $router->url('recent/' . ($params['page'] - 1)) . $get ?>"> < </a></li>
    <?php
        }

    else { ?>
        <li><a> < </a></li>
    <?php
    }
    
    if(($params['page'] - 1) != 0){ ?>
        <li><a href="<?= $router->url('recent/' . ($params['page'] - 1)) . $get ?>">  <?= ($params['page'] - 1) ?> </a></li>
    <?php
        }

    if($params['page'] + 1 <= $params['nbPages'] && $params['page'] - 1 != 0){ ?>
        <li><a href=""> ... </a></li>
        
    <?php 
    }
    if($params['page'] < $params['nbPages']) { ?>
        <li><a href="<?= $router->url('recent/' . ($params['page'] + 1)) . $get ?>">  <?= ($params['page'] + 1) ?> </a></li>
    <?php 
    } 

    if ($params['page'] != $params['nbPages']) { ?>
        <li><a href="<?= $router->url('recent/' . ($params['page'] + 1)) . $get ?>"> > </a></li>
    <?php
        }

    else { ?>
        <li><a> > </a></li>
    <?php
    }
    ?>

</ul>

