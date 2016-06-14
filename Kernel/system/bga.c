#include   <io.h>
#include <naiveConsole.h>
#include <bga.h>

char * vga = (char *) VBE_DISPI_LFB_PHYSICAL_ADDRESS;

void BgaWriteRegister(unsigned short IndexValue, unsigned short DataValue){

    write_port_word(VBE_DISPI_IOPORT_INDEX, IndexValue);
    write_port_word(VBE_DISPI_IOPORT_DATA, DataValue);
}
 
unsigned short BgaReadRegister(unsigned short IndexValue){

    write_port_word(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return read_port_word(VBE_DISPI_IOPORT_DATA);
}
 
int BgaIsAvailable(void){    

    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
}
 
void BgaSetVideoMode(unsigned int Width, unsigned int Height, unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory){    

    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BgaWriteRegister(VBE_DISPI_INDEX_XRES, Width);
    BgaWriteRegister(VBE_DISPI_INDEX_YRES, Height);
    BgaWriteRegister(VBE_DISPI_INDEX_BPP, BitDepth);
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |  
          (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) | 
                 (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));    

    //drawFractal();

    return;
}
 
void BgaSetBank(unsigned short BankNumber){

    BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
    return;
}

void BgaGetCapabilities(){

    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE,VBE_DISPI_GETCAPS);
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_XRES));
    ncNewline();
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_YRES));
    ncNewline();
    ncPrintDec(BgaReadRegister(VBE_DISPI_INDEX_BPP));
    ncNewline();

    return;

}

void drawFractal(){

    int i = 0;    

    for (i = 0; i<1024*768*3;i++){

        vga[i] = (char)0xFF;
    
    }

    //drawPixel(600,600);
}
