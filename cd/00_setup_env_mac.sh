#!/usr/bin/env bash
#
# Copyright (c) 2019-2020 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_macos_cross
export DOCKER_NAME_TAG=ubuntu:20.04
export HOST=x86_64-apple-darwin18
export PACKAGES="cmake imagemagick libcap-dev librsvg2-bin libz-dev libbz2-dev libtiff-tools libtinfo5 python3-dev python3-setuptools xorriso"
export XCODE_VERSION=12.1
export XCODE_BUILD_ID=12A7403
export GRIDCOIN_CONFIG="--with-gui --enable-reduce-exports"
