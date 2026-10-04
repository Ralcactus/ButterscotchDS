#include "nds_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"*/

bool nds_load_bin_5551(const char* path, int frame, uint16_t** out_pixels, int* out_w, int* out_h)
{
	uint8_t magic[4];
	uint16_t nameLen = 0;
	uint16_t w = 0;
	uint16_t h = 0;
	uint32_t size = 0;
	uint16_t* pixels = NULL;
	uint8_t* row = NULL;

	//Open the bin file
	FILE* binfile = fopen(path, "rb");
	if (!binfile)
		return false;

	fread(magic, 1, 4, binfile);

	uint8_t b2[2];
	fseek(binfile, 8, SEEK_CUR);
	fread(b2, 1, 2, binfile);
	nameLen = (uint16_t)(b2[0] | (b2[1] << 8));

	fseek(binfile, nameLen, SEEK_CUR);

	for (int i = 0; ; i++)
	{
		uint8_t bw[2];
		uint8_t bh[2];
		uint8_t b4[4];

		fread(bw, 1, 2, binfile);
		fread(bh, 1, 2, binfile);
		fread(b4, 1, 4, binfile);

		w = (uint16_t)(bw[0] | (bw[1] << 8));
		h = (uint16_t)(bh[0] | (bh[1] << 8));

		size = (uint32_t)b4[0] | ((uint32_t)b4[1] << 8) | ((uint32_t)b4[2] << 16) | ((uint32_t)b4[3] << 24);

		if (i == frame)
			break;

		fseek(binfile, size, SEEK_CUR);
	}

	pixels = (uint16_t*)malloc((size_t)w * h * sizeof(uint16_t));
	row = (uint8_t*)malloc((size_t)w * 4);

	/*int w = 0;
	int h = 0;
	int n = 0;
	unsigned char* rgba = stbi_load(path, &w, &h, &n, 4);
	if (!rgba || w <= 0 || h <= 0)
		return false;

	uint16_t* pixels = (uint16_t*)malloc((size_t)w * (size_t)h * sizeof(uint16_t));*/
	
	if (!pixels){
		//stbi_image_free(rgba);
		return false;
	}

	for (int y = 0; y < h; y++)
	{
		if (fread(row, 1, (size_t)w * 4, binfile) != (size_t)w * 4)
			return false;
		for (int x = 0; x < w; x++)
		{
			uint8_t r = row[x * 4 + 0];
			uint8_t g = row[x * 4 + 1];
			uint8_t b = row[x * 4 + 2];
			uint8_t a = row[x * 4 + 3];

			uint16_t rgb15 = (uint16_t)(((r >> 3) & 31) | (((g >> 3) & 31) << 5) | (((b >> 3) & 31) << 10));

			if (a >= 128)
				rgb15 |= (1u << 15);

			pixels[y * w + x] = rgb15;
		}
	}

	/*for (int i = 0; i < w * h; i++)
	{
		unsigned char r = rgba[(i * 4) + 0];
		unsigned char g = rgba[(i * 4) + 1];
		unsigned char b = rgba[(i * 4) + 2];
		unsigned char a = rgba[(i * 4) + 3];

		uint16_t rgb15 = (uint16_t)(((r >> 3) & 31) | (((g >> 3) & 31) << 5) | (((b >> 3) & 31) << 10));
		if (a >= 128)
			rgb15 |= (1u << 15);
		pixels[i] = rgb15;
	}*/

	//stbi_image_free(rgba);

	free(row);
	fclose(binfile);
	*out_pixels = pixels;
	*out_w = w;
	*out_h = h;
	return true;
}
