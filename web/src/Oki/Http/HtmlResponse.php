<?php

declare(strict_types=1);

namespace Oki\Http;

use Oki\DI\DI;

class HtmlResponse implements HttpResponse
{
    private int $status;

    private string $viewPath;

    private array $viewParams;

    public function __construct(int $status, string $viewPath, array $viewParams = [])
    {
        $this->status = $status;
        $this->viewPath = $viewPath;
        $this->viewParams = $viewParams;
    }

    public static function notFound(string $message): HtmlResponse
    {
        return new HtmlResponse(404, '404', ['message' => $message]);
    }

    public function getStatusCode(): int
    {
        return $this->status;
    }

    public function render(DI $di, string $viewBasePath): void
    {
        $router = $di->getRouter();
        $security = $di->getSecurity();
        http_response_code($this->status);
        $params = $this->viewParams;
        ob_start();
        require $viewBasePath . '/' . $this->viewPath . '.php';
        $content = ob_get_clean();
        require $viewBasePath . '/layout.php';
    }
}
