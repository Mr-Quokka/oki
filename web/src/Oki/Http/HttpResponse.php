<?php

namespace Oki\Http;

interface HttpResponse
{
    public function getStatusCode(): int;

    public function render(string $viewBasePath): void;
}
