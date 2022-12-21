<?php

declare(strict_types=1);

namespace Oki\Model;

use JsonSerializable;

abstract class AbstractPackage implements JsonSerializable
{
    protected int $id_package;

    protected string $name;

    protected string $description;

    public function getId(): int
    {
        return $this->id_package;
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function getDescription(): string
    {
        return $this->description;
    }
}
