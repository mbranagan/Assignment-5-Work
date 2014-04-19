CLIBS=		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include #2011
CPATHS=		-framework GLUT -I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


main:		main.cpp Dem.cpp Triangle.cpp
		g++ main.cpp Dem.cpp Triangle.cpp -o main -framework GLUT -framework OpenGL -framework Cocoa

testing:	testing.cpp
		g++ testing.cpp -o testing -framework GLUT -framework OpenGL -framework Cocoa 
text:		text.cpp
		g++ text.cpp -o text -framework GLUT -framework OpenGL -framework Cocoa
clean:
		rm *.o