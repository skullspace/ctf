
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include "jpeg/jpeglib.h"

using namespace std;

JSAMPLE *img;
const int width=640;
const int height=480;

class Point
{
public:
	Point(short int x, short int y): x_(x), y_(y) {}
    Point(): x_(0), y_(0) {}
	short int x() const { return x_; }
	short int y() const { return y_; }

private:
	short int x_,y_;
};

void setPixel(Point p)
{
    img[3*(p.x()+width*p.y())+0]=0;
    img[3*(p.x()+width*p.y())+1]=0;
	img[3*(p.x()+width*p.y())+2]=0;
}

void line(Point a, Point b)
{
    const short dx=b.x()-a.x();
    const short dy=b.y()-a.y();
	const short adx=abs(dx);
    const short ady=abs(dy);
    if(adx>ady)
	{
		short yincr= dy>=0 ? 1 : -1;
        short d=2*ady-adx;
		short v=2*(ady-adx);
	    short w=2*ady;
        short y=a.y();
	    if(dx>0)
		{
    	    for(short x=a.x();x<=b.x();x++)
		    {
		    	setPixel(Point(x,y));
        		if(d>0)
                {
        	        y+=yincr;
                    d+=v;
		    	} else d+=w;
	    	}
    	} else {
            for(short x=a.x();x>=b.x();x--)
		    {
                setPixel(Point(x,y));
	    		if(d>0)
                {
        	    	y+=yincr;
                    d+=v;
	    	    } else d+=w;
    		}
        }
	} else {
	    short xincr= dx>=0 ? 1 : -1;
		short d=2*adx-ady;
    	short v=2*(adx-ady);
        short w=2*adx;
	    short x=a.x();
		if(dy>0)
        {
    		for(short y=a.y();y<=b.y();y++)
		    {
			    setPixel(Point(x,y));
        	    if(d>0)
    			{
                    x+=xincr;
        	    	d+=v;
			    } else d+=w;
            }
		} else {
            for(short y=a.y();y>=b.y();y--)
	    	{
	        	setPixel(Point(x,y));
                if(d>0)
        	    {
			    	x+=xincr;
	    	    	d+=v;
                } else d+=w;
		    }
    	}
    }
}

int main()
{
	img=new JSAMPLE[height*width*3];
	memset(img,0xff,height*width*3);
	line(Point(10,100),Point(10,200));
	line(Point(10,200),Point(40,220));
	line(Point(40,220),Point(70,220));
	line(Point(70,220),Point(100,200));
	line(Point(100,200),Point(100,100));
	line(Point(300,200),Point(300,100));
	line(Point(300,100),Point(345,160));
	line(Point(345,160),Point(390,100));
	line(Point(390,100),Point(390,200));
	line(Point(400,200),Point(445,100));
	line(Point(445,100),Point(490,200));
	line(Point(500,200),Point(500,100));
	line(Point(500,100),Point(590,150));
	line(Point(590,150),Point(500,200));
	line(Point(300,330),Point(330,300));
	line(Point(330,300),Point(360,300));
	line(Point(360,300),Point(390,330));
	line(Point(390,330),Point(345,370));
	line(Point(345,390),Point(345,400));
	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;
	FILE *outfile=fopen("out.jpeg","wb");
	cinfo.err=jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo,outfile);
	cinfo.image_width=width;
	cinfo.image_height=height;
	cinfo.input_components=3;
	cinfo.in_color_space=JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo,90,TRUE);
	jpeg_start_compress(&cinfo,TRUE);
	JSAMPROW row_pointer[1];
	while(cinfo.next_scanline<cinfo.image_height)
	{
		row_pointer[0]=&img[cinfo.next_scanline*width*3];
		jpeg_write_scanlines(&cinfo,row_pointer,1);
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	delete[] img;
	cout<<"Done"<<endl;
}
