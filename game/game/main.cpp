#include<cstdio>
#include<iostream>
#include<easyx.h>
#include<cmath>
#include<stdlib.h>

using namespace std;
#define TwoPointDistance(x1,y1,x2,y2)  sqrt(pow(x1-x2,2) + pow(y1-y2,2))
IMAGE pic[6];
void loadpic()
{
	//加载图片
	for (int i = 0;i < 4;i++)
	{
		char str[50] = {};
		sprintf_s(str, "Resource/images/%d.png", i);
		loadimage(pic+i, str);

	}
	loadimage(pic + 4, "Resource/images/bk.jpg", 1080, 640 - 120);
}




//hook,
struct Hook
{
	double x;//开始时坐标
	double y;
	double ex;//变化的坐标
	double ey;
	double len;//绳长
	double angle;
	int state=0;
	double dx;
	double dy;


};

void hookinit(Hook* hook, int x, int y, int len)
{
	hook->x = x;
	hook->y = y;
	hook->len = 60;
	hook->ex=hook->x;
	hook->ey =hook-> y+hook->len;
	hook->angle = 0.0;
	hook->dx = 0.0;
	hook->dy = 0.0;
}
void hookdraw(Hook* hook)
{
	setlinestyle(PS_SOLID,3);//样式
	setlinecolor(BROWN);//颜色
	line(hook->x, hook->y, hook->ex, hook->ey);
	setfillcolor(GREEN);
	solidrectangle(hook->ex - 5, hook->ey - 5, hook->ex + 5, hook->ey + 5);
	

}//制作一个钩子形象


//钩子摆动制作
int a = 1;//判断角度有无超过
void hookswing(Hook* hook)
{
	if (hook->state == 0)
	{

		double rad = (hook->angle) * 3.14 / 180;
		if ((hook->angle) >= 80 || (hook->angle) <= -80)
		{
			a = -a;
		}
		(hook->angle) += 0.05 * a;
		hook->ex = (hook->x) + sin(rad) * hook->len;
		hook->ey = (hook->y) + cos(rad) * hook->len;

	}
}




Hook myhook;
void init()
{
	loadpic();
	hookinit(&myhook, 500, 100, 60);

}//初始化钩子


//钩子伸缩
void hookcontrol(Hook*hook)
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		hook->state = 1;
		
	}
	if (hook->state == 1)
	{
		double rad = (hook->angle) * 3.14 / 180;
		hook->ex += 0.2 * sin(rad);
		hook->ey += 0.2 * cos(rad);
		hook->len += 0.2;
		if (hook->ex<0 || hook->ex > getwidth() || hook->ey > getheight())
		{
			hook->state = -1;
		}
	}
	else if (hook->state == -1)
	{
		double rad = (hook->angle) * 3.14 / 180;
		hook->ex -= 0.2 * sin(rad);
		hook->ey -= 0.2 * cos(rad);
		hook->len -= 0.2;
		if (hook->len <= 60)
		{
			hook->state = 0;
		}
	}
	else if (hook->state == 0)
	{
		return;
	}

}


struct Mine
{
	int x;
	int y;
	int size;
	int type;
	int value;
}mine[15];
void mine_init(Mine* pthis)
{
	pthis->x = rand() % (getwidth() - 100);
	pthis->y = rand() % (getheight() - 150) + 150;
	
	pthis->type = rand() % 3;
	pthis->value = rand() % 100 + 100;

}
void mine_draw(Mine* pthis)
{
        putimage(pthis->x, pthis->y, pic + pthis->type);
}
Mine mymine[10];
void draw()
{
	//绘制形象
	putimage(0, 120, pic+4);//背景
	setfillcolor(RGB(312,132,231));
	solidrectangle(0, 0, 1080, 120);
	putimage(480, 15, pic+3);
    hookdraw(&myhook);//钩子
	for (int j = 0;j < 9;j++)
	{
		mine_draw(&mymine[j]);
	}
	
}






int main()
{
	initgraph(1080, 640);
    
	init();
	for (int j=0;j<9;j++) 
	{
		mine_init( & mymine[j]);
	}
	while (true)
	{
		BeginBatchDraw();
		
		draw();
		
		FlushBatchDraw();
		hookswing(&myhook);

		for(int k=0;k<9;k++)
			if (myhook.ex > mymine[k].x && myhook.ex <(mymine[k].x + mymine[k].value) && myhook.ey>mymine[k].y && myhook.ey < (mymine[k].y + mymine[k].value))
			{
				myhook.state = -1;
				hookcontrol(&myhook);
				mymine[k] = {};

			}
		hookcontrol(&myhook);
		
	}
	
	
	return 0;

}

