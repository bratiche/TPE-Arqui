#include <fractals.h>
#include <syscalls.h>

//Fuente: http://www.splinter.com.au/converting-hsv-to-rgb-colour-using-c
static void HsvToRgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v);

void mandelbrot(int iter, int r, int g, int b, int r2, int g2, int b2) {   ///TODO pasar para externo, iteraciones...

    double ImageWidth=1024;
    double ImageHeight=768;
    double MinRe = -2.0;
    double MaxRe = 1.0;
    double MinIm = -1.2;
    double MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
    double Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
    double Im_factor = (MaxIm-MinIm)/(ImageHeight-1);

    for(unsigned y=0; y<ImageHeight; ++y)
    {
        double c_im = MaxIm - y*Im_factor;
        for(unsigned x=0; x<ImageWidth; ++x)
        {
            double c_re = MinRe + x*Re_factor;

            double Z_re = c_re, Z_im = c_im;
            int isInside = 1;
            unsigned n = 0;
            for(n=0; n<iter; ++n)
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
            	draw(x, y, r, g, b); 
            }
            else if (n <= (iter / 2) - 1) {        // de interno a negro
                draw(x, y, (r2 / iter) * n, (g2 / iter) * n, (b2 / iter) * n);
            }
            else if (n >= iter / 2 && n <= iter - 1 ) {      
                draw(x, y, (r2 / iter) * n, (g2 / iter) * n, (b2 / iter) * n);
                //draw(x, y, r2 - n % 255, g2 - n % 255, b2 - n % 255);
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
            unsigned char r, g, b;
            HsvToRgb(&r, &g, &b, i % 256, 255, 255 * (i < maxIterations));

            //draw the pixel
            draw(x, y, r, g, b);

        }
    }

}

void juliaSet() {

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
          unsigned char r, g, b;
          HsvToRgb(&r, &g, &b, i % 256, 255, 255 * (i < maxIterations));

          //draw the pixel
          draw(x, y, r, g, b);
          
        }
    }


}

static void HsvToRgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v)
{
    unsigned char region, fpart, p, q, t;

    if(s == 0) {
        /* color is grayscale */
        *r = *g = *b = v;
        return;
    }

    /* make hue 0-5 */
    region = h / 43;
    /* find remainder part, make it from 0-255 */
    fpart = (h - (region * 43)) * 6;

    /* calculate temp vars, doing integer multiplication */
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

    /* assign temp vars based on color cone region */
    switch(region) {
        case 0:
            *r = v; *g = t; *b = p; break;
        case 1:
            *r = q; *g = v; *b = p; break;
        case 2:
            *r = p; *g = v; *b = t; break;
        case 3:
            *r = p; *g = q; *b = v; break;
        case 4:
            *r = t; *g = p; *b = v; break;
        default:
            *r = v; *g = p; *b = q; break;
    }

    return;
}