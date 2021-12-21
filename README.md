# QOI - The “Quite OK Image Format” for fast, lossless image compression

Single-file MIT licensed library for C/C++

See [qoi.h](https://github.com/phoboslab/qoi/blob/master/qoi.h) for
the documentation and format specification.

More info at https://qoiformat.org


## Why?

Compared to stb_image and stb_image_write QOI offers 20x-50x faster encoding,
3x-4x faster decoding and 20% better compression. It's also stupidly simple and
fits in about 300 lines of C.


## Example Usage

- [qoiconv.c](https://github.com/phoboslab/qoi/blob/master/qoiconv.c)
converts between png <> qoi
 - [qoibench.c](https://github.com/phoboslab/qoi/blob/master/qoibench.c)
a simple wrapper to benchmark stbi, libpng and qoi


## Limitations

The QOI file format allows for huge images with up to 18 exa-pixels. A streaming 
en-/decoder can handle these with minimal RAM requirements, assuming there is 
enough storage space.

This particular implementation of QOI however is limited to images with a 
maximum size of 400 million pixels. It will safely refuse to en-/decode anything
larger than that. This is not a streaming en-/decoder. It loads the whole image
file into RAM before doing any work and is not extensively optimized for 
performance (but it's still very fast).

If this is a limitation for your use case, please look into any of the other 
implementations listed below.


## Tools

- https://github.com/floooh/qoiview - native QOI viewer
- https://github.com/pfusik/qoi-ci/releases/tag/qoi-ci-1.0.0 - QOI Plugin installer for GIMP, Paint.NET and XnView MP
- https://github.com/iOrange/QoiFileTypeNet/releases/tag/v0.2 - QOI Plugin for Paint.NET


## Implementations of QOI

- https://github.com/MasterQ32/zig-qoi (Zig)
- https://github.com/steven-joruk/qoi (Rust)
- https://github.com/ChevyRay/qoi_rs (Rust)
- https://github.com/zakarumych/rapid-qoi (Rust)
- https://github.com/xfmoulet/qoi (Go)
- https://github.com/panzi/jsqoi (TypeScript)
- https://github.com/0xd34df00d/hsqoi (Haskell)
- https://github.com/pfusik/qoi-ci (Ć, transpiled to C, C++, C#, Java, JavaScript, Python and Swift)
- https://github.com/kodonnell/qoi (Python)
- https://github.com/NUlliiON/QoiSharp (C#)
- https://github.com/rbino/qoix (Elixir)
- https://github.com/elihwyma/Swift-QOI (Swift)
- https://github.com/saharNooby/qoi-java (Java)
- https://github.com/Cr4xy/lua-qoi (Lua)
- https://github.com/superzazu/SDL_QOI (C, SDL2 bindings)
- https://github.com/SerenityOS/serenity (in system-wide [LibGfx](https://github.com/SerenityOS/serenity/blob/master/Userland/Libraries/LibGfx/QOILoader.h))


## Packages

[AUR](https://aur.archlinux.org/pkgbase/qoi-git/) - system-wide qoi.h, qoiconv and qoibench install as split packages.
