#include "bmp.h"

uint8_t *read_rgb(FILE *fp, uint32_t width, uint32_t height, uint16_t bpp){
    uint16_t bytespp = bpp/8;
    uint8_t *rgb  = malloc(sizeof(uint16_t) * width * height * bytespp);
    if(!rgb){
        fprintf(stdout, "Cannot allocate memory for results string in all experiment\n");
        return 0;
    }  
     for (uint32_t i = 0; i < width * height * bytespp; i++) {
        rgb[i] = 0;
        fread(&rgb[i], 1, 1, fp);
    }

    return rgb;
}

uint8_t *read_palette(FILE *fp, uint32_t colors){
    uint8_t table_size = colors * 4;// colors_used * bytes of color table
    uint8_t *palette = malloc(sizeof(uint16_t) * table_size);
    if(!palette){
        fprintf(stdout, "Cannot allocate memory for results string in all experiment\n");
        return 0;
    }
    for(uint32_t i = 0; i < table_size; i++){
        palette[i] = 0;
        fread(&palette[i],1,1,fp);
    }
    return palette;

} 


IMAGE *read_Image(FILE *fp,BMPINFOHEADER *info){
    IMAGE picture = calloc(1,sizeof(IMAGE));
    if (!picture) {
        fprintf(stdout, "Cannot allocate memory for results string in all experiment\n");
        return NULL;
    }


    picture->info = info;

    if (picture->info->bpp == 8) {
        picture->palette = read_palette(fp, picture->info->color_used);
        if (!picture->palette) {
            free(picture);
            return NULL;
        }
    }

    picture->rgb = read_rgb(fp,picture->info->width,picture->info->height,picture->info->bpp);
    if(!picture->rgb){
        free(image);
        return NULL;
    }

    return picture;

}

void rgb_convert(IMAGE *picture){
    int bytespp = picture->info->bpp / 8;
    for(uint32_t i = 0; i< picture->info->width * picture->info->height * bytespp ; i++){
        picture->rgb[i] = ~picture->rgb[i];
    }
}

void palette_convert(IMAGE *picture){
    for(uint32_t i = 0; i < picture->info->color_used * 4; i++ ){
        if((i+1 )%4 == 0){
            picture->info->palette[i] = ~picture->info->palette[i];
        }
    }
}
int create_BMPImage(BMPFILEHEADER *header,BMPINFOHEADER *info,IMAGE *picture,char *output){
    FILE *fp = fopen(output,"wb");
    fwrite(&header,sizeof(BMPFILEHEADER),1,fp);
    fwrite(&info,sizeof(BMPINFOHEADER),1,fp);

    if ( picture->info->bpp == 8) {
        for (uint32_t i = 0; i < picture->info->color_used * 4; i++) {
            fwrite(&picture->palette[i], 1, 1, fp);
        }
    }
    for (uint32_t i = 0;
         i < picture->info->width * picture->info->height * picture->info->bpp / 8; i++) {
        fwrite(&picture->rgb[i], 1, 1, fp);
    }

    fclose(fp);

}

void free_Image(IMAGE *picture, BMPFILEHEADER *header, BMPINFOHEADER *info){
    free(picture->info);
    free(picture->palette);
    free(picture->rgb);
    free(picture);
    free(info);
    free(header);
}

int BMP_openfile(char* input_name, char* output_name){
    FILE *fp = fopen(input_name,"rb");
    BMPINFOHEADER *info;
    BMPFILEHEADER *header;
    header = malloc(sizeof(BMPFILEHEADER));
    info = malloc(sizeof(BMPINFOHEADER));
    fread(header,sizeof(BMPFILEHEADER),1,fp); 
    fread(info,sizeof(BMPINFOHEADER),1,fp);

    if (header->file_type != 19778){
        fprintf(stdout,"Error format. The converter only supports BMP format.");
        fclose(fp);
        return -1;
    }
    if (info->planes != 1){
        fprintf(stdout, "Error planes. The number of planes must be 1.");
        fclose(fp);
        return -2;
    }
    if (info->bpp != 8 && info->bpp != 24){
        fprintf(stdout,"Error bits per pixel error. The converter only supports 8 and 24 bits per pixel images.");
        fclose(fp);
        return -2;
    }
    if(info->compression != 0){
        fprintf(stdout, "Error compression. Only uncompressed images are supported");
        fclose(fp);
        return -2;
    }

    struct Image *picture = read_Image(fp,info);
    if(!picture){
        fclose(fp);
        return -1;
    }
    
    
    fclose(fp);
    
    create_BMP_Image(header,info,picture);
    free_Image(picture);
    return 0;
}

}

