#include <iostream>
using namespace std;
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
int imageCount = 0;
bool TestimageCount(TGAImage& image, int x, int y, const TGAColor& color)
{
	//cout << x << "----" << y<<endl;
	imageCount++;
	return image.set(x, y, color);
}

//错误：绘制次数太多，性能低下。另外因为是按固定的间隔增加的 0.1 可能会错过一些格子
void  line1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	for (float t = 0.; t < 1.; t += .1) {
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		TestimageCount(image, x, y, color);
	}
}

//错误：只能用于x1 》x0 y1》y0
void  line2(int x0, int y0,int x1,int y1,TGAImage&image,TGAColor color){
	 for(int x = x0; x <= x1; x ++){
		 float t = (x - x0) / (float)(x1 - x0);
		int y = y0  + (y1-y0) * t;
		cout << x << "----" << y <<"--t---"<<t<< endl;
		TestimageCount(image, x, y, color);
	}
}
//最终
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;

	//设想极端情况,X0 到X1 只相差1，但Y0-Y1相差100，以X++来循环，Y无法插值。所以要以最长的轴来++循环
	//由于高度大于宽度会造成空洞，需要保持不陡
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // if the line is steep, we transpose the image 
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) { // make it left−to−right 
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	float derror = std::abs(dy / float(dx)); //斜率，X轴上每移动一格，Y轴增加的误差
	float error = 0;
	int y = y0;


	for (int x = x0; x <= x1; x++) {
		//float t = (x - x0) / (float)(x1 - x0);
		//int y = y0 * (1. - t) + y1 * t;
		if (steep) {
			TestimageCount(image, y, x, color); // if transposed, de−transpose 
		}
		else {
			TestimageCount(image, x, y, color);
		}
		error += derror;
		if (error > .5) {
			y += (y1 > y0 ? 1 : -1);
			error -= 1.;
		}
	}
}

int TestLinemain() {
	TGAImage image(100, 100, TGAImage::RGB);
	//device_draw_line(3, 3, 67, 83,image, red);
	//line(67, 83, 3, 3, image, red);
	//lineSimple(3, 3, 8, 16, image, red);
	line(3, 3, 67, 83, image, red);
	//lineSimple(20, 13, 40, 80, image, red);
	//lineSimple(80, 40, 13, 20, image, red);
	//line(3, 3, 67, 83, image, red);
	//image.set(52, 41, red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	cout << imageCount << "----" << endl;
	return 0;
}