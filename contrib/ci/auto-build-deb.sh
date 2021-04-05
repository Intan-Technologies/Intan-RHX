#!/bin/sh
set -e

#
# This script is supposed to be run by the GitHub CI system.
#

if [ -z "$1" ]; then
    echo "No suite set to build the package for."
    exit 1
fi

SUITE=$1

#
# Build Debian package
#

git_commit=$(git log --pretty="format:%h" -n1)
git_current_tag=$(git describe --abbrev=0 --tags 2> /dev/null || echo "v3.0.2")
git_commit_no=$(git rev-list --count HEAD)
upstream_version=$(echo "${git_current_tag}" | sed 's/^v\(.\+\)$/\1/;s/[-]/./g')
upstream_version="$upstream_version+git$git_commit_no"

set +x
mv contrib/debian .
dch --distribution "UNRELEASED"	--newversion="${upstream_version}" -b \
    "New automated build from: ${upstream_version} - ${git_commit}"

mkdir -p result
debspawn build \
            --results-dir=./result \
            $SUITE \
            .
