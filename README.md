# jsnfmt

A fast, lightweight JSON formatter for *nix systems. Forked from [bsouther/jsonf](https://github.com/bsouther/jsonf).

[How is GNU `yes` so fast?](https://www.reddit.com/r/unix/comments/6gxduc/how_is_gnu_yes_so_fast/) helped making it faster :)

## Why?

Sometimes I have to work with large JSON files (> 1GB). These are kind of hard to deal with if they're not formatted (i.e. a single line). The fastest JSON formatter I found performed at around 15 MB/s.

`jsnfmt` is roughly ten times faster on my system:

    ψ jsnfmt large.json | pv > /dev/null
    1032MiB 0:00:04 [ 158MiB/s] [     <=>                               ]

## Usage

`jsnfmt` always writes to `stdout`. It can read input from `stdin`

    echo '{"foo": [1,2,3], "bar": "baz"}' | jsnfmt

or from a file

    jsnfmt ~/some.json > formatted.json

It does not have any command line switches.


## Building

Prerequisites:

* C compiler (tested with clang)
* clang-format (if you wish to contribute)
* make

After installing these, you can simply run `make`.


## License

See the `LICENSE` file.