<?php

declare(strict_types=1);

namespace Oki\Router;

use Oki\Http\HttpResponse;
use Oki\DI\DI;

class Router
{
	private string $url;
    private string $method;

	/**
	 * @var Route[]
	 */
	private array $routes = [];

	public function __construct(string $url, string $requestMethod)
	{
		$this->url = trim($url, '/');
        $this->method = $requestMethod;
	}

	public function get(string $path, callable $callable): self
	{
		return $this->addRoute('GET', $path, $callable);
	}

	private function addRoute(string $method, string $path, $callable): self
	{
		$route = new Route($path, $callable);
		$this->routes[$method][] = $route;
		return $this;
	}

	public function run(DI $di): HttpResponse
	{
		if (!isset($this->routes[$this->method])) {
			throw new RouteNotFoundException('Unknown HTTP method');
		}
		foreach ($this->routes[$this->method] as $route) {
			if ($route->matches($this->url)) {
				return $route->call($di);
			}
		}
		throw new RouteNotFoundException('No matching routes');
	}
}

