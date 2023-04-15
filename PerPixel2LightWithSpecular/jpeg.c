#include <stdio.h>
#include <malloc.h>
#include "image.h"

typedef unsigned short qtab_t[64];

typedef struct
{
	struct
	{
		unsigned char size;
		unsigned char code;
	} small[512], large[65536];
} huff_t;

typedef struct
{
	huff_t *huff_ac, *huff_dc;
	qtab_t *qtab;
	int dc_prev;
	int smpx, smpy;
	float t[256];
} component_t;

component_t component[4];
huff_t hufftable_ac[4], hufftable_dc[4];
qtab_t qtable[4];
float dctt[64];
unsigned char prec, ncomp;
int width, height;
int xblock, yblock, blockx, blocky;
unsigned char *data, *bpos, *dend;
int bsize;
int restart_int;
unsigned char eof;
unsigned char ss_start, ss_end, sbits;
unsigned int dt;
int bfree;

int zigzag[64]=
{
	0,	1,	8,	16,	9,	2,	3,	10,
	17,	24,	32,	25,	18,	11,	4,	5,
	12,	19,	26,	33,	40,	48,	41,	34,
	27,	20,	13,	6,	7,	14,	21,	28,
	35,	42,	49,	56,	57,	50,	43,	36,
	29,	22,	15,	23,	30,	37,	44,	51,
	58,	59,	52,	45,	38,	31,	39,	46,
	53,	60,	61,	54,	47,	55,	62,	63
};

char file_open(char *filename)
{
	FILE *stream;

	data=NULL;

	if((stream=fopen(filename, "rb"))==NULL)
	{
		return 0;
	}
	else
	{
		fseek(stream, 0, SEEK_END);
		bsize=ftell(stream);
		fseek(stream, 0, SEEK_SET);

		data=(unsigned char *)malloc(bsize);

		fread(data, 1, bsize, stream);
		fclose(stream);

		return 1;
	}
}

void file_close(void)
{
	if(data!=NULL)
		free(data);
}

unsigned char get_byte(void)
{
	if(bpos>=dend)
	{
		eof=1;
		return 0;
	}
	else
		return *bpos++;
}

void strm_skip(int n)
{
	unsigned char a, b;

	bfree+=n;
	dt<<=n;

	while(bfree>=8)
	{
		bfree-=8;
		b=get_byte();

		if(b==255)
			a=get_byte();

		dt|=(b<<bfree);
	}
}

int huff_dec(huff_t *h)
{
	unsigned int id, n, c;

	id=(dt>>(23));
	n=h->small[id].size;
	c=h->small[id].code;

	if(n==255)
	{
		id=(dt>>(16));
		n=h->large[id].size;
		c=h->large[id].code;
	}

	strm_skip(n);

	return c;
}

int word_dec(int n)
{
	int w;
	unsigned int s;

	if(n==0)
		return 0;
	else
	{
		s=(dt>>(31));
		w=(dt>>(32-n));
		strm_skip(n);

		if(s==0)
			w=(w|(0xffffffff<<n))+1;

		return w;
	}
}

void get_info(void)
{
	unsigned char cn, sf, qt;
	int i;

	prec=get_byte();

	if(prec!=8)
		return;

	height=((get_byte()<<8)+get_byte());
	width=((get_byte()<<8)+get_byte());
	ncomp=get_byte();

	if((ncomp!=3)&&(ncomp!=1))
		return;

	for(i=0;i<ncomp;i++)
	{
		cn=get_byte();
		sf=get_byte();
		qt=get_byte();

		component[cn-1].qtab=&qtable[qt];
		component[cn-1].smpy=sf&15;
		component[cn-1].smpx=(sf>>4)&15;
	}

	if(component[0].smpx==1)
		blockx=8;
	else
		blockx=16;

	if(component[0].smpy==1)
		blocky=8;
	else
		blocky=16;

	xblock=width/blockx;

	if((width&(blockx-1))!=0)
		xblock++;

	yblock=height/blocky;

	if((height&(blocky-1))!=0)
		yblock++;
}

void decode_hufftable(int len)
{
	int length[257], i, j, n, code, codelen, delta, rcode, cd, rng;
	unsigned char lengths[16], b, symbol[257];
	huff_t *h;

	len-=2;

	while(len>0)
	{
		b=get_byte();
		len--;
		h=&hufftable_dc[0];

		switch(b)
		{
			case 0:
				h=&hufftable_dc[0];
				break;

			case 1:
				h=&hufftable_dc[1];
				break;

			case 16:
				h=&hufftable_ac[0];
				break;

			case 17:
				h=&hufftable_ac[1];
				break;
		}

		for(i=0;i<16;i++)
			lengths[i]=get_byte();

		len-=16;
		n=0;

		for(i=0;i<16;i++)
		{
			len-=lengths[i];

			for(j=0;j<lengths[i];j++)
			{
				symbol[n]=get_byte();
				length[n++]=i+1;
			}
		}

		code=0;
		codelen=length[0];

		for(i=0;i<n;i++)
		{
			rcode=code<<(16-codelen);
			cd=rcode>>7;

			if(codelen<=9)
			{
				rng=1<<(9-codelen);

				for(j=cd;j<cd+rng;j++)
				{
					h->small[j].code=(unsigned char)symbol[i];
					h->small[j].size=(unsigned char)codelen;
				}
			}
			else
			{
				h->small[cd].size=(unsigned char)255;
				rng=1<<(16-codelen);

				for(j=rcode;j<rcode+rng;j++)
				{
					h->large[j].code=(unsigned char)symbol[i];
					h->large[j].size=(unsigned char)codelen;
				}
			}

			code++;
			delta=length[i+1]-codelen;
			code<<=delta;
			codelen+=delta;
		}
	}
}

void fidct(void)
{
	float a=0.353553385f;
	float b=0.490392625f;
	float c=0.415734798f;
	float d=0.277785122f;
	float e=0.097545162f;
	float f=0.461939752f;
	float g=0.191341713f;
	float cd=0.6935199499f;
	float be=0.5879377723f;
	float bc=0.9061274529f;
	float de=0.3753302693f;
	float a0, f2, g2, a4, f6, g6, s0, s1, s2, s3;
	float t0, t1, t2, t3, m0, m1, m2, m3;
	float h0, h1, h2, h3, r0, r1, r2, r3, w;
	int i;

	for(i=0;i<64;i+=8)
	{
		if((dctt[i+1]!=0)||(dctt[i+2]!=0)||(dctt[i+3]!=0)||(dctt[i+4]!=0)||(dctt[i+5]!=0)||(dctt[i+6]!=0)||(dctt[i+7]!=0))
		{
			a0=a*dctt[i];
			f2=f*dctt[i+2];
			g2=g*dctt[i+2];
			a4=a*dctt[i+4];
			g6=g*dctt[i+6];
			f6=f*dctt[i+6];
			m0=a0+a4;
			m1=a0-a4;
			m2=f2+g6;
			m3=g2-f6;
			s0=m0+m2;
			s1=m1+m3;
			s2=m1-m3;
			s3=m0-m2;
			h2=dctt[i+7]+dctt[i+1];
			h3=dctt[i+7]-dctt[i+1];
			r2=dctt[i+3]+dctt[i+5];
			r3=dctt[i+3]-dctt[i+5];
			h0=cd*dctt[i+1];
			h1=be*dctt[i+1];
			r0=be*dctt[i+5];
			r1=cd*dctt[i+3];
			w=de*r3;
			t0=h1+r1+e*(h3+r3)-w;
			t1=h0-r0-d*(h2-r3)-w;
			w=bc*r2;
			t2=h0+r0+c*(h3+r2)-w;
			t3=h1-r1-b*(h2+r2)+w;
			dctt[i]=s0+t0;
			dctt[i+1]=s1+t1;
			dctt[i+2]=s2+t2;
			dctt[i+3]=s3+t3;
			dctt[i+4]=s3-t3;
			dctt[i+5]=s2-t2;
			dctt[i+6]=s1-t1;
			dctt[i+7]=s0-t0;
		}
		else
		{
			a0=dctt[i]*a;
			dctt[i]=dctt[i+1]=dctt[i+2]=dctt[i+3]=dctt[i+4]=dctt[i+5]=dctt[i+6]=dctt[i+7]=a0;
		}
	}

	for(i=0;i<8;i++)
	{
		if((dctt[8+i]!=0)||(dctt[16+i]!=0)||(dctt[24+i]!=0)||(dctt[32+i]!=0)||(dctt[40+i]!=0)||(dctt[48+i]!=0)||(dctt[56+i]!=0))
		{
			a0=a*dctt[i];
			f2=f*dctt[16+i];
			g2=g*dctt[16+i];
			a4=a*dctt[32+i];
			g6=g*dctt[48+i];
			f6=f*dctt[48+i];
			m0=a0+a4;
			m1=a0-a4;
			m2=f2+g6;
			m3=g2-f6;
			s0=m0+m2;
			s1=m1+m3;
			s2=m1-m3;
			s3=m0-m2;
			h2=dctt[56+i]+dctt[8+i];
			h3=dctt[56+i]-dctt[8+i];
			r2=dctt[24+i]+dctt[40+i];
			r3=dctt[24+i]-dctt[40+i];
			h0=cd*dctt[8+i];
			h1=be*dctt[8+i];
			r0=be*dctt[40+i];
			r1=cd*dctt[24+i];
			w=de*r3;
			t0=h1+r1+e*(h3+r3)-w;
			t1=h0-r0-d*(h2-r3)-w;
			w=bc*r2;
			t2=h0+r0+c*(h3+r2)-w;
			t3=h1-r1-b*(h2+r2)+w;
			dctt[i]=s0+t0;
			dctt[i+8]=s1+t1;
			dctt[i+16]=s2+t2;
			dctt[i+24]=s3+t3;
			dctt[i+32]=s3-t3;
			dctt[i+40]=s2-t2;
			dctt[i+48]=s1-t1;
			dctt[i+56]=s0-t0;
		}
		else
		{
			a0=dctt[i]*a;
			dctt[i]=dctt[i+8]=dctt[i+16]=dctt[i+24]=dctt[i+32]=dctt[i+40]=dctt[i+48]=dctt[i+56]=a0;
		}
}
	}

void decode_qtable(int len)
{
	int i;
	unsigned char b;

	len-=2;

	while(len>0)
	{
		b=(unsigned char)get_byte();
		len--;

		if((b&16)==0)
		{
			for(i=0;i<64;i++)
				qtable[b&15][i]=get_byte();

			len-=64;
		}
		else
		{
			for(i=0;i<64;i++)
				qtable[b&15][i]=((get_byte()<<8)+get_byte());

			len-=128;
		}
	}
}

void decode_block(void)
{
	int compn, i, j, b, p, codelen, code, cx, cy, otab[64];
	qtab_t *qtab;

	for(compn=0;compn<ncomp;compn++)
	{
		qtab=component[compn].qtab;

		for(cy=0;cy<component[compn].smpy;cy++)
		{
			for(cx=0;cx<component[compn].smpx;cx++)
			{
				for(i=0;i<64;i++)
					otab[i]=0;

				codelen=huff_dec(component[compn].huff_dc);
				code=word_dec(codelen);

				otab[0]=code+component[compn].dc_prev;
				component[compn].dc_prev=otab[0];
				i=1;

				while(i<64)
				{
					codelen=huff_dec(component[compn].huff_ac);

					if(codelen==0)
						i=64;
					else
						if(codelen==0xf0)
							i+=16;
						else
						{
							code=word_dec(codelen&15);

							i=i+(codelen>>4);
							otab[i++]=code;
						}
				}

				for(i=0;i<64;i++)
					dctt[zigzag[i]]=(float)((*qtab)[i]*otab[i]);

				fidct();
				b=(cy<<7)+(cx<<3);
				p=0;

				for(i=0;i<8;i++)
				{
					for(j=0;j<8;j++)
						component[compn].t[b++]=dctt[p++]+128;

					b+=8;
				}
			}
		}
	}
}

int clamptobyte(int value)
{
	if(value<0)
		value=0;

	if(value>255)
		value=255;

	return value;
}

int decode_scan(IMAGE_t *Image)
{
	int nnx, nny, i, j;
	int xmin, ymin, xmax, ymax, blockn, adr1, adr2;
	int y1, u1, v1, y2, u2, v2, u3, v3;
	int dux, duy, dvx, dvy;
	unsigned char sc, ts, ncomp;
	float cy, cu, cv;

	ncomp=get_byte();

	Image->Width=width;
	Image->Height=height;
	Image->Depth=ncomp*8;
	Image->Data=(unsigned char *)malloc(width*height*ncomp);

	for(i=0;i<ncomp;i++)
	{
		sc=get_byte();
		ts=get_byte();

		component[sc-1].huff_dc=&hufftable_dc[ts>>4];
		component[sc-1].huff_ac=&hufftable_ac[ts&15];
	}

	ss_start=get_byte();
	ss_end=get_byte();
	sbits=get_byte();

	if((ss_start!=0)||(ss_end!=63))
		return 0;

	if(ncomp==3)
	{
		dux=2+component[1].smpx-component[0].smpx;
		duy=2+component[1].smpy-component[0].smpy;
		dvx=2+component[2].smpx-component[0].smpx;
		dvy=2+component[2].smpy-component[0].smpy;
	}

	dt=0;
	bfree=0;
	strm_skip(32);

	blockn=0;
	ymin=0;

	for(nny=0;nny<yblock;nny++)
	{
		ymax=ymin+blocky;

		if(ymax>height)
			ymax=height;

		xmin=0;

		for(nnx=0;nnx<xblock;nnx++)
		{
			xmax=xmin+blockx;

			if(xmax>width)
				xmax=width;

			decode_block();

			blockn++;

			if((blockn==restart_int)&&((nnx<xblock-1)||(nny<yblock-1)))
			{
				blockn=0;

				if(bfree!=0)
					strm_skip(8-bfree);

				if(word_dec(8)!=255)
					return 0;

				for(i=0;i<ncomp;i++)
					component[i].dc_prev=0;
			}

			if(ncomp==3)
			{
				y1=u1=v1=0;
				adr1=(height-1-ymin)*width+xmin;

				for(i=ymin;i<ymax;i++)
				{
					adr2=adr1;
					adr1-=width;
					y2=y1;
					y1+=16;
					u3=(u1>>1)<<4;
					u1+=duy;
					v3=(v1>>1)<<4;
					v1+=dvy;
					u2=v2=0;

					for(j=xmin;j<xmax;j++)
					{
						cy=component[0].t[y2++];
						cu=component[1].t[u3+(u2>>1)]-128.0f;
						cv=component[2].t[v3+(v2>>1)]-128.0f;

						Image->Data[3*adr2]=clamptobyte((int)(cy+1.772f*cu));
						Image->Data[3*adr2+1]=clamptobyte((int)(cy-0.34414f*cu-0.71414f*cv));
						Image->Data[3*adr2+2]=clamptobyte((int)(cy+1.402f*cv));
						adr2++;

						u2+=dux;
						v2+=dvx;
					}
				}
			}
			else
				if(ncomp==1)
				{
					y1=0;
					adr1=(height-1-ymin)*width+xmin;

					for(i=ymin;i<ymax;i++)
					{
						adr2=adr1;
						adr1-=width;
						y2=y1;
						y1+=16;

						for(j=xmin;j<xmax;j++)
							Image->Data[adr2++]=clamptobyte((int)(component[0].t[y2++]));
					}
				}

			xmin=xmax;
		}

		ymin=ymax;
	}

	return 1;
}

int decode_image(IMAGE_t *Image)
{
	int w;
	unsigned char a, hdr, scan;

	eof=0;
	bpos=data;
	dend=bpos+bsize;
	w=((get_byte()<<8)+get_byte());

	if(w!=0xffd8)
		return 0;

	hdr=scan=0;

	while(eof==0)
	{
		a=(unsigned char)get_byte();

		if(a!=0xff)
			return 0;

		a=(unsigned char)get_byte();
		w=((get_byte()<<8)+get_byte());

		switch(a)
		{
			case 0xe0:
				if(hdr!=0)
					break;

				if(get_byte()!='J')
					return 0;

				if(get_byte()!='F')
					return 0;

				if(get_byte()!='I')
					return 0;

				if(get_byte()!='F')
					return 0;

				hdr=1;
				w-=4;
				break;

			case 0xc0:
				get_info();
				w=0;
				break;

			case 0xc4:
				decode_hufftable(w);
				w=0;
				break;

			case 0xd9:
				w=0;
				break;

			case 0xda:
				if(scan!=0)
					break;

				scan=1;

				if(!decode_scan(Image))
					return 0;

				w=0;
				eof=1;
				break;

			case 0xdb:
				decode_qtable(w);
				w=0;
				break;

			case 0xdd:
				restart_int=((get_byte()<<8)+get_byte());
				w=0;
				break;
		}

		while(w>2) 
		{
			get_byte();
			w--;
		}
	}

	return 1;
}

int JPEG_Load(char *filename, IMAGE_t *Image)
{
	int i;

	for(i=0;i<4;i++)
		component[i].dc_prev=0;

	restart_int=-1;

	if(file_open(filename)==1)
	{
		int ret=decode_image(Image);

		file_close();

		if(!ret)
			return 0;

		return 1;
	}
	else
		return 0;
}
