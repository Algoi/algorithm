---
name: github-workflow
description: Use this skill when the user wants to work with a GitHub repository using git and GitHub CLI, including cloning or pulling code, checking repo state, creating or switching branches, addressing pull request review comments, committing changes, pushing a branch, and creating or updating a pull request.
---

# GitHub Workflow

Use this skill for day-to-day GitHub repository work driven by `git` and `gh`.

## Defaults

- Assume `git` and `gh` are installed.
- Assume `gh` authentication is already configured.
- Treat repository state as the source of truth. Inspect it before acting.
- Require user confirmation before any remote write action such as `push`, `gh pr create`, `gh pr review`, or posting comments.

## When To Use

Use this skill when the user asks to:

- clone a GitHub repository
- pull or sync the latest changes
- inspect the current repository, branch, or worktree status
- create or switch branches
- review a pull request or address review comments
- commit local changes
- push a branch
- create or update a pull request

Do not use this skill for:

- non-GitHub git hosting unless the commands are still compatible
- account or organization administration
- CI/CD administration outside normal repository workflows

## Workflow

1. Run `scripts/check-env.sh` before the first GitHub action in a session, or when auth/tooling may be missing.
2. Run `scripts/repo-context.sh` before choosing a git action inside an existing repository.
3. Choose the task flow based on the repo state and the user request.
4. Perform local read or local edit actions directly when safe.
5. Before any remote write action, summarize the exact command or action and ask for confirmation.
6. After the action, report the result and the next likely step.

## Task Flows

### Clone Or Enter A Repository

- If not in a git repository, ask for the GitHub repo URL or owner/name if it is not already known.
- Prefer `gh repo clone OWNER/REPO` when `gh` is available and the repo is on GitHub.
- Otherwise use `git clone`.
- After cloning, inspect branch and worktree state before making changes.

### Sync Local Work

- Inspect current branch, remote, default branch, and worktree state first.
- If the worktree is dirty, do not switch branches or pull blindly.
- Prefer `git fetch --all --prune` before deciding whether to rebase, merge, or switch.
- Use the repository's default branch as the base unless the user specifies otherwise.

### Create Or Switch Branches

- Confirm whether uncommitted changes exist before switching.
- Prefer branch creation from the latest base branch state.
- Use descriptive branch names when creating a new branch if the user has not specified one.

### Address PR Review Comments

- Inspect the PR first with `gh pr view` or `gh pr checkout`.
- Gather review threads, comments, changed files, and CI status before editing.
- Fix the requested changes locally.
- Summarize what changed before committing.
- Ask for confirmation before pushing and before submitting a review response.

### Commit Changes

- Review `git status --short` and relevant diffs first.
- Group changes into a coherent commit.
- Write concise commit messages in imperative mood.
- Do not commit unrelated worktree changes without calling them out.

### Push Or Open A PR

- Confirm remote branch, target base branch, and whether the branch already exists on remote.
- Ask before `git push`, `gh pr create`, `gh pr edit`, `gh pr review`, or `gh pr comment`.
- When opening a PR, include a concise summary, testing note, and any review context.

## Safety Rules

- Never assume the current branch is safe to modify without checking.
- Never discard local changes unless the user explicitly asks.
- Never perform remote write operations without confirmation.
- If auth, remotes, or base branch are unclear, stop and inspect before proceeding.

## Outputs

- Keep updates concise and action-oriented.
- When asking for confirmation, include the concrete remote action.
- When reporting results, include branch, PR number if relevant, and any remaining manual step.

## Resources

- Use `scripts/check-env.sh` for tool and auth checks.
- Use `scripts/repo-context.sh` for repository context.
- Use `references/command-patterns.md` for recommended command shapes and common flows.
