#include <GLUT/GLUT.h> // (or others, depending on the system in use)
#include <math.h>
#include "sgi.h"

#define MAX(x,y,z) (x > y ? x : y) > z ? (x > y ? x : y) : z
#define MIN(x,y,z) (x < y ? x : y) < z ? (x < y ? x : y) : z

GLsizei w,h;
int width, height, depth;
unsigned char *image = NULL;
int Status;

void init (void);
void reshape(int w, int h);
void lineSegment (void);
void mainSelect(int);
void fileSelect(int);
void displaySelect(int);
void NegativeImage();
void GrayscaleImage();
void BrightnessImage();
void ContrastImage();
void EdgeDetection();
void SaturationImage();
void Embossing();
void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v);
void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v);
void Blurring();
void Sharpenning();

void init (void)  {
    glClearColor (1.0, 1.0, 1.0, 0.0);       // Set display-window color to white.
    glMatrixMode (GL_PROJECTION); // Set projection parameters.
    gluOrtho2D (0.0, 200.0, 0.0, 150.0);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   height = (GLint) h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void display(void)
{

   glClear(GL_COLOR_BUFFER_BIT);
   glRasterPos2i(0, 0);

   glDrawPixels(width, height, GL_RGB,
                GL_UNSIGNED_BYTE, image);
   glFlush();
}

void mainSelect(int value)
{
    Status=value;
    switch (value) {
        case 666:
            printf("exit\n");
            exit(0);
            break;
    }
}

void fileSelect(int value)
{
    char *filename;
    Status=value;
    switch (value) {
        case 0:
            filename="sample.rgb";
            image = read_sgi(filename, &width, &height, &depth);
            glutReshapeWindow(width,height);
            display();
            break;
        case 1:
            filename="train.bmp";
            image = read_bmp(filename, &width, &height, &depth);
            glutReshapeWindow(width,height);
            display();
            break;
        case 2:
            filename="test.bmp";
            image = read_bmp(filename, &width, &height, &depth);
            glutReshapeWindow(width,height);
            display();
            break;
        case 3:
            filename="test2.bmp";
            image = read_bmp(filename, &width, &height, &depth);
            glutReshapeWindow(width,height);
            display();
            break;
    }
}

void displaySelect(int value)
{
    Status=value;
    switch (value) {
        case 10:
            NegativeImage();
            display();
            break;
        case 11:
            GrayscaleImage();
            display();
            break;
        case 12:
            BrightnessImage();
            display();
            break;
        case 13:
            ContrastImage();
            display();
            break;
        case 14:
            EdgeDetection();
            display();
            break;
        case 15:
            SaturationImage();
            display();
            break;
            
        case 16:
            Embossing();
            display();
            break;
        case 17:
            Blurring();
            display();
            break;
        case 18:
            Sharpenning();
            display();
            break;
       
    }
}

void NegativeImage() {
    int i, j;
    int r, g, b;
    for (j = 0; j < height; j++)  {
        for (i = 0; i < width * 3; i = i + 3) {
            r = image[j * width * 3 + i + 0];
            g = image[j * width * 3 + i + 1];
            b = image[j * width * 3 + i + 2];
            image[j * width * 3 + i + 0] = 255 - r;
            image[j * width * 3 + i + 1] = 255 - g;
            image[j * width * 3 + i + 2] = 255 - b;
       }
   }
}

void GrayscaleImage() {
    int i, j;
    float r, g, b;
    float lumi;
    for (j = 0; j < height; j++)  {
        for (i = 0; i < width * 3; i = i + 3) {
            r = image[j * width * 3 + i + 0];
            g = image[j * width * 3 + i + 1];
            b = image[j * width * 3 + i + 2];
            
            lumi = 0.30 * r + 0.59 * g + 0.11 * b;
            
            image[j * width * 3 + i + 0] = lumi;
            image[j * width * 3 + i + 1] = lumi;
            image[j * width * 3 + i + 2] = lumi;
       }
   }

}

void BrightnessImage() {
    int i, j;
    float r, g, b;
    float ro, go, bo;
    float scale = 1.5;
    
    // float scale = 0.8;   // 밝기 어둡게 하려면 1.0 이하
    for (j = 0; j < height; j++)  {
        for (i = 0; i < width * 3; i = i + 3) {
            r = image[j * width * 3 + i + 0];
            g = image[j * width * 3 + i + 1];
            b = image[j * width * 3 + i + 2];
            
            // editing
            ro = r * scale;
            go = g * scale;
            bo = b * scale;
            
            // clamping
            if (ro > 255) ro = 255; if (ro < 0) ro = 0;
            if (go > 255) go = 255; if (go < 0) go = 0;
            if (bo > 255) bo = 255; if (bo < 0) bo = 0;

            image[j * width * 3 + i + 0] = ro;
            image[j * width * 3 + i + 1] = go;
            image[j * width * 3 + i + 2] = bo;
       }
   }
}

void ContrastImage() {
    int i, j;
    float r, g, b;
    float ro, go, bo;
    float scale = 1.5;
    float Lumi = 0;
    
    // 평균 밝기 값 구하기
    for (j = 0; j < height; j++)  {
        for (i = 0; i < width * 3; i = i + 3) {
            r = image[j * width * 3 + i + 0];
            g = image[j * width * 3 + i + 1];
            b = image[j * width * 3 + i + 2];
            
            Lumi += 0.3 * r +0.59 * g + 0.11 * b;
        }
        Lumi = (float) (Lumi / (width * height));
        
        for (j = 0; j < height; j++)  {
            for (i = 0; i < width * 3; i = i + 3) {
                r = image[j * width * 3 + i + 0];
                g = image[j * width * 3 + i + 1];
                b = image[j * width * 3 + i + 2];
                
                // editing
                ro = Lumi + (r - Lumi) * scale;
                go = Lumi + (g - Lumi) * scale;
                bo = Lumi + (b - Lumi) * scale;
                
                // clamping
                if (ro > 255) ro = 255; if (ro < 0) ro = 0;
                if (go > 255) go = 255; if (go < 0) go = 0;
                if (bo > 255) bo = 255; if (bo < 0) bo = 0;
                
                image[j * width * 3 + i + 0] = ro;
                image[j * width * 3 + i + 1] = go;
                image[j * width * 3 + i + 2] = bo;
            }
        }
    }
}

void EdgeDetection() {
    
    int filter[3][3]={{-1,-1,-1},{-1,8,-1},{-1,-1,-1}}; // image.pdf 38pg 참고

    unsigned char *timage = (unsigned char *)malloc(width*height*3);
    int r,g,b, sumr,sumg,sumb, i,j,k,l,pos;
    int filter_sum=1;
    for (j=1;j<height-1; j++)  {    // height -1 해주는 이유 : 가장 자리를 구할 수 없기 때문에 & 시작은 1부터 (0 아님)
        for (i=1;i<(width-1); i++) {    //weight - 1 위와 마찬가지.
            
            // 각각의 r, g, b 의 sum 을 해준다
            sumr=0; sumb=0; sumg=0;
            
            for(k=0;k<3;k++){   //주변 픽셀을 꺼내오는 부분
                for(l=0;l<3;l++){
                    pos= (j+l-1)*width*3+(i+k-1)*3;
                    
                    r=image[pos+0];
                    g=image[pos+1];
                    b=image[pos+2];
                    
                    sumr += r*filter[k][l];
                    sumg += g*filter[k][l];
                    sumb += b*filter[k][l];
                }
            }
            
            // 갯수로 나눠주면 평균값 구할 수 있음
            sumr/=filter_sum;
            sumg/=filter_sum;
            sumb/=filter_sum;
            
            //clamping
            if ( sumr > 255) sumr=255; if ( sumr < 0.0) sumr=0.0;
            if ( sumg > 255) sumg=255; if ( sumg < 0.0) sumg=0.0;
            if ( sumb > 255) sumb=255; if ( sumb < 0.0) sumb=0.0;
            
            
            // timage 가 해주는 역할 : 원본을 바꾸면 화질이 깨져서 원본은 그대로 두고 바꾼 픽셀 값을 timage 에 따로 저장함
            timage[j*width*3+i*3+0]=sumr;
            timage[j*width*3+i*3+1]=sumg;
            timage[j*width*3+i*3+2]=sumb;
        }
    }
    
    // timage 로 구한 이미지를 실제 원본에 덮어쓰는 과정 -> 화면상에 display
    for (j=1;j<height-1; j++)  {
        for (i=1;i<(width-1); i++) {
            image[j*width*3+i*3+0]=timage[j*width*3+i*3+0];
            image[j*width*3+i*3+1]=timage[j*width*3+i*3+1];
            image[j*width*3+i*3+2]=timage[j*width*3+i*3+2];
        }
    }

}


void Embossing() {
    
    unsigned char *timage = (unsigned char *)malloc(width*height*3);
    int r,g,b, sumr,sumg,sumb, i,j,k,l,pos;
    int filter_sum=1;
    
    //1. 그레이 필터로 바꾸기
    GrayscaleImage();
    
    //2. filtering
    int filter[3][3]={{2,0,0},{0,-1,0},{0,0,-1}};
    
    for (j=1;j<height-1; j++)  {
        for (i=1;i<(width-1); i++) {
            sumr=0; sumb=0; sumg=0;
            
            for(k=0;k<3;k++){
                for(l=0;l<3;l++){
                    
                    pos= (j+l-1)*width*3+(i+k-1)*3;
                    
                    r=image[pos+0];
                    g=image[pos+1];
                    b=image[pos+2];
                    
                    sumr += r*filter[k][l];
                    sumg += g*filter[k][l];
                    sumb += b*filter[k][l];
                    
                }
            }
            
            sumr/=filter_sum;
            sumg/=filter_sum;
            sumb/=filter_sum;
            
            if ( sumr > 255) sumr=255; if ( sumr < 0.0) sumr=0.0;
            if ( sumg > 255) sumg=255; if ( sumg < 0.0) sumg=0.0;
            if ( sumb > 255) sumb=255; if ( sumb < 0.0) sumb=0.0;
            
            
            
            timage[j*width*3+i*3+0]=sumr + 128;
            timage[j*width*3+i*3+1]=sumg + 128;
            timage[j*width*3+i*3+2]=sumb + 128;
        }
        
    }
   
    //4. 더한 값에 clamping
    for (j=1;j<height-1; j++)  {
        for (i=1;i<(width-1); i++) {
            image[j*width*3+i*3+0]=timage[j*width*3+i*3+0];
            image[j*width*3+i*3+1]=timage[j*width*3+i*3+1];
            image[j*width*3+i*3+2]=timage[j*width*3+i*3+2];
        }
    }
}

void SaturationImage() {
    int i, j;
    float r, g, b, h, s, v;
    float scale = 0.5;

    for (j = 0; j < height; j++) {
        for (i = 0; i < width * 3; i = i + 3) {
            r = image[j * width * 3 + i + 0];
            g = image[j * width * 3 + i + 1];
            b = image[j * width * 3 + i + 2];
            r = r / 255.0; g = g / 255.0; b = b / 255.0;
            
            // step1 : Color Convert RGB -> HSV
            RGBtoHSV(r, g, b, &h, &s, &v);
            
            // step2 : Scale from S for each pixel component
            s *= scale;
            
            // step3 : Color Convert HSV -> RGB
            HSVtoRGB(&r, &g, &b, h, s, v);
            r = r * 255.0; g = g * 255.0; b = b * 255.0;
            
            // clamping
            if (r > 255) r = 255; if (r < 0) r = 0;
            if (g > 255) g = 255; if (g < 0) g = 0;
            if (b > 255) b = 255; if (b < 0) b = 0;
            
            image[j * width * 3 + i + 0] = r;
            image[j * width * 3 + i + 1] = g;
            image[j * width * 3 + i + 2] = b;
        }
    }
    
}

void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v) {
    float min, max, delta;
    
    min = MIN(r, g, b);
    max = MAX(r, g, b);
    
    *v = max;   //v
    
    delta = max - min;
    
    if(max != 0)
        *s = delta / max;   //s
    else {
        // r = g = b = 0    //s = 0, v is undefined
        *s = 0;
        *h = -1;
        return;
    }
    
    if(r == max)
        *h = (g - b) / delta;   //between yellow & magenta
    else if(g == max)
        *h = 2 + (b - r) / delta;   //between cyan & yellow
    else
        *h = 4 + (r - g) / delta;   //between magenta & cyan
    
    *h *= 60; 
    if (*h < 0)
        *h += 360;
}

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v) {
    
    int i;
    float f, p, q, t;
    
    if (s == 0) {
        //achromatic (grey)
        *r = *g = *b = v;
        return;
    }
    
    h /= 60;
    i = float(h);
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    
    switch (i) {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:    // == case 5:
            *r = v;
            *g = p;
            *b = q;
            break;
    }
}

void Blurring() {
    
    float filter[3][3]={{0.0625,0.125,0.0625},{0.125,0.25,0.125},{0.0625,0.125,0.0625}}; // blurring filter

    unsigned char *timage = (unsigned char *)malloc(width*height*3);
    int r,g,b, sumr,sumg,sumb, i,j,k,l,pos;
    int filter_sum=1;
    for (j=1;j<height-1; j++)  {    // height -1 해주는 이유 : 가장 자리를 구할 수 없기 때문에 & 시작은 1부터 (0 아님)
        for (i=1;i<(width-1); i++) {    //weight - 1 위와 마찬가지.
            
            // 각각의 r, g, b 의 sum 을 해준다
            sumr=0; sumb=0; sumg=0;
            
            for(k=0;k<3;k++){   //주변 픽셀을 꺼내오는 부분
                for(l=0;l<3;l++){
                    pos= (j+l-1)*width*3+(i+k-1)*3;
                    
                    r=image[pos+0];
                    g=image[pos+1];
                    b=image[pos+2];
                    
                    sumr += r*filter[k][l];
                    sumg += g*filter[k][l];
                    sumb += b*filter[k][l];
                }
            }
            
            // 갯수로 나눠주면 평균값 구할 수 있음
            sumr/=filter_sum;
            sumg/=filter_sum;
            sumb/=filter_sum;
            
            //clamping
            if ( sumr > 255) sumr=255; if ( sumr < 0.0) sumr=0.0;
            if ( sumg > 255) sumg=255; if ( sumg < 0.0) sumg=0.0;
            if ( sumb > 255) sumb=255; if ( sumb < 0.0) sumb=0.0;
            
            
            // timage 가 해주는 역할 : 원본을 바꾸면 화질이 깨져서 원본은 그대로 두고 바꾼 픽셀 값을 timage 에 따로 저장함
            timage[j*width*3+i*3+0]=sumr;
            timage[j*width*3+i*3+1]=sumg;
            timage[j*width*3+i*3+2]=sumb;
        }
    }
    
    // timage 로 구한 이미지를 실제 원본에 덮어쓰는 과정 -> 화면상에 display
    for (j=1;j<height-1; j++)  {
        for (i=1;i<(width-1); i++) {
            image[j*width*3+i*3+0]=timage[j*width*3+i*3+0];
            image[j*width*3+i*3+1]=timage[j*width*3+i*3+1];
            image[j*width*3+i*3+2]=timage[j*width*3+i*3+2];
        }
    }

}

void Sharpenning() {

    int filter[3][3]={{0,-1,0},{-1,5,-1},{0,-1,0}}; //

    unsigned char *timage = (unsigned char *)malloc(width*height*3);
    int r,g,b, sumr,sumg,sumb, i,j,k,l,pos;
    int filter_sum=1;
    for (j=1;j<height-1; j++)  {    // height -1 해주는 이유 : 가장 자리를 구할 수 없기 때문에 & 시작은 1부터 (0 아님)
        for (i=1;i<(width-1); i++) {    //weight - 1 위와 마찬가지.
            
            // 각각의 r, g, b 의 sum 을 해준다
            sumr=0; sumb=0; sumg=0;
            
            for(k=0;k<3;k++){   //주변 픽셀을 꺼내오는 부분
                for(l=0;l<3;l++){
                    pos= (j+l-1)*width*3+(i+k-1)*3;
                    
                    r=image[pos+0];
                    g=image[pos+1];
                    b=image[pos+2];
                    
                    sumr += r*filter[k][l];
                    sumg += g*filter[k][l];
                    sumb += b*filter[k][l];
                }
            }
            
            // 갯수로 나눠주면 평균값 구할 수 있음
            sumr/=filter_sum;
            sumg/=filter_sum;
            sumb/=filter_sum;
            
            //clamping
            if ( sumr > 255) sumr=255; if ( sumr < 0.0) sumr=0.0;
            if ( sumg > 255) sumg=255; if ( sumg < 0.0) sumg=0.0;
            if ( sumb > 255) sumb=255; if ( sumb < 0.0) sumb=0.0;
            
            
            // timage 가 해주는 역할 : 원본을 바꾸면 화질이 깨져서 원본은 그대로 두고 바꾼 픽셀 값을 timage 에 따로 저장함
            timage[j*width*3+i*3+0]=sumr;
            timage[j*width*3+i*3+1]=sumg;
            timage[j*width*3+i*3+2]=sumb;
        }
    }
    
    // timage 로 구한 이미지를 실제 원본에 덮어쓰는 과정 -> 화면상에 display
    for (j=1;j<height-1; j++)  {
        for (i=1;i<(width-1); i++) {
            image[j*width*3+i*3+0]=timage[j*width*3+i*3+0];
            image[j*width*3+i*3+1]=timage[j*width*3+i*3+1];
            image[j*width*3+i*3+2]=timage[j*width*3+i*3+2];
        }
    }

}

int main (int argc, char** argv)   {

    int fileMenu, mainMenu, pixelOperation, imageFiltering;

    glutInit (&argc, argv);                 // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);     // Set display mode.
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


    glutInitWindowPosition (200, 100);     // Set top-left display-window position.
    glutInitWindowSize (500, 500);     // Set display-window width and height.
    glutCreateWindow ("Image Processing Program"); // Create display window.

    init ( );                     // Execute initialization procedure.
    glutDisplayFunc(display);     // Send graphics to display window.
    glutReshapeFunc(reshape);

    fileMenu = glutCreateMenu(fileSelect);
    glutAddMenuEntry("sample.rgb",0);
    glutAddMenuEntry("train.bmp",1);
    glutAddMenuEntry("test.bmp", 2);
    glutAddMenuEntry("test2.bmp", 3);
    
    
    pixelOperation = glutCreateMenu(displaySelect);
    glutAddMenuEntry("Grayscale Image",11);
    glutAddMenuEntry("Negative Image",10);
    glutAddMenuEntry("Adjust Brightness",12);
    glutAddMenuEntry("Contrast",13);
    glutAddMenuEntry("Saturation", 15);

    imageFiltering = glutCreateMenu(displaySelect);
    glutAddMenuEntry("Blurring",17);
    glutAddMenuEntry("Edge Detection",14);
    glutAddMenuEntry("Sharpening", 18);
    glutAddMenuEntry("Embossing", 16);
    

    
    mainMenu = glutCreateMenu(mainSelect);
    glutAddSubMenu("1. Open File", fileMenu);
    glutAddSubMenu("2. Pixel Operation", pixelOperation);
    glutAddSubMenu("3. Image Filtering", imageFiltering);
    glutAddMenuEntry("4. Quit",666);
    glutSetMenu(mainMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop ( );         // Display everything and wait.

    return 0;
}
