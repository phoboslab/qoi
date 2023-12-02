![QOI Logo](https://qoiformat.org/qoi-logo.svg)

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


## MIME Type, File Extension

The recommended MIME type for QOI images is `image/qoi`. While QOI is not yet
officially registered with IANA, I believe QOI has found enough adoption to
prevent any future image format from choosing the same name, thus making a 
MIME type collision highly unlikely ([see #167](https://github.com/phoboslab/qoi/issues/167)).

The recommended file extension for QOI images is `.qoi`


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


## Improvements, New Versions and Contributing

The QOI format has been finalized. It was a conscious decision to **not** have a
version number in the file header. If you have a working QOI implementation today, 
you can rest assured that it will be compatible with all QOI files tomorrow.

There are a lot of interesting ideas for a successor of QOI, but none of these will 
be implemented here. That doesn't mean you shouldn't experiment with QOI, but please
be aware that pull requests that change the format will not be accepted.

Likewise, pull requests for performance improvements will probably not be accepted
either, as this "reference implementation" tries to be as easy to read as possible.


## Tools

- [floooh/qoiview](https://github.com/floooh/qoiview) - native QOI viewer
- [pfusik/qoi-fu](https://github.com/pfusik/qoi-fu/releases) - QOI Plugin installer for Windows Explorer, Finder, GNOME, GIMP, Imagine, Paint.NET and XnView
- [iOrange/QoiFileTypeNet](https://github.com/iOrange/QoiFileTypeNet/releases) - QOI Plugin for Paint.NET
- [iOrange/QOIThumbnailProvider](https://github.com/iOrange/QOIThumbnailProvider) - Add thumbnails for QOI images in Windows Explorer
- [Tom94/tev](https://github.com/Tom94/tev) - another native QOI viewer (allows pixel peeping and comparison with other image formats)
- [qoiconverterx](https://apps.apple.com/br/app/qoiconverterx/id1602159820) QOI <=> PNG converter available on the Mac App Store
- [kaetemi/qoi-ma](https://github.com/kaetemi/qoi-max) - QOI Bitmap I/O Plugin for 3ds Max
- [rtexviewer](https://raylibtech.itch.io/rtexviewer) - texture viewer, supports QOI
- [rtexpacker](https://raylibtech.itch.io/rtexpacker) - texture packer, supports QOI
- [DmitriySalnikov/godot_qoi](https://github.com/DmitriySalnikov/godot_qoi) - QOI GDNative Addon for Godot Engine
- [dan9er/farbfeld-convert-qoi](https://gitlab.com/dan9er/farbfeld-convert-qoi) - QOI <=> farbfeld converter
- [LTMX/Unity.QOI](https://github.com/LTMX/Unity.QOI) - QOI Importer and Exporter for the Unity3D Game Engine
- [Ben1138/unity-qoi](https://github.com/Ben1138/unity-qoi) - QOI Importer(only) support for the Unity3D Game Engine
- [xiaozhuai/jetbrains-qo](https://github.com/xiaozhuai/jetbrains-qoi) - [QOI Support](https://plugins.jetbrains.com/plugin/19352-qoi-support) for Jetbrains' IDE.
- [serge-ivamov/QOIql](https://github.com/serge-ivamov/QOIql) - MacOS QuickLook plugin for QOI
- [tobozo/kde-thumbnailer-qoi](https://github.com/tobozo/kde-thumbnailer-qoi) - QOI Thumbnailer for KDE
- [walksanatora/qoi-thumbnailer-nemo](https://github.com/walksanatora/qoi-thumbnailer-nemo) - QOI Thumbnailer for Nemo
- [hzeller/timg](https://github.com/hzeller/timg) - a terminal image viewer with QOI support
- [LuisAlfredo92/Super-QOI-converter](https://github.com/LuisAlfredo92/Super-QOI-converter "LuisAlfredo92/Super-QOI-converter") - A program to convert JPG, JPEG, BMP, and PNG to QOI
	- [Console version](https://github.com/LuisAlfredo92/Super-QOI-converter-Console- "Console version"): Available for Linux, OSX and Windows
	- [GUI version](https://github.com/LuisAlfredo92/Super-QOI-converter-GUI- "GUI version"): Available only for windows
- [tacent view](https://github.com/bluescan/tacentview) - Image and texture viewer, supports QOI
- [colemanrgb/qoi2spr](https://github.com/colemanrgb/qoi2spr) - A variety of applications for decoding and encoding of QOI images on [RISC OS](https://www.riscosopen.org/)

## Implementations & Bindings of QOI

- [pfusik/qoi-fu](https://github.com/pfusik/qoi-fu) - Fusion, transpiling to C, C++, C#, D, Java, JavaScript, Python, Swift and TypeScript
- [kodonnell/qoi](https://github.com/kodonnell/qoi) - Python
- [JaffaKetchup/dqoi](https://github.com/JaffaKetchup/dqoi) - Dart, with Flutter support
- [Cr4xy/lua-qoi](https://github.com/Cr4xy/lua-qoi) - Lua
- [superzazu/SDL_QOI](https://github.com/superzazu/SDL_QOI) - C, SDL2 bindings
- [saharNooby/qoi-java](https://github.com/saharNooby/qoi-java) - Java
- [MasterQ32/zig-qoi](https://github.com/MasterQ32/zig-qoi) - Zig
- [rbino/qoix](https://github.com/rbino/qoix) - Elixir
- [NUlliiON/QoiSharp](https://github.com/NUlliiON/QoiSharp) - C#
- [aldanor/qoi-rust](https://github.com/aldanor/qoi-rust) - Rust
- [zakarumych/rapid-qoi](https://github.com/zakarumych/rapid-qoi) - Rust
- [takeyourhatoff/qoi](https://github.com/takeyourhatoff/qoi) - Go
- [DosWorld/pasqoi](https://github.com/DosWorld/pasqoi) - Pascal
- [elihwyma/Swift-QOI](https://github.com/elihwyma/Swift-QOI) - Swift
- [xfmoulet/qoi](https://github.com/xfmoulet/qoi) - Go
- [erratique.ch/qoic](https://erratique.ch/software/qoic) - OCaml
- [arian/go-qoi](https://github.com/arian/go-qoi) - Go
- [kchapelier/qoijs](https://github.com/kchapelier/qoijs) - JavaScript
- [KristofferC/QOI.jl](https://github.com/KristofferC/QOI.jl) - Julia
- [shadowMitia/libqoi](https://github.com/shadowMitia/libqoi) - C++
- [MKCG/php-qoi](https://github.com/MKCG/php-qoi) - PHP
- [LightHouseSoftware/qoiformats](https://github.com/LightHouseSoftware/qoiformats) - D
- [mhoward540/qoi-nim](https://github.com/mhoward540/qoi-nim) - Nim
- [wx257osn2/qoixx](https://github.com/wx257osn2/qoixx) - C++
- [Tiefseetauchner/lr-paint](https://github.com/Tiefseetauchner/lr-paint) - Processing
- [amstan/qoi-fpga](https://github.com/amstan/qoi-fpga) - FPGA: verilog
- [musabkilic/qoi-decoder](https://github.com/musabkilic/qoi-decoder) - Python
- [mathpn/py-qoi](https://github.com/mathpn/py-qoi) - Python
- [JohannesFriedrich/qoi4R](https://github.com/JohannesFriedrich/qoi4R) - R
- [shraiwi/mini-qoi](https://github.com/shraiwi/mini-qoi) - C, streaming decoder
- [10maurycy10/libqoi/](https://github.com/10maurycy10/libqoi/) - Rust
- [0xd34df00d/hsqoi](https://github.com/0xd34df00d/hsqoi) - Haskell
- [418Coffee/qoi-v](https://github.com/418Coffee/qoi-v) - V
- [Imagine-Programming/QoiImagePlugin](https://github.com/Imagine-Programming/QoiImagePlugin) - PureBasic
- [Fabien-Chouteau/qoi-spark](https://github.com/Fabien-Chouteau/qoi-spark) - Ada/SPARK formally proven
- [mzgreen/qoi-kotlin](https://github.com/mzgreen/qoi-kotlin) - Kotlin Multiplatform
- [Aftersol/Simplified-QOI-Codec](https://github.com/Aftersol/Simplified-QOI-Codec) - C99, encoder and decoder, freestanding
- [AuburnSounds/gamut](https://github.com/AuburnSounds/gamut) - D
- [AngusJohnson/TQoiImage](https://github.com/AngusJohnson/TQoiImage) - Delphi
- [MarkJeronimus/qoi-java-spi](https://github.com/MarkJeronimus/qoi-java-spi) - Java SPI
- [aumouvantsillage/qoi-racket](https://github.com/aumouvantsillage/qoi-racket) - Racket
- [rubikscraft/qoi-stream](https://github.com/rubikscraft/qoi-stream) - C99, one byte at a time streaming encoder and decoder
- [rubikscraft/qoi-img](https://github.com/rubikscraft/qoi-img) - NodeJS typescript, bindings to both [QOIxx](https://github.com/wx257osn2/qoixx) and [qoi-stream](https://github.com/rubikscraft/qoi-stream)
- [grego/hare-qoi](https://git.sr.ht/~grego/hare-qoi) - Hare
- [MrNocole/ZTQOI](https://github.com/MrNocole/ZTQOI) - Objective-C
- [bpanthi977/qoi](https://github.com/bpanthi977/qoi) - Common Lisp
- [Floessie/pam2qoi](https://github.com/Floessie/pam2qoi) - C++
- [SpeckyYT/spwn-qoi](https://github.com/SpeckyYT/spwn-qoi) - SPWN
- [n00bmind/qoi](https://github.com/n00bmind/qoi) - Jai
- [SixLabors/ImageSharp](https://github.com/SixLabors/ImageSharp) - C# image proccesing library

## QOI Support in Other Software

- [Amiga OS QOI datatype](https://github.com/dgaw/qoi-datatype) - adds support for decoding QOI images to the Amiga operating system.
- [SerenityOS](https://github.com/SerenityOS/serenity) - supports decoding QOI system wide through a custom [cpp implementation in LibGfx](https://github.com/SerenityOS/serenity/blob/master/Userland/Libraries/LibGfx/QOILoader.h)
- [Raylib](https://github.com/raysan5/raylib) - supports decoding and encoding QOI textures through its [rtextures module](https://github.com/raysan5/raylib/blob/master/src/rtextures.c)
- [Rebol3](https://github.com/Oldes/Rebol3/issues/39) - supports decoding and encoding QOI using a native codec
- [c-ray](https://github.com/vkoskiv/c-ray) - supports QOI natively
- [SAIL](https://sail.software) - image decoding library, supports decoding and encoding QOI images
- [Orx](https://github.com/orx/orx) - 2D game engine, supports QOI natively
- [IrfanView](https://www.irfanview.com) - supports decoding and encoding QOI through its Formats plugin
- [ImageMagick](https://github.com/ImageMagick/ImageMagick) - supports decoding and encoding QOI, since 7.1.0-20
- [barebox](https://barebox.org) - bootloader, supports decoding QOI images for splash logo, since v2022.03.0
- [KorGE](https://korge.org) - & KorIM Kotlin 2D game engine and imaging library, supports decoding and encoding QOI natively since 2.7.0
- [DOjS](https://github.com/SuperIlu/DOjS) - DOS JavaScript Canvas implementation supports loading QOI files
- [XnView MP](https://www.xnview.com/en/xnviewmp/) - supports decoding QOI since 1.00
- [ffmpeg](https://ffmpeg.org/) - supports decoding and encoding QOI since 5.1
- [JPEGView](https://github.com/sylikc/jpegview) - lightweight Windows image viewer, supports decoding and encoding of QOI natively, since 1.1.44
- [darktable](https://github.com/darktable-org/darktable) - photography workflow application and raw developer, supports decoding since 4.4.0
- [KDE](https://kde.org) - supports decoding and encoding QOI images. Implemented in [KImageFormats](https://invent.kde.org/frameworks/kimageformats)
- [EFL](https://www.enlightenment.org) - supports decoding and encoding QOI images since 1.27.
- [Swingland](https://git.sr.ht/~phlash/swingland) - supports QOI decoding/loading via the `ImageIO` API of this Java Swing reimplemenation for Wayland

## Packages

- [AUR](https://aur.archlinux.org/pkgbase/qoi-git/) - system-wide qoi.h, qoiconv and qoibench install as split packages.
- [Debian](https://packages.debian.org/bookworm/source/qoi) - packages for binaries and qoi.h
- [Ubuntu](https://launchpad.net/ubuntu/+source/qoi) - packages for binaries and qoi.h

Packages for other systems [tracked at Repology](https://repology.org/project/qoi/versions).
