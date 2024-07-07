To integrate gLPU with GitHub issues, you need to setup a Personal Access Token
(PAT) in your GitHub account. This token is used to authenticate the your game
with GitHub.

# Create a Personal Access Token

1. Go to your GitHub account settings (https://github.com/settings/tokens).
2. Click on *Generate new token*.
3. You will need to use a fine-grained token with access to either All
   repositories, or select repositories (like your game repo).
4. For permissions, you need to select Issues (read/write).
5. Click on *Generate token*. You will see a token like
   `ghp_pat_1234567890abcdefghij1234567890abcdefghij`. Record this token.

# Setting up the token in your game

1. You can safely store your token in in `/adm/etc/config.json`. You should
   define the following properties:

    * `GITHUB_REPORTER_OWNER`: The owner of the repository
    * `GITHUB_REPORTER_REPO`: The repository name
    * `GITHUB_REPORTER_PAT`: The Personal Access Token
    * `GITHUB_REPORTER_TYPES`: An array of valid issue labels

2. Once you've updated the `config.json`, execute the `master` command to
   update the configuration daemon.

# Using the GitHub reporter

The GitHub daemon to report issues is `/adm/daemons/github_issues.c`. This
daemon will report issues to your GitHub repository.

To use the daemon, you need to call the `create_issue` method with the
following arguments:

* `string type`: The type of the issue. This should be one of the types
  defined in `GITHUB_REPORTER_TYPES`.
* `string title`: The title of the issue.
* `string body`: The body of the issue.
* `mixed callback`: The callback to call when the issue is created. This
  should be a function pointer or a string with the name of the function to
  call. If using a string, the callback will be called in the same object
  that called `create_issue`. So, do ensure, in both cases, that the calling
  object does not become unloaded and that, in the case of a string, the
  function exists and is public.

The daemon will then create an issue in your GitHub repository with the title
and body you provided and assign the label corresponding to the type you used.

Once it has received a positive response from GitHub, it will call the callback
with the body of the response. If there was an error, it will call the callback
with the error message in the body.

# Backlog processing

If, for any reason, the GitHub daemon is unable to create an issue, it will
store the issue in a backlog. The backlog is a list of issues that the daemon
will attempt to create when it is able to do so. This is called in the setup()
method of the daemon, however, you can also call it manually by calling the
`process_backlog` method.

# Notes

* `config.json` is by default in the `.gitignore` file, therefore you don't
  need to worry about accidentally committing your token to your repository.
* Personal Access Tokens have a required expiration. So, you will need to
  generate a new token when the current one expires. Do make note of when
  the token expires and set a reminder to generate a new token before that
  time.
* The `github_issues` daemon will report issues as the owner of the PAT.
* The `github_issues` daemon will not report issues that are not in the
  `GITHUB_REPORTER_TYPES` array.
* Backlog processing will not execute callbacks, as that information cannot be
  stored in the backlog.
