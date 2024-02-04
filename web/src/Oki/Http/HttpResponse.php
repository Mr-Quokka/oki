<?php

namespace Oki\Http;

use Oki\DI\DI;

interface HttpResponse
{
    public const OK = 200;
    public const CREATED = 201;
    public const BAD_REQUEST = 400;
    public const UNAUTHORIZED = 401;
    public const FORBIDDEN = 403;
    public const NOT_FOUND = 404;
    public const CONFLICT = 409;
    public const INTERNAL_SERVER_ERROR = 500;

    public function getStatusCode(): int;

    public function render(DI $di, string $viewBasePath): void;
}
