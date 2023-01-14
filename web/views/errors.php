<?php if (!empty($params['errors'])):
    foreach ($params['errors'] as $error): ?>
        <article class="message is-danger error">
            <div class="message-body">
                <?= $error ?>
            </div>
        </article>
    <?php endforeach;
endif; ?>
