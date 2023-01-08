<?php

declare(strict_types=1);

namespace Oki\Model;

use Oki\Http\JsonResponse;

class TransactionResult
{
    private int $dataId;
    private int $httpStatus;
    private string $message;
    private $terminate;

    public function __construct(int $dataId, int $httpStatus, string $message)
    {
        $this->dataId = $dataId;
        $this->httpStatus = $httpStatus;
        $this->message = $message;
    }

    public function getDataId(): int
    {
        return $this->dataId;
    }

    public function isSuccess(): bool
    {
        return $this->httpStatus >= 200 && $this->httpStatus < 300;
    }

    public function isError(): bool
    {
        return !$this->isSuccess();
    }

    public function setTerminate(callable $terminate): void
    {
        $this->terminate = $terminate;
    }

    public function validate(): void
    {
        if ($this->terminate !== null) {
            call_user_func_array($this->terminate, []);
        }
    }

    public function asJson(): JsonResponse
    {
        if ($this->isError()) {
            return JsonResponse::error($this->httpStatus, $this->message);
        }
        return JsonResponse::success($this->httpStatus, $this->message);
    }
}
