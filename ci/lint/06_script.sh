#!/usr/bin/env bash
#
# Copyright (c) 2018-2019 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://opensource.org/licenses/mit-license.php.

export LC_ALL=C


if [ "$EVENT_TYPE" = "pull_request" ]; then
  test/lint/commit-script-check.sh $(git rev-parse HEAD~$COMMIT_COUNT)..$GITHUB_SHA
fi
#test/lint/git-subtree-check.sh src/crypto/ctaes
#test/lint/git-subtree-check.sh src/secp256k1
#test/lint/git-subtree-check.sh src/univalue
#test/lint/git-subtree-check.sh src/leveldb
#test/lint/git-subtree-check.sh src/crc32c
#test/lint/check-doc.py
#test/lint/check-rpc-mappings.py .
test/lint/lint-all.sh

#if [ "$CI_REPO_SLUG" = "gridcoin-community/Gridcoin-Research" ] && [ "$CI_EVENT_TYPE" = "cron" ]; then
#    git log --merges --before="2 days ago" -1 --format='%H' > ./contrib/verify-commits/trusted-sha512-root-commit
#    $CI_RETRY_EXE gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys $(<contrib/verify-commits/trusted-keys) &&
#    ./contrib/verify-commits/verify-commits.py --clean-merge=2;
#fi
