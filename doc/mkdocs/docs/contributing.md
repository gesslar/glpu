The `main` branch is our stable branch and is protected to ensure the integrity of the codebase. Only specific users and automated tools can push directly to `main`.

### Development Branch

The `dev` branch is used for ongoing development. All feature branches should be created off the `dev` branch. Changes to `dev` should be reviewed and tested before being merged into `main`.

### How to Contribute

1. **Create a Branch**: Branch off from the `dev` branch.

        git checkout dev
        git pull origin dev
        git checkout -b your-feature-branch

2. Make Your Changes: Commit your changes to your branch.
3. Create a Pull Request: Open a pull request to merge your changes into dev.
4. Review and Merge: Once your pull request is reviewed and approved, it will be merged into dev.
