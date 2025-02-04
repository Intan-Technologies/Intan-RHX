#!/bin/sh
#
# Install Intan RHX build dependencies
#
set -e
set -x

export DEBIAN_FRONTEND=noninteractive

# update caches
apt-get update -qq

# install build essentials
apt-get install -yq \
    eatmydata \
    build-essential \
    gcc \
    g++

# package build essentials
eatmydata apt-get install -yq --no-install-recommends \
    ca-certificates \
    git-core \
    dpkg-dev \
    debhelper \
    devscripts \
    libdistro-info-perl \
    debspawn

# validation tools
eatmydata apt-get install -yq --no-install-recommends \
    appstream \
    desktop-file-utils

# Intan RHX build dependencies
eatmydata apt-get install -yq --no-install-recommends \
    ocl-icd-opencl-dev \
	qmake6 \
	qt6-base-dev \
	qt6-multimedia-dev \
	liblua5.3-dev