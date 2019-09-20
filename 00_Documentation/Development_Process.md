# Development Process

## Branching Model
The project SHALL use [git-flow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow) as _branching model_.

This introduces the following branches

| Branch Name                                | Branch Life-Time | Usage                                                            |
| -------------------------------------------|------------------|------------------------------------------------------------------|
| `master`                                   | long-lived       | has the latest release, carries all tagged releases              |
| `develop`                                  | long-lived       | source for any development started for the next released version |
| `feature/#<github-issue>_human_readable`   | short-lived      | any development for an issue happens here                        |
| `release/<A.B.C>`                          | short-lived      | preparing a new release version                                  |


### Restrictions
- developers MUST NOT commit to `master` and `develop` directly
- all new features MUST be developed on a `feature` branch and merged via _Pull Request_
- _Pull Requests_ MUST only be merged if the _quality gate_ is passed
- `feature` branches MUST start with the GitHub issue id and MAY end with human-readable information on the branches usage
- any commit message MUST start with the GitHub issue aligned to 5 chars (e.g. `#12   commit message`)
- `feature` branches SHALL be rebased onto `develop`

## Quality Gate
- CI was able to build the software on all platforms
- CI was able to execute all unit tests without any failures 
- Coverage for the code under test is sufficient (let's say >= 90% for now)
