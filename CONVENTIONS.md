Conventions
----------

Note : Ces conventions s'appliquent à la fois à la partie PHP et la partie C++.
Les lignes sont indentées avec 4 espaces et le code est écrit en anglais, hors commentaires écrits en français.

Conventions de nommage
---------------------

### Général

Les noms de classes utilisent la notation `UpperCamelCase`. Les noms de méthodes, d'attribut et de variables sont notées avec la syntaxe `lowerCamelCase`, excepté ce qui est lié à des colonnes SQL.

Les noms de méthodes doivent de préférence être des phrases verbales comme `listPackage` ou `addVersion`.
Les noms de booléens devraient être de préférence être des adjectifs comme `active`, avec la méthode correspondante nommée `isActive`.

Une variable faisant référence à de multiples éléments (liste, tableau, arbre, graphe...) est nommée au pluriel comme `packages` pour plusieurs éléments de type `Package`.

Les noms de tables et de colonnes SQL sont écrites avec la syntaxe `snake_case`. Les mots-clés SQL sont capitalisés.

Un intitulé de commit commence par une majuscule et un verbe à l'impératif. Il contient éventuellement la portée de la modification :
- *web* : relatif à la partie web PHP
- *cli* : relatif à la ligne de commande C++
- ...

### Abréviations

La règle générale est d'éviter les abréviations si elles ne sont pas claires comme tout le monde. Des noms complets sont souvent plus simples à lire.
Une liste non exhaustive des abréviatives possibles serait :
- *id* pour *identifier*
- *pkg* pour *package*
- *min*/*max* pour *minimum*/*maximum*

Convention de style
-------------------

```php
<?php declare(strict_types=1);

namespace Model;

class Bird {

    private int $number;

    private string $name;

    public function __construct(int $number, string $name = 'unknown') {
        $this->number = $number;
        $this->name = $name;
    }
    
    public function getSpeed(): int {
        if ($this->number === 55) {
            return 20;
        } else if ($this->name === 'Leonard') {
            return 19;
        }
        return strlen($this->name);
    }
}
```

`clang-format` est utilisé pour formater le code source C++.

```cpp
#pragma once

namespace oki {
    template <typename T>
    class Box {
    private:
        T value;

    public:
        Box(T value);
        const T &peek() const;
    }
}
```
