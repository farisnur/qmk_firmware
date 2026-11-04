#!/bin/sh
# Merge the latest qmk/qmk_firmware master into this fork and drop every
# keyboard that is not ours, so the repo keeps only QMK core + own keyboards.
#
# Usage: ./util/sync_upstream.sh            (run from anywhere in the repo)
# Own keyboards are listed in MY_KEYBOARDS (top-level folders in keyboards/).

MY_KEYBOARDS="void30 zen2040"
UPSTREAM_URL="https://github.com/qmk/qmk_firmware.git"

set -e
cd "$(git rev-parse --show-toplevel)"

if [ -n "$(git status --porcelain)" ]; then
    echo "Working tree not clean - commit or stash first." >&2
    exit 1
fi

git remote get-url upstream >/dev/null 2>&1 || git remote add upstream "$UPSTREAM_URL"
git fetch --filter=blob:none upstream master

# Merge without committing so foreign keyboards can be dropped in the same commit.
git merge --no-commit --no-ff upstream/master || true

is_mine() {
    for kb in $MY_KEYBOARDS; do
        case "$1" in keyboards/"$kb"/*) return 0 ;; esac
    done
    return 1
}

# Remove every tracked or conflicted path under keyboards/ that is not ours.
git ls-files keyboards | sort -u | while IFS= read -r f; do
    is_mine "$f" || git rm -q --cached --ignore-unmatch -- "$f"
done
for d in keyboards/*; do
    name=$(basename "$d")
    case " $MY_KEYBOARDS " in *" $name "*) ;; *) rm -rf -- "$d" ;; esac
done

if [ -n "$(git diff --name-only --diff-filter=U)" ]; then
    echo "Conflicts outside foreign keyboards - resolve them, then 'git commit':" >&2
    git diff --name-only --diff-filter=U >&2
    exit 1
fi

git submodule update --init --recursive
git commit -q -m "Merge upstream qmk/qmk_firmware master (own keyboards only)" || echo "Nothing to merge."
echo "Done. Review with 'git log -1 --stat', then push."
