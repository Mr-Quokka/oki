<?php

namespace Oki\Http;

use Oki\DI\DI;

class RedirectResponse implements HttpResponse
{
    private int $status;
    private string $targetUrl;

    public function __construct(string $targetUrl, int $status = 303)
    {
        $this->targetUrl = $targetUrl;
        $this->status = $status;
    }

    public function getStatusCode(): int
    {
        return $this->status;
    }

    public function render(DI $di, string $viewBasePath): void
    {
        http_response_code($this->status);
        header('Location: ' . $this->targetUrl);
    }
}
