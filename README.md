
# Telegram Deobfuscator

A small script to deobfuscate data sent from and to the official [Telegram Android app](https://github.com/DrKLO/Telegram). 

## Build

- `cd` into the folder
- Run cmake `cmake .`
- Build using `make`

## Run

- Run `./deobfuscator  <outgoing stream> <incoming stream> [<key>]`
  - `<outgoing stream>` is a binary file of sniffed outgoing data
  - `<incoming stream>` is a binary file of sniffed incoming data
  - `<key>` is a binary file containing auth_key

The `<key>` argument may be omitted, however if it is present, it decrypts the IGE payload and prints content of the real messages.


## License

Copyright 2016 Tomas Susanka

Licensed under the MIT license.
