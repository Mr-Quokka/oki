<?php

declare(strict_types=1);

namespace Oki\Validator;

class PaginationValidator
{
    public static function getLimit(array $get): int
    {
        if (isset($get['limit']) && is_numeric($get['limit'])) {
            return min(100, max(0, intval($get['limit'])));
        }
        return 5;
    }

    public static function getPage(array $get): int
    {
        if (isset($get['page']) && is_numeric($get['page'])) {
            return min(100, max(1, intval($get['page'])));
        }
        return 1;
    }
}
