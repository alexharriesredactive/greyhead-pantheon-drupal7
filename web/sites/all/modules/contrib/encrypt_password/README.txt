CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installation
 * Credits

INTRODUCTION
------------
Password is a simple module to encrypt the hashes of user passwords. It
provides an extra level of security beyond using a salt and employing
multiple iterations of the hashing algorithm. This module does not
change or override Drupal's standard hashing functions and it will not
allow you to make users' plaintext passwords retrievable.

The settings page provides the following options: 
* Encrypt passwords: If this checkbox is selected, password encryption
  will be enabled.
* Configuration: Allows selection of a configuration to be used for
  password encryption. Configurations are provided by the Encrypt module.
* Roles: Allows only certain roles to be included for password
  encryption.

The page also displays statistics about encrypted passwords, listed by
role. When the settings form is submitted, if encryption is enabled, any
existing passwords for users with the chosen roles will be encrypted.

Encrypt Password requires the Encrypt module in order to provide the
functionality for encrypting and decrypting. Encrypt Password is
currently very stable, but since user account passwords are being
modified, it's an excellent idea to perform a backup of the users table
before enabling encryption.

INSTALLATION
------------
1. Download the module.
2. Enable the module at Administer >> Site building >> Modules.

CREDITS
-------
* Rick Hawkins (rlhawk)
* Patrick Teglia (CrashTest_ or pteglia)
* Sponsored and made possible by Townsend Security - http://townsendsecurity.com
