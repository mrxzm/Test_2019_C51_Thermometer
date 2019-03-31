#include "reg52.h"

#define uchar unsigned char
#define uint unsigned int
#define DataPort P0 //���������ݶ˿� P0
sbit LATCH1=P2^4; // 74HC573
sbit DS =P3 ^ 3; // DS18B20

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 位码 0~9
unsigned char code dofly_WeiMa[]={0xf7,0xfe,0xfd,0xfb,    0xdf, 0xbf, 0x7f, 0xef,   };//段码


unsigned int Ds_Result();//�������ս���
bit DS_Init();//��ʼ���Ӻ���
void Ds_Write(unsigned char dat);//��д���Ӻ���������������д���
unsigned char Ds_Read();//�������Ӻ��������ڴ����߶�ֵ��
void Dispaly(int p1, int p2, int p3, int p4, int point);//������ ��ʾ ����
void Delay_us(int t); // ��ʱ 1us
void Delay_ms(int t); // ��ʱ 1ms


void main()
{
	int test = 0, num = 0; // ��Ҫ��ʾ��ֵ

	while(1)//����ѭ����ʾ
	{
		num = Ds_Result(); // ��Ҫ��ʾ��ֵ

		Dispaly(num/1000, (num%1000)/100, ((num%1000)%100)/10, ((num%1000)%100)%10, 3);
		Delay_ms(10);
	}
}
///////////////////////////    DS18B20      ///////////////////////
bit Ds_Init()
{
	bit ack;
	DS = 1;
	Delay_us(8);
	DS = 0;
	Delay_us(80);
	DS = 1;
	Delay_us(14);
	while(DS == 0);
	DS = 1;
	Delay_us(20);
	return ack;
}

void Ds_Write(unsigned char dat)
{
	unsigned int i;
	for(i = 0; i < 8; i++)
	{
		DS = 0;
		//Delay_us(1);
		DS = dat&0x01;
		Delay_us(5);
		DS = 1;
		//Delay_us(2);
		dat>>=1;
	}
}

unsigned char Ds_Read()
{
	unsigned char dat = 0;
	int i;
	for(i = 0; i < 8; i++)
	{
		DS = 0;
		dat>>=1;
		DS = 1;
		//Delay_us(6);
		if(DS)
		{
			dat|=0x80;
		}
		Delay_us(5);
	}
	return dat;
}

unsigned int Ds_Result()
{
	float tp = 0;
	unsigned int temp = 0;
	unsigned char LSB = 0,MSB = 0;//�����������ݵĵڰ�λ���߰�λ

	Ds_Init();
	Ds_Write(0xCC);//����ROMѰַ
	Ds_Write(0x44);//����һ���¶�ת��

	Ds_Init();
	Ds_Write(0xCC);//����Ѱַ
	Ds_Write(0xBE);//���Ͷ�ֵ���
	LSB=Ds_Read();
	MSB=Ds_Read();
	temp=MSB;
	temp<<=8;
	temp=temp|LSB;
    tp=temp*0.0625;
	return tp * 10;
	//return (tp*100+0.5);
}


///////////////////////////    END      /////////////////////////////



///////////////////////////    ������      ///////////////////////////////
void Dispaly(int p1, int p2, int p3, int p4, int point){
	int i;
	char TempData[4];

	// ����
	TempData[0]=dofly_DuanMa[p1];
	TempData[1]=dofly_DuanMa[p2];
	TempData[2]=dofly_DuanMa[p3];
	TempData[3]=dofly_DuanMa[p4];

	for(i = 0; i < 4; i++)
	{
		DataPort=0;
		LATCH1=0;
		P2 = dofly_WeiMa[i];
		if(point == (i+1) ){
			DataPort = TempData[i] | 0x80;
		}
		else{
			DataPort = TempData[i];
		}
		LATCH1=1;
		LATCH1=0;
		Delay_ms(5);
	}
}



void Delay_ms(int i)
{
	uint j;
	for(;i>0;i--)
	for(j=150;j>0;j--);
}
void Delay_us(int t)
{
	while(t--);
}
