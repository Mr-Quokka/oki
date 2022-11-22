<?php

declare(strict_types=1);

namespace Oki\Router;

use Exception;
use Throwable;

/**
 * Lorsqu'aucune route ne correspond à l'url demandée.
 */
class RouteNotFoundException extends Exception
{
	public function __construct(string $message, int $code = 0, ?Throwable $previous = null)
	{
		parent::__construct($message, $code, $previous);
	}
}
