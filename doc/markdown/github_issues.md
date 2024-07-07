To integrate gLPU with GitHub issues, you need to create a GitHub application and configure it.

## Creating the GitHub Application

1. Go to the GitHub Developer Settings page (https://github.com/settings/apps)
2. Click `New GitHub App` and fill out the details, ensuring that you have
   selected the permission for `Issues (Read & Write)`.
3. After creating the app, you will be provided with a `App ID`, record this.
4. Click on the `Generate a new client secret` button to generate a new client
   secret, record this.
5. Near the bottom of the page, click on the `Generate a private key` button to
   generate a new private key, record this. This will download a `.pem` file to
   your computer.

## Installing the GitHub Application

1. On the left hand sidebar of your application's page, click `Install App`
2. Choose an account to install the app to by clicking the `Install` button.
3. In the repo selection, if this is a generic app, you can select `All
   repositories`, otherwise select only your repository for your game.
4. You will need to obtain the installation ID, but to do that you need to
   generate a JSON Web Token (JWT) to authenticate with the GitHub API. Use
   the following script to generate the JWT, where the first argument is the
   `App ID` and the second argument is the path to the `.pem` file you
   downloaded. Save this script as `generate_jwt.py` and run it with the
   appropriate arguments, recording the output. Make sure you have the
   following libraries installed: `pyjwt`, `time`, `sys`, and `cryptography`.

```python
import jwt
import time
import sys

def generate_jwt(app_id, private_key_path):
    # Load your GitHub App's private key from the .pem file
    with open(private_key_path, 'r') as f:
        private_key = f.read()

    payload = {
        'iat': int(time.time()),
        'exp': int(time.time()) + (10 * 60),  # JWT expiration time (10 minutes)
        'iss': app_id
    }

    jwt_token = jwt.encode(payload, private_key, algorithm='RS256')
    return jwt_token

def main():
    if len(sys.argv) != 3:
        print("Usage: python generate_jwt.py <app_id> <path_to_pem_file>")
        sys.exit(1)

    app_id = sys.argv[1]
    private_key_path = sys.argv[2]

    jwt_token = generate_jwt(app_id, private_key_path)
    print("Generated JWT:", jwt_token)

if __name__ == "__main__":
    main()

```
5. To get the installation ID, run the following curl command to list
   installations and find the id field in the output.

```bash
curl -X POST \
curl -H "Authorization: Bearer YOUR_JWT_TOKEN" \
     -H "Accept: application/vnd.github.v3+json" \
     https://api.github.com/app/installations
```

6. Use the installation ID obtained in the previous step to generate an
   installation access token by running the following curl command. Replace
   `YOUR_INSTALLATION_ID` with the installation ID.

```bash
curl -X POST \
-H "Authorization: Bearer YOUR_JWT_TOKEN" \
-H "Accept: application/vnd.github.v3+json" \
https://api.github.com/app/installations/YOUR_INSTALLATION_ID/access_tokens
```

7. You will receive back a `token` that you can use to authenticate with the
   GitHub API. Record this token.

8. Finally, to create an issue, use the installation access token with the
   following curl command. Replace `YOUR_INSTALLATION_ACCESS_TOKEN` with the
   oken received in the previous step, and adjust the payload (title, body,
   labels) as needed. Substitute `YOUR_ACCOUNT` and `YOUR_REPO` with your
   GitHub account and repository.

```bash
curl -X POST \
-H "Authorization: token YOUR_INSTALLATION_ACCESS_TOKEN" \
-H "Accept: application/vnd.github.v3+json" \
-d '{
  "title": "Bug: Unexpected behavior",
  "body": "Details about the bug.",
  "labels": ["bug"]
}' \
https://api.github.com/repos/YOUR_ACCOUNT/YOUR_REPO/issues
```

In-game, you can create a custom HTTP Client to manage the GitHub API Issues
requests.
