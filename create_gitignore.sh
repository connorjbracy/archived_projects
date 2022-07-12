#!/bin/sh

PROJECT_ROOT=${PROJECT_ROOT:-"$(pwd)"}
GITIGNORE_PATH="$PROJECT_ROOT/.gitignore"

echo ".gitignore" > $GITIGNORE_PATH
# find . -name *public.gitignore
for f in $(find . -wholename "./*/public.gitignore"); do
  cat "$f" | sed -r "s|^|${f}/|" >> $GITIGNORE_PATH
done
