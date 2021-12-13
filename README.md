# QOI - The “Quite OK Image” format for fast, lossless image compression

Single-file MIT licensed library for C/C++

See [qoi.h](https://github.com/phoboslab/qoi/blob/master/qoi.h) for
the documentation.

More info at https://phoboslab.org/log/2021/11/qoi-fast-lossless-image-compression

⚠️ Please note that this library is not yet ready to deal with untrusted input.

⚠️ 2021.11.30 – the file format is not yet finalized. We're still working to fix
some smaller issues. The final specification will be announced on 2021.12.20.
Thanks for your patience! The WIP file format specification can be found in
[qoi.h](https://github.com/phoboslab/qoi/blob/master/qoi.h)


## Why?

Compared to stb_image and stb_image_write QOI offers 20x-50x faster encoding,
3x-4x faster decoding and 20% better compression. It's also stupidly simple and
fits in about 300 lines of C.


## Example Usage

- [qoiconv.c](https://github.com/phoboslab/qoi/blob/master/qoiconv.c)
converts between png <> qoi
 - [qoibench.c](https://github.com/phoboslab/qoi/blob/master/qoibench.c)
a simple wrapper to benchmark stbi, libpng and qoi


## Tools

- https://github.com/floooh/qoiview


## Implementations of QOI

- https://github.com/MasterQ32/zig-qoi (Zig)
- https://github.com/steven-joruk/qoi (Rust)
- https://github.com/ChevyRay/qoi_rs (Rust)
- https://github.com/zakarumych/rapid-qoi (Rust)
- https://github.com/xfmoulet/qoi (Go)
- https://github.com/panzi/jsqoi (TypeScript)
- https://github.com/0xd34df00d/hsqoi (Haskell)
- https://github.com/pfusik/qoi-ci (Ć)
- https://github.com/kodonnell/qoi (Python)
- https://github.com/NUlliiON/QoiSharp (C#)
- https://github.com/rbino/qoix (Elixir)
- https://github.com/elihwyma/Swift-QOI (Swift)
- https://github.com/saharNooby/qoi-java (Java)


## Packages

[AUR](https://aur.archlinux.org/pkgbase/qoi-git/) - system-wide qoi.h, qoiconv and qoibench install as split packages.

