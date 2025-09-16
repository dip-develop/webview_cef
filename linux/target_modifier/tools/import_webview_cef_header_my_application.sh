#!/bin/bash

# Adds the line
# '#include <flutter_chromium/flutter_chromium_plugin.h>'
# after the line
# '#include "flutter/generated_plugin_registrant.h"'
# if '#include <flutter_chromium/flutter_chromium_plugin.h>' is missing

# Check if the correct number of arguments is passed
if [ $# -ne 1 ]; then
  echo "Usage: $0 <file-path>"
  exit 1
fi

# Assign file path argument
file="$1"

# Check if the file exists
if [ ! -f "$file" ]; then
  echo "File not found: $file"
  exit 1
fi

# Check if the line already exists in the file
if grep -q '#include <flutter_chromium/flutter_chromium_plugin.h>' "$file"; then
  echo "The line '#include <flutter_chromium/flutter_chromium_plugin.h>' already exists in the file."
  exit 0
fi

# Iterate through the file and look for the line "#include \"flutter/generated_plugin_registrant.h\""
awk '
  /#include "flutter\/generated_plugin_registrant.h"/ {
    print;
    print "#include <flutter_chromium/flutter_chromium_plugin.h>"
    next
  }
  { print }
' "$file" > "$file.tmp" && mv "$file.tmp" "$file"

echo "Added '#include <flutter_chromium/flutter_chromium_plugin.h>' after '#include \"flutter/generated_plugin_registrant.h\"'"
