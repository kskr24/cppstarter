#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <problem_number>" >&2
  exit 1
fi

name="$1"
dir="src/${name}"

if [[ -d "$dir" ]]; then
  echo "Error: $dir already exists" >&2
  exit 1
fi

mkdir -p "$dir"

cat > "${dir}/main.cpp" <<EOF
// $1
#include <array>
#include <queue>
#include <vector>

int main() { return 0; }
EOF

cat > "${dir}/${name}.mk" <<EOF
bin/${name}: src/${name}/main.o
	\$(CXX) -o \$@ \$^ \$(LDFLAGS)
EOF

echo "Created ${dir}"
