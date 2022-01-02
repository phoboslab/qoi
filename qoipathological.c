/*

Command line tool to generate pathological qoi format files

Compile with: 
	gcc qoipathological.c -std=c99 -lm -o qoipathological

Dominic Szablewski - https://phoboslab.org

-- LICENSE: The MIT License(MIT)

Copyright(c) 2021 Dominic Szablewski

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files(the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions :
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <stdlib.h>
#include <math.h>

#define QOI_IMPLEMENTATION
#include "qoi.h"

#define PI           3.14159265358979323846  /* pi */

void gen_ramp(int w, int h, void * bytes) {
    int p = 0;
    
    while(p < w * h * 4) {
        int x = p % (w * 4);
        int r = x % 256;
        int g = x % 256;
        int b = x % 256;
        int a = 0xff;
        int pixel = r << 24 | g << 16 | b << 8 | a;
        qoi_write_32(bytes, &p, pixel);
    }
}

void gen_wave(int w, int h, void * bytes) {
    int p = 0;
    
    while(p < w * h * 4) {
        float x = (p%w) / (float) w;
        float y = (p/w) / (float) h;
        int r = 0x7f + round(sin(2*PI*x) * 0x7f);
        int g = 0x7f;
        int b = 0x7f + round(cos(2*PI*y) * 0x7f);
        int a = round(sqrt(x*x+y*y)*0x7f);
        a %= 0xff;
        int pixel = r << 24 | g << 16 | b << 8 | a;
        qoi_write_32(bytes, &p, pixel);
    }
}

void gen_tiles(int w, int h, void * bytes) {
    int p = 0;
    
    while(p < w * h * 4) {
        float x = (p%w) / (float) w;
        float y = (p/w) / (float) h;
        while(y>1) y-=1;
        int r = (x < 0.5) ? 0x00 : 0xff;
        int g = 0x7f;
        int b = (y < 0.5) ? 0x00 : 0xff;
        int a = 0xff;
        int pixel = r << 24 | g << 16 | b << 8 | a;
        qoi_write_32(bytes, &p, pixel);
    }
}

void gen_radial(int w, int h, void * bytes) {
    int p = 0;
    
    while(p < w * h * 4) {
        float x = ((p/4) % w) / (float) w - 0.5;
        float y = ((p/4) / w) / (float) h - 0.5;
        int r = round((1-sqrt(x*x+y*y))*0xff);
        int g = round((1-sqrt(x*x+y*y))*0xff);
        int b = round((1-sqrt(x*x+y*y))*0xff);
        int a = round((1-sqrt(x*x+y*y))*0xff);
        if(r<0) r = 0;
        if(g<0) g = 0;
        if(b<0) b = 0;
        if(a<0) a = 0;
        int pixel = r << 24 | g << 16 | b << 8 | a;
        qoi_write_32(bytes, &p, pixel);
    }
}

void gen_diagonal(int w, int h, void * bytes) {
    int p = 0;
    
    while(p < w * h * 4) {
        float x = ((p/4) % w) / (float) w;
        float y = ((p/4) / w) / (float) h;
        int r = round(0xff-(x-y)*(x-y)*0xff);
        int g = round(0xff-(x-y)*(x-y)*0xff);
        int b = round(0xff-(x-y)*(x-y)*0xff);
        int a = round(0xff-(x-y)*(x-y)*0xff);
        if(r<0) r = 0;
        if(g<0) g = 0;
        if(b<0) b = 0;
        if(a<0) a = 0;
        int pixel = (r%0xff) << 24 | (g%0xff) << 16 | (b%0xff) << 8 | a;
        qoi_write_32(bytes, &p, pixel);
    }
}

void gen_modulo(int w, int h, void * bytes) {
    int p = 0;
    
    while(p < w * h * 4) {
        int x = p % (w*4);
        int y = p / (w);
        int r = x % 0xff;
        int g = 0x7f;
        int b = y % 0xff;
        int a = (x*y) % 0xff;
        int pixel = r << 24 | g << 16 | b << 8 | a;
        qoi_write_32(bytes, &p, pixel);
    }
}


int main(const int argc, const char *argv[]) {
    void *bytes = NULL;
    int w = 640;
    int h = 640;
    int channels = 4;
    int encoded = 0;
    
    bytes = malloc(w * h * 4);

    gen_ramp(w,h,bytes);
    encoded = qoi_write("ramp.qoi", bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		});

    gen_wave(w,h,bytes);
    encoded = qoi_write("wave.qoi", bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		});

    gen_diagonal(w,h,bytes);
    encoded = qoi_write("diagonal.qoi", bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		});
		
    gen_radial(w,h,bytes);
    encoded = qoi_write("radial.qoi", bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		});

    gen_modulo(w,h,bytes);
    encoded = qoi_write("modulo.qoi", bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		});
		
    int p = 0;	
    qoi_write_32(bytes, &p, 0xffddbb99);
    encoded = qoi_write("point.qoi", bytes, &(qoi_desc){
			.width = 1,
			.height = 1, 
			.channels = channels,
			.colorspace = QOI_SRGB
		});
				
    gen_tiles(w,h,bytes);
    {			
        int p = 0;
	FILE *f = fopen("underflow.qoi", "wb");
	int size;
	void *encoded;

	if (!f) {
		return 0;
	}

	encoded = qoi_encode(bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		}, &size);
	if (!encoded) {
		fclose(f);
		return 0;
	}

	qoi_write_32(encoded, &p, QOI_MAGIC);
	qoi_write_32(encoded, &p, w/8);
	qoi_write_32(encoded, &p, h/8);
	
	fwrite(encoded, 1, size, f);
	fclose(f);

	QOI_FREE(encoded);
    }
    
    gen_tiles(w/8,h/8,bytes);
    {			
        int p = 0;
	FILE *f = fopen("overflow.qoi", "wb");
	int size;
	void *encoded;

	if (!f) {
		return 0;
	}

	encoded = qoi_encode(bytes, &(qoi_desc){
			.width = w/8,
			.height = h/8, 
			.channels = channels,
			.colorspace = QOI_SRGB
		}, &size);
	if (!encoded) {
		fclose(f);
		return 0;
	}

	qoi_write_32(encoded, &p, QOI_MAGIC);
	qoi_write_32(encoded, &p, w);
	qoi_write_32(encoded, &p, h);
	
	fwrite(encoded, 1, size, f);
	fclose(f);

	QOI_FREE(encoded);
    }
			
    gen_tiles(w,h,bytes);
    {			
        int p = 0;
	FILE *f = fopen("truncated.qoi", "wb");
	int size;
	void *encoded;

	if (!f) {
		return 0;
	}

	encoded = qoi_encode(bytes, &(qoi_desc){
			.width = w,
			.height = h, 
			.channels = channels,
			.colorspace = QOI_SRGB
		}, &size);
	if (!encoded) {
		fclose(f);
		return 0;
	}

	qoi_write_32(encoded, &p, QOI_MAGIC);
	qoi_write_32(encoded, &p, w);
	qoi_write_32(encoded, &p, h);
	
	qoi_write_32(encoded, &p, 0);
	qoi_write_32(encoded, &p, 1);
	
	fwrite(encoded, 1, p, f);
	fclose(f);

	QOI_FREE(encoded);
    }			
    	
    if(bytes) free(bytes);			
    return 0;
}
