CLIBS=		-lm -lglut -lGL -lGLU -lX11 # 2014
#CPATHS=	-L/usr/X11R6/lib -I/usr/X11R6/include #2011
CPATHS=		-framework GLUT -I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)


main:		main.cpp Dragon.cpp
		g++ main.cpp Dragon.cpp -o main -framework GLUT -framework OpenGL -framework Cocoa

maintest:	maintest.cpp Dem.cpp
		g++ maintest.cpp Dem.cpp -o maintest -framework GLUT -framework OpenGL -framework Cocoa
Dragon:		Dragon.cpp
		g++ Dragon.cpp -o Dragon -framework GLUT -framework OpenGL -framework Cocoa
square:		square.cpp
		g++ square.cpp -o square  -framework GLUT -framework OpenGl -framework Cocoa

BranaganG1:	BranaganG1.cpp
		g++ BranaganG1.cpp -o BranaganG1 -framework GLUT -framework OpenGL -frameworkCocoa

dragonFractal:	dragonFractal.cpp
		g++ dragonFractal.cpp -o dragonFractal -framework GLUT -framework OpenGL -framework Cocoa
fracs:		fracs.cpp
		g++ fracs.cpp -o fracs -framework GLUT -framework OpenGL -framework Cocoa

menutest:	menutest.cpp
		g++ menutest.cpp -o menutest -framework GLUT -framework OpenGl -framework Cocoa

menu2:		menu2.cpp
		g++ menu2.cpp -o menu2 -framework GLUT -framework OpenGL -framework Cocoa

menu:		menu.cpp
		g++ menu.cpp -o menu $(CFLAGS)

mouse:		mouse.cpp
		g++ mouse.cpp -o mouse -framework GLUT -framework OpenGL -framework Cocoa
font:		font.cpp
		g++ font.cpp -o font -framework GLUT -framework OpenGL -framework Cocoa

text:		text.cpp
		g++ text.cpp -o text -framework GLUT -framework OpenGL -framework Cocoa
clean:
		rm *.o