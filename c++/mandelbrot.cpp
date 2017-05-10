#include <complex>  
#include <complex.h>
#include<iostream>
#include "graphics.h"
#include <conio.h>
#include<string>
#include<fstream>
using namespace std;

struct pixel  //单个像素
{
	char r, g, b;
};

class Complex
{
public:
	Complex(double real = 0, double imag = 0) :real(real), imag(imag){}
	Complex(Complex& com) :real(com.real), imag(com.imag){}
	double& operator[](int i){ return i == 0 ? real : imag; }
	double get_mod2(){ return pow(real, 2) + pow(imag, 2); }
	Complex& operator=(Complex& com){ real = com.real; imag = com.imag; return *this; }
	Complex operator+(Complex& com){ return Complex(real + com.real, imag + com.imag); }
	Complex operator*(Complex& com){ return Complex(real*com.real - imag*com.imag, real*com.imag + imag*com.real); }

private:
	double real, imag;
};
  
class Mandelbrot
{
public:
	Mandelbrot(int _width=640, int _height=480)
		:path(path), width(_width), height(_height){}
	~Mandelbrot(){}
public:
	void set_path(string path)
	{
		this->path = path;
	}
	void draw(double from_x,double to_x,double from_y,double to_y) //图片保存路径
	{
		create_image();
		write_head();

		for (int y = 0; y < height; y++)
		{
			Complex c(0, from_y + (to_y - from_y) * y / (double)height);
			for (int x = 0; x < width; x++)
			{
				c[0] = from_x + (to_x - from_x) * x / (double)width;
				Complex z(0, 0);
				int k;
				for (k = 0; k < 1000; k++)
				{
					if (z.get_mod2() > 4.0)	break;
					z = z * z + c;
				}
				pixel p = { Color[k % 64].b, Color[k % 64].g,
					Color[k % 64].r
				};
				pixel q = { 0, 0, 0 };
				if (k == 1000) //收敛
				{
					image.write((char*)&q,sizeof(q));
				}
				else //发散
				{
					image.write((char*)&p,sizeof(p));
				}
			}
		}
		image.close();
	}
	void get_color(string path)
	{
		color.open(path,ios::binary);
		for (int i = 0; i < 64; ++i)
		{
			color.read((char*)&Color[i], sizeof(pixel));
		}
		color.close();
	}
	int get_fileSize(){ return height*width*sizeof(pixel) / 1024; }  //KB
private:
	ofstream image;
	ifstream color;
	void create_image()
	{
		image.open(path,ios::binary);
	}
	void write_head() //文件头
	{
		image << "P6\n"
			<< "#Mandelbrot Set\n"
			<< width << ' '
			<< height << '\n'
			<< 255<<'\n';
	}
	
	int width, height; //图片尺寸
	pixel Color[64];
	string path;
};



int main()
{

	Mandelbrot test;
	test.set_path("D:\\1.ppm");
	test.get_color("D:\\color.dat");
	cout << test.get_fileSize()<<"KB";
	test.draw(-2.1, 1.1, -1.2, 1.2);
	
	return 0;
}