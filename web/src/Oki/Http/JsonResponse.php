<?php

declare(strict_types=1);

namespace Oki\Http;

class JsonResponse implements HttpResponse
{
    private int $status;
    private $body;

    public function __construct(int $status, $body)
    {
        $this->status = $status;
        $this->body = $body;
    }

    public function getStatusCode(): int
    {
        return $this->status;
    }

    public function render(string $viewBasePath): void
    {
        http_response_code($this->status);
        header('Content-type: application/json');
        echo json_encode($this->body);
    }
}
