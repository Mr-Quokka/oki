<?php

namespace Oki\Http;

use Oki\DI\DI;

interface HttpResponse
{
    public function getStatusCode(): int;

    public function render(DI $di, string $viewBasePath): void;
}
