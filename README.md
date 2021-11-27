# QOI - The “Quite OK Image” format for fast, lossless image compression

Single-file MIT licensed library for C/C++

See [qoi.h](https://github.com/phoboslab/qoi/blob/master/qoi.h) for
the documentation.

More info at https://phoboslab.org/log/2021/11/qoi-fast-lossless-image-compression

⚠️ Please note that this library is not yet ready to deal with untrusted input.

⚠️ 2021.11.27 – the specification for QOI has changed to accomodate some 
concerns with the format. If you are working on a QOI implementation, please 
refer to 
[#37 The QOI File Format Specification](https://github.com/phoboslab/qoi/issues/37) 
for the details.

These specification changes are ~~not yet reflected in the code here~~ 
reflected in qoi.h now. 


## Why?

Compared to stb_image and stb_image_write QOI offers 20x-50x faster encoding,
3x-4x faster decoding and 20% better compression. It's also stupidly simple and
fits in about 300 lines of C.


## Example Usage

- [qoiconv.c](https://github.com/phoboslab/qoi/blob/master/qoiconv.c)
converts between png <> qoi
 - [qoibench.c](https://github.com/phoboslab/qoi/blob/master/qoibench.c)
a simple wrapper to benchmark stbi, libpng and qoi
