# GitHub Workflow Command Patterns

Use these command patterns when the skill needs concrete `git` or `gh` commands.

## Environment Check

```bash
./github-workflow/scripts/check-env.sh
```

## Repository Context

```bash
./github-workflow/scripts/repo-context.sh
```

## Clone

Prefer GitHub CLI when cloning from GitHub:

```bash
gh repo clone OWNER/REPO
```

Fallback:

```bash
git clone git@github.com:OWNER/REPO.git
```

## Sync Default Branch

```bash
git fetch --all --prune
git switch main
git pull --ff-only
```

Replace `main` with the detected default branch when needed.

## Create A Feature Branch

```bash
git fetch origin
git switch main
git pull --ff-only
git switch -c feature/short-description
```

## Inspect A PR

```bash
gh pr view 123 --comments --web
```

CLI-focused view:

```bash
gh pr view 123 --comments
gh pr diff 123
```

Checkout the PR branch locally:

```bash
gh pr checkout 123
```

## Address Review Comments

```bash
gh pr view 123 --comments
gh pr checkout 123
git status --short
git diff
```

After local fixes:

```bash
git add -p
git commit -m "Address PR review feedback"
```

Push only after confirmation:

```bash
git push origin HEAD
```

## Open A Pull Request

```bash
gh pr create --base main --fill
```

More explicit version:

```bash
gh pr create \
  --base main \
  --title "Short title" \
  --body "Summary\n\n## Testing\n- tests or checks run"
```

## Update PR Metadata

```bash
gh pr edit 123 --title "Updated title" --body-file /path/to/body.md
```

## Comment Or Review

Comment:

```bash
gh pr comment 123 --body "Updated this based on feedback."
```

Approve:

```bash
gh pr review 123 --approve --body "Looks good."
```

Request changes:

```bash
gh pr review 123 --request-changes --body "Please address the noted issues."
```

## Commit Hygiene

Inspect:

```bash
git status --short
git diff
git diff --cached
```

Commit:

```bash
git add -p
git commit -m "Implement feature X"
```

## Safety Notes

- Inspect worktree state before pull, switch, rebase, or merge.
- Do not push, create PRs, or submit reviews without explicit confirmation.
- Do not assume `main`; prefer the detected default branch.
