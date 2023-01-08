<?php

namespace Oki\Util;

final class FileHelper
{
    public static function formatBytes(int $size, int $precision = 2): string
    {
        $base = floor(log($size) / log(1024));
        $units = ['B', 'kB', 'MB', 'GB', 'TB', 'PB'];
        return round($size / (1 << (10 * $base)), $precision) . ' ' . $units[$base];
    }
}
