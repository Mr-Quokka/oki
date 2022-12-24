# Dependencies

Oki uses the [semantic versioning specification](https://semver.org) to determine is compatible between different versions of a package.

Dependencies specify a compatible version range they want to rely on:

|Requirement|Example|Equivalence|Description|
|--|--------|--|-------------|
|Caret|`1.2.3` or `^1.2.3`| `>=1.2.3,<2.0.0`|Any greater or equal version without a major version bump.|
|Tilde|`~1.2`|`>=1.2.0,<1.3.0`|Any greater or equal version without a major or minor version bump.|
|Equals| `=1.2.3`|`=1.2.3`|Exactly the specified version only.|
|Wildcard|`*` |`Ω`|Any version.|
