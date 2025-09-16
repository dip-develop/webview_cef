#!/bin/bash

# Adds '#include <flutter_chromium/flutter_chromium_plugin.h>' at the top of the file

# Check if the correct number of arguments is passed
if [ $# -ne 1 ]; then
  echo "Usage: $0 <file-path>"
  exit 1
fi

file="$1"

if [ ! -f "$file" ]; then
  echo "File not found: $file"
  exit 1
fi

if grep -q '#include <flutter_chromium/flutter_chromium_plugin.h>' "$file"; then
  echo "The line '#include <flutter_chromium/flutter_chromium_plugin.h>' already exists in the file."
else
  (echo "#include <flutter_chromium/flutter_chromium_plugin.h>"; cat "$file") > "$file.tmp" && mv "$file.tmp" "$file"
  echo "Added '#include <flutter_chromium/flutter_chromium_plugin.h>' at the top of the file."
fi
