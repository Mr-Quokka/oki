<?php if (!empty($params['errors'])):
    foreach ($params['errors'] as $error): ?>
        <article class="message is-danger">
            <div class="message-header">
                <p>Error</p>
            </div>
            <div class="message-body">
                <?= $error ?>
            </div>
        </article>
    <?php endforeach;
endif; ?>
