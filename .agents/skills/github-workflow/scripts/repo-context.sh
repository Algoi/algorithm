#!/usr/bin/env bash

set -euo pipefail

if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "inside_git_repo=false"
  exit 0
fi

branch="$(git branch --show-current 2>/dev/null || true)"
if [ -z "$branch" ]; then
  branch="DETACHED"
fi

remote_name="$(git remote 2>/dev/null | head -n 1 || true)"
default_branch=""

if [ -n "$remote_name" ]; then
  default_branch="$(git symbolic-ref --quiet --short "refs/remotes/${remote_name}/HEAD" 2>/dev/null | sed "s#^${remote_name}/##" || true)"
fi

if [ -z "$default_branch" ]; then
  default_branch="$(git remote show "${remote_name:-origin}" 2>/dev/null | sed -n 's/.*HEAD branch: //p' | head -n 1 || true)"
fi

last_commit="$(git log -1 --pretty=format:'%h %s' 2>/dev/null || true)"

echo "inside_git_repo=true"
echo "repo_root=$(git rev-parse --show-toplevel)"
echo "branch=${branch}"
echo "remote=${remote_name:-none}"
echo "default_branch=${default_branch:-unknown}"
echo "last_commit=${last_commit:-none}"
echo "status<<EOF"
git status --short
echo "EOF"
