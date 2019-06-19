# Author: Ivan Svarkovsky (ivansvarkovsky@gmail.com)
# Date: 14/06/2019


# Makefile 

binary: snake.o libm_deq.a libgfx.a
	cc -o snake snake.o  -L. -lm_deq -lgfx -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lm \
	-Os -s -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--strip-all

snake.o: snake.c
	cc -c snake.c -Os -s -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--strip-all

libgfx.a: gfx.o
	ar cr libgfx.a gfx.o 

h_gfx.o: hgfx.c
	cc -c hgfx.c -Os -s -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--strip-all

libm_deq.a: m_deq.o
	ar cr libm_deq.a m_deq.o 

h_m_deq.o: hm_deq.c
	cc -c hm_deq.c -Os -s -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--strip-all

clean:
	rm -f *.o *.a snake


# make
# cc -c snake.c
# cc    -c -o m_deq.o m_deq.c
# ar cr libm_deq.a m_deq.o 
# cc    -c -o gfx.o gfx.c
# ar cr libgfx.a gfx.o 
# cc -o snake snake.o  -L. -lm_deq -lgfx -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lm \
# -Os -s -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -Wl,--strip-all


# ./snake


# make clean
# rm -f *.o *.a snake



