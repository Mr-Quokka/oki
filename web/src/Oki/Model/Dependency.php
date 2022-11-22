<?php

declare(strict_types=1);

namespace Oki\Model;

use JsonSerializable;

class Dependency implements JsonSerializable
{
	private int $package_reference_id;
	private int $constrainer_id;
	private string $constraint_value;

	public function jsonSerialize()
	{
		$dict = [
			'package_reference_id' => $this->package_reference_id,
			'constrainer_id' => $this->constrainer_id,
			'constraint_value' => $this->constraint_value
		];
		return $dict;
	}
}

?>