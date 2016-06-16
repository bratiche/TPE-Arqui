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
    int pos = y * width * 3 + x * 3;

    if (pos > width * height * 3) {
        BgaFillScreen(0xff, 0, 0);
        return;
    }

    linearBuffer[pos++] = b;
    linearBuffer[pos++] = g;
    linearBuffer[pos] = r;
}

void BgaDrawRect(int r, int g, int b, int x, int y, int w, int h) {
    int i, j;

    for (i = x; i < x + w; i++) {
        for (j = y; j < y + h; j++) {
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

void mandelbrot(int r, int g, int b){

    double ImageWidth=1024;
    double ImageHeight=768;
    double MinRe = -2.0;
    double MaxRe = 1.0;
    double MinIm = -1.2;
    double MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
    double Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
    double Im_factor = (MaxIm-MinIm)/(ImageHeight-1);
    unsigned MaxIterations = 30;

    for(unsigned y=0; y<ImageHeight; ++y)
    {
        double c_im = MaxIm - y*Im_factor;
        for(unsigned x=0; x<ImageWidth; ++x)
        {
            double c_re = MinRe + x*Re_factor;

            double Z_re = c_re, Z_im = c_im;
            int isInside = 1;
            unsigned n = 0;
            for(n=0; n<MaxIterations; ++n)
            {
                double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
                if(Z_re2 + Z_im2 > 4)
                {
                    isInside = 0;
                    break;
                }
                Z_im = 2*Z_re*Z_im + c_im;
                Z_re = Z_re2 - Z_im2 + c_re;
            }
            if(isInside) { 
            	BgaDrawPixel(x, y,r,g,b); 
            }
            else{
                //BgaDrawPixel(x, y,0xff-n,0xff-n,0xff-n);
            }
        }
    }


}

void mandelbrot2(){
    int w = 1024;
    int h = 768;
    double pr, pi;           //real and imaginary part of the pixel p
  	double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old z
  	double zoom = 1, moveX = -0.5, moveY = 0; //you can change these to zoom and change position
  	//ColorRGB color; //the RGB color value for the pixel
  	int maxIterations = 100;//after how much iterations the function should stop

  	//loop through every pixel
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
            pr = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
            pi = (y - h / 2) / (0.5 * zoom * h) + moveY;
            newRe = newIm = oldRe = oldIm = 0; //these should start at 0,0
            //"i" will represent the number of iterations
            int i;
            //start the iteration process
            for(i = 0; i < maxIterations; i++){
                //remember value of previous iteration
                oldRe = newRe;
                oldIm = newIm;
                //the actual iteration, the real and imaginary part are calculated
                newRe = oldRe * oldRe - oldIm * oldIm + pr;
                newIm = 2 * oldRe * oldIm + pi;
                //if the point is outside the circle with radius 2: stop
                if((newRe * newRe + newIm * newIm) > 4) break;
            }
        
            //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
            BgaDrawPixel(x,y , i % 256 , 255 , 255 * (i < maxIterations));
            //draw the pixel

        }
    }

}

void mandelbrot3(){

    int w = 1024;
    int h = 768;

    //each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
    double cRe, cIm;           //real and imaginary part of the constant c, determines shape of the Julia Set
    double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
    double zoom=1, moveX=0	, moveY=0; //you can change these to zoom and change position
    //ColorRGB color; //the RGB color value for the pixel
    int maxIterations=128; //after how much iterations the function should stop 


    //pick some values for the constant c, this determines the shape of the Julia Set
    cRe = -0.7;
    cIm = 0.27015;

    //begin the program loop
  
    //draw the fractal
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++)
        {
          //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
          newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
          newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
          //i will represent the number of iterations
          int i;
          //start the iteration process
          for(i = 0; i < maxIterations; i++)
          {
            //remember value of previous iteration
            oldRe = newRe;
            oldIm = newIm;
            //the actual iteration, the real and imaginary part are calculated
            newRe = oldRe * oldRe - oldIm * oldIm + cRe;
            newIm = 2 * oldRe * oldIm + cIm;
            //if the point is outside the circle with radius 2: stop
            if((newRe * newRe + newIm * newIm) > 4) break;
          }
          //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
          BgaDrawPixel(x,y,i % 256, 255, 255 * (i < maxIterations));
          //draw the pixel
          
        }
    }


}

