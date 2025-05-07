# Password manager
### Maxime Prevost


This project is a password manager with GUI interface developped in C using wxwidget.
***

The user will enter a master password.
To check if the master password is the good one, we will create a test file containing the hash of the password through the sha-256 algorithm.

All the passwords and usernames stored in the file will be encrypted using the obfuscated masterkey and a XOR operation (not the safer way but still working).

The user will get the opportunity to generate a random password. He will be able to choose the kind of alphanumeric parameter he want and the len of the password.

## Credits

The sha256 algorithm used in this repo come's from https://lucidar.me/fr/dev-c-cpp/sha-256-in-c-cpp/