<?php

declare(strict_types=1);

namespace Oki\Router;

use Oki\DI\DI;
use Oki\Http\HttpResponse;

class Route
{
	private string $path;

	private $callable;

	/**
	 * @var string[]
	 */
	private array $matches = [];

	public function __construct(string $path, callable $callable)
	{
		$this->path = $path;
		$this->callable = $callable;
	}

	public function matches(string $url): bool
	{
		return preg_match($this->path, $url, $this->matches) === 1;
	}

	public function call(DI $di): HttpResponse
	{
		return call_user_func_array($this->callable, [$di, $this->matches]);
	}
}
