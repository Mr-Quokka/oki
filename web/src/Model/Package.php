<?php declare(strict_types = 1);

class Package {

	private int $id;


	private string $name;
	
	/**
	 * @var PackageVersion[]
	 */
	private array $versions = [];
}
