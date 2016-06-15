#include <io.h>
#include <naiveConsole.h>
#include <bga.h>

/* Functions to set up the VESA video mode */ 

void BgaWriteRegister(unsigned short IndexValue, unsigned short DataValue) {

    write_port_word(VBE_DISPI_IOPORT_INDEX, IndexValue);
    write_port_word(VBE_DISPI_IOPORT_DATA, DataValue);
}
 
unsigned short BgaReadRegister(unsigned short IndexValue) {

    write_port_word(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return read_port_word(VBE_DISPI_IOPORT_DATA);
}
 
int BgaIsAvailable(void) {    

    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4 ? 1 : 0);
}
 
 
void BgaSetBank(unsigned short BankNumber) {

    BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
    return;
}

void BgaGetCapabilities() {

    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE,VBE_DISPI_GETCAPS);
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_XRES));
    ncNewline();
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_YRES));
    ncNewline();
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_BPP));
    ncNewline();

}

static unsigned int width = DEFAULT_WIDTH;
static unsigned int height = DEFAULT_HEIGHT;
static unsigned int bpp = DEFAULT_BPP;

void BgaSetVideoMode(unsigned int Width, unsigned int Height, unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory) {    

    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BgaWriteRegister(VBE_DISPI_INDEX_XRES, Width);
    BgaWriteRegister(VBE_DISPI_INDEX_YRES, Height);
    BgaWriteRegister(VBE_DISPI_INDEX_BPP, BitDepth);
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |  
          (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) | 
                 (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));

    width = Width;
    height = Height;
    bpp = BitDepth;    
}


/* Functions to manipulate the screen's pixels */

/* 
  Direccion que apunta a la direccion donde empieza el lfb (linear frame buffer)
  Hardcodeada del bootloader de Pure64. Para obtener esta direccion seteamos el modo vesa en 1
  y modificamos el codigo para que no inicialice en modo video (solo setea la direccion del buffer)
*/
unsigned char ** addressAddress = (unsigned char **)(0x0000000000005C00 + 40);

void BgaDrawPixel(int x, int y, int r, int g, int b) {
    unsigned char * linearBuffer = *addressAddress;
    int pos = x * width * 3 + y * 3;

    linearBuffer[pos++] = b;
    linearBuffer[pos++] = g;
    linearBuffer[pos] = r;
}

//TODO fix
void BgaDrawRect(int r, int g, int b, int x, int y, int w, int h) {
    int i, j;

    for (i = x; i < w * 3; i++) {
        for (j = y; j < h * 3; j++) {
            BgaDrawPixel(i, j, r, g, b);
        }
    }
}

void BgaFillScreen(int r, int g, int b) {
    unsigned char * linearBuffer = *addressAddress;
    int i;

    for (i = 0; i < width * height * 3;) {
        linearBuffer[i++] = b;
        linearBuffer[i++] = g;
        linearBuffer[i++] = r;
    }

}

