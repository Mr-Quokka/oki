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
        return new HtmlResponse(HttpResponse::NOT_FOUND, '404', ['message' => $message]);
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

    /**
     * @param string[] $errors
     * @return $this
     */
    public function errors(array $errors): self
    {
        $this->viewParams['errors'] = $errors;
        if (!empty($errors) && $this->status === HttpResponse::OK) {
            $this->status = HttpResponse::BAD_REQUEST;
        }
        return $this;
    }
}
