#!/bin/bash

# Récupérer tous les tags qui respectent la regex des versions
tags=$(git tag -l | grep -E '^[0-9]+(\.[0-9]+)*$' | sort -V)

# Créer une nouvelle branche orpheline
git checkout --orphan version-history

# Pour chaque tag, créer un commit avec l'état du dépôt à ce tag
for tag in $tags; do
    git rm -rf .
    git checkout $tag -- .
    git commit --allow-empty -m "v$tag"
done
