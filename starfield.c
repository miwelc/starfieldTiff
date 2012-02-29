/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <psppower.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#define RGB(r, g, b) ((r)|((g)<<8)|((b)<<16))
#define MSG_DISP_TIME 30
#define Z_MAX_DIST 100
#define STAR_MAX_VELOCITY 2
#define MAX_INIT_STAR_X SCREEN_WIDTH/2*Z_MAX_DIST
#define MAX_INIT_STAR_Y SCREEN_HEIGHT/2*Z_MAX_DIST
#define MAX_STARS (320*240*20)/100

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define PIXEL_SIZE (4) /* change this if you change to another screenmode */
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE (BUF_WIDTH SCR_HEIGHT * 2) /* zbuffer seems to be 16-bit? */

#define IS_ALPHA(color) (((color)&0xff000000)==0xff000000?0:1)
#define FRAMEBUFFER_SIZE (PSP_LINE_SIZE*SCREEN_HEIGHT*4)
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

#define	PSP_LINE_SIZE 512
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

typedef u32 Color;
#define A(color) ((u8)(color >> 24 & 0xFF))
#define B(color) ((u8)(color >> 16 & 0xFF))
#define G(color) ((u8)(color >> 8 & 0xFF))
#define R(color) ((u8)(color & 0xFF))

static int dispBufferNumber;
static int initialized = 0;

unsigned int __attribute__((aligned(16))) list[262144];

Color* g_vram_base = (Color*) (0x40000000 | 0x04000000);

Color* getVramDrawBuffer()
{
	Color* vram = (Color*) g_vram_base;
	if (dispBufferNumber == 0) vram += FRAMEBUFFER_SIZE / sizeof(Color);
	return vram;
}

void fillScreenRect(Color color, int x0, int y0, int width, int height)
{
	if (!initialized) return;
	int skipX = PSP_LINE_SIZE - width;
	int x, y;
	Color* data = getVramDrawBuffer() + x0 + y0 * PSP_LINE_SIZE;
	for (y = 0; y < height; y++, data += skipX) {
		for (x = 0; x < width; x++, data++) *data = color;
	}
}

void guStart()
{
	sceGuStart(GU_DIRECT, list);
}

void flipScreen()
{
	if (!initialized) return;
	sceGuSwapBuffers();
	dispBufferNumber ^= 1;
}

void initGraphics()
{
	dispBufferNumber = 0;

	sceGuInit();

	guStart();
	sceGuDrawBuffer(GU_PSM_8888, (void*)FRAMEBUFFER_SIZE, PSP_LINE_SIZE);
	sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)0, PSP_LINE_SIZE);
	sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	sceGuDepthBuffer((void*) (FRAMEBUFFER_SIZE*2), PSP_LINE_SIZE);
	sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuDepthRange(0xc350, 0x2710);
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuAlphaFunc(GU_GREATER, 0, 0xff);
	sceGuEnable(GU_ALPHA_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuTexMode(GU_PSM_8888, 0, 0, 0);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	sceGuAmbient(0xffffffff);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	initialized = 1;
}

void putPixelScreen(Color color, int x, int y)
{
	Color* vram = getVramDrawBuffer();
	vram[PSP_LINE_SIZE * y + x] = color;
}

struct star
{
    int x,y,z;
    int velocity;
};

struct starfield
{
    struct star tab[MAX_STARS];
    int nb_stars;
    int z_move;
};

int t_disp = 0;
struct timeval tv;
int show_fps = 0, start_fps, frame = 0;

static void init_star(struct star * star, int z_move)
{
    star->velocity = rand() % STAR_MAX_VELOCITY+1;

    star->x = rand() % (2*MAX_INIT_STAR_X)-MAX_INIT_STAR_X;
    star->y = rand() % (2*MAX_INIT_STAR_Y)-MAX_INIT_STAR_Y;

    if(z_move >= 0)
        star->z = Z_MAX_DIST;
    else
        star->z = rand() % Z_MAX_DIST/2+1;
}

static void move_star(struct star * star, int z_move)
{
    star->z -= z_move * star->velocity;

    if (star->z <= 0 || star->z > Z_MAX_DIST)
        init_star(star, z_move);
}

static void draw_star(struct star * star, int z_move)
{
    int x_draw, y_draw;

    x_draw = star->x / star->z + SCREEN_WIDTH / 2;
    if(x_draw < 1 || x_draw >= SCREEN_WIDTH)
    {
       init_star(star, z_move);
       return;
    }

    y_draw = star->y / star->z + SCREEN_HEIGHT/2;

    if(y_draw < 1 || y_draw >= SCREEN_HEIGHT)
    {
       init_star(star, z_move);
       return;
    }

    putPixelScreen(RGB(255, 255, 255), x_draw, y_draw);
    if(star->z < 5*Z_MAX_DIST/6)
    {
       putPixelScreen(RGB(255, 255, 255), x_draw, y_draw-1);
       putPixelScreen(RGB(255, 255, 255), x_draw-1, y_draw);
       if(star->z < Z_MAX_DIST/2)
       {
          putPixelScreen(RGB(255, 255, 255), x_draw+1, y_draw);
          putPixelScreen(RGB(255, 255, 255), x_draw, y_draw+1);
       }
    }
}

static void starfield_add_stars(struct starfield * starfield, int nb_to_add)
{
    int i, old_nb_stars;

    old_nb_stars = starfield->nb_stars;
    starfield->nb_stars += nb_to_add;

    if(starfield->nb_stars > (SCREEN_WIDTH*SCREEN_HEIGHT*20)/100)
        starfield->nb_stars = (SCREEN_WIDTH*SCREEN_HEIGHT*20)/100;

    for(i=old_nb_stars; i < starfield->nb_stars; i++)
    {
        init_star(&(starfield->tab[i]), starfield->z_move);
    }
}

static void starfield_del_stars(struct starfield * starfield, int nb_to_del)
{
    starfield->nb_stars -= nb_to_del;
    if(starfield->nb_stars < 0)
        starfield->nb_stars = 0;
}

static void starfield_move_and_draw(struct starfield * starfield)
{
    int i;
    for(i=0;i<starfield->nb_stars;++i)
    {
        move_star(&(starfield->tab[i]), starfield->z_move);
        draw_star(&(starfield->tab[i]), starfield->z_move);
    }
}

static struct starfield starfield;

int main()
{
    initGraphics();
    SceCtrlData pad, pad_old;
    sceCtrlReadBufferPositive(&pad_old, 1);
    
    starfield.nb_stars = 0;
    starfield.z_move = 1;
    starfield_add_stars(&starfield, 600);
    
    while(1) {
    	fillScreenRect(RGB(0, 0, 0), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

      starfield_move_and_draw(&starfield);
	  Print(0,0,RGB(255, 255, 255),"StarField - Ported to the TIFF exploit by BlackSith");
	  Print(2,1,RGB(255, 255, 255),"Originally ported to the PSP by Karmon");
	  Print(0,30,RGB(255, 255, 255),"..Press Start to Exit..");

      //char str_buffer[40];
      if(t_disp > 0)
      {
         t_disp--;
         //sprintf(str_buffer, sizeof(str_buffer), "Stars:%d \nSpeed:%d", starfield.nb_stars, starfield.z_move);
         //print(3, SCREEN_HEIGHT-10, RGB(255, 255, 255), str_buffer);
      }

      /*if(frame == 0)
      {
         gettimeofday(&tv, NULL);
         start_fps = tv.tv_sec;
      }

      if(show_fps > 0)
      {
         gettimeofday(&tv, NULL);
         //snprintf(str_buffer, sizeof(str_buffer), "FPS: %ld", frame/(tv.tv_sec-start_fps+1));
         //printTextScreen(3, 3, str_buffer, RGB(255, 255, 255));
      }*/
      frame++;
    
      sceCtrlReadBufferPositive(&pad, 1);
      if(pad.Buttons != pad_old.Buttons){
      	pad_old = pad;
        if(pad.Buttons & PSP_CTRL_UP) {
          starfield.z_move++;
          t_disp = MSG_DISP_TIME;
      } else if(pad.Buttons & PSP_CTRL_DOWN) {
          starfield.z_move--;
          t_disp = MSG_DISP_TIME;
        }
        if(pad.Buttons & PSP_CTRL_RIGHT) {
          starfield_add_stars(&starfield, 100);
          t_disp=MSG_DISP_TIME;
      } else if(pad.Buttons & PSP_CTRL_LEFT) {
          starfield_del_stars(&starfield, 100);
          t_disp=MSG_DISP_TIME;
        }
        if(pad.Buttons & PSP_CTRL_CROSS) {
          if(show_fps > 0)
            show_fps--;
          else
            show_fps++;
			  }
			}
			flipScreen();
			sceDisplayWaitVblankStart();
		if(pad.Buttons & PSP_CTRL_START) { vshKernelExitVSHVSH(0); }
		}

    //sceKernelSleepThread();
    return(0);
}