<?php

declare(strict_types=1);

namespace Oki\Validator;

use Oki\Model\PackageManifest;

const HAS_ZIP_EXT = false;

final class PublicationValidator
{
    public static function validateVersionContent(array $files, array &$errors, string $key = 'package'): bool
    {
        if (empty($files[$key])) {
            $errors[] = 'No content has been found';
            return false;
        }
        if (HAS_ZIP_EXT) {
            $zip = new \ZipArchive();
            if ($zip->open($files[$key]['tmp_name']) === true) {
                $zip->close();
            } else {
                $errors[] = 'The version content cannot be read as a .zip file';
            }
        } else {
            $file = fopen($files[$key]['tmp_name'], 'r');
            if (!$file) {
                $errors[] = 'Couldn\'t read version content';
                return false;
            }
            $blob = fgets($file, 5);
            if (strpos($blob, 'PK') === false) {
                $errors[] = 'The version content does not look like a .zip file';
            }
            fclose($file);
        }
        return empty($errors);
    }

    public static function validateJson(array $post, array &$errors, string $key = 'manifest'): ?PackageManifest
    {
        if (empty($post[$key])) {
            $errors[] = 'No manifest has been found';
            return null;
        }
        try {
            $json = json_decode($post[$key], true, 4, JSON_THROW_ON_ERROR);
            if (empty($json['name']) || !self::validateShortName($json['name'])) {
                $errors[] = 'Invalid package name';
            }
            if (empty($json['version']) || !self::validateVersion($json['version'])) {
                $errors[] = 'Invalid package version';
            }
            if (empty($json['kind']) || !is_string($json['kind'])) {
                $errors[] = 'Invalid package kind';
            }
            if (isset($json['dependencies']) && !self::validateDependencies($json['dependencies'])) {
                $errors[] = 'Invalid dependencies';
            }
            if (empty($errors)) {
                return new PackageManifest(
                    $json['name'],
                    $json['description'] ?? '',
                    $json['version'],
                    $json['kind'],
                    $json['dependencies'] ?? []
                );
            }
        } catch (\JsonException $ex) {
            $errors[] = $ex->getMessage();
        }
        return null;
    }

    public static function validateShortName(string $name): bool
    {
        return preg_match('/^[a-z0-9_-]+$/', $name) && strlen($name) < 64;
    }

    public static function validateVersion(string $version): bool
    {
        return preg_match('/^([0-9]+)\.([0-9]+)\.([0-9]+)$/', $version) && strlen($version) < 64;
    }

    public static function validateRange(string $range): bool
    {
        return preg_match('/^([\^~=]?([0-9]+)\.([0-9]+)\.([0-9]+))|\*$/', $range) === 1;
    }

    public static function validateDependencies(array $dependencies): bool
    {
        if (!is_array($dependencies)) {
            return false;
        }
        foreach ($dependencies as $package => $range) {
            if (!self::validateShortName($package) || !self::validateRange($range)) {
                return false;
            }
        }
        return true;
    }
}
