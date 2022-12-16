<?php

declare(strict_types=1);

namespace Oki\Model;

use JsonSerializable;

abstract class AbstractPackage implements JsonSerializable
{
    protected int $id_package;

    protected string $short_name;

    protected string $description;

    public function getId()
    {
        return $this->id_package;
    }

    public function getShortName(): string
    {
        return $this->short_name;
    }

    public function getDescription(): string
    {
        return $this->description;
    }
}
