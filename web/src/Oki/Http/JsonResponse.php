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

    public static function success(int $status, string $message): JsonResponse
    {
        return new JsonResponse($status, ['success' => $message]);
    }

    public static function error(int $status, string $message): JsonResponse
    {
        return new JsonResponse($status, ['error' => $message]);
    }

    public static function badRequest(string $message): JsonResponse
    {
        return self::error(400, $message);
    }

    public static function notFound(string $message): JsonResponse
    {
        return self::error(404, $message);
    }

    public static function conflict(string $message): JsonResponse
    {
        return self::error(409, $message);
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
