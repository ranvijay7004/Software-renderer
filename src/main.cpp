#include <SFML/Graphics.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;

void setpixel(std::vector<uint8_t>& framebuffer , int x ,int y,
    uint8_t r , uint8_t g, uint8_t b , uint8_t a ){

        int index = (y * WIDTH + x)*4;

        framebuffer[index + 0] = r;
        framebuffer[index + 1] = g;
        framebuffer[index + 2] = b;
        framebuffer[index + 3] = a;
    }

void drawline(std::vector<uint8_t>& framebuffer , int x0 , int y0 , int x1 , 
     int y1, uint8_t r , uint8_t g, uint8_t b , uint8_t a ){

        bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
        if(steep){
            std::swap(x0,y0);
            std::swap(x1,y1);
        }

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int cx,cy;

        if(x1 > x0){
            cx = +1;
        }
        else{cx = -1;}

        if(y1 > y0){cy = +1;}
        else{cy = -1;}

        int x = x0;
        int y = y0;
        int error = 0;

        for(int i= 0 ; i <= dx ; i++){

            if(steep){ setpixel(framebuffer,y,x,r,g,b,a);}
            else{setpixel(framebuffer,x,y,r,g,b,a);}

            error += dy;
            if(error >= dx){
                y += cy;
                error -= dx;
            }
            x += cx;
        }
    }

    void drawtriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1, int x2,
        int y2 , int x3 , int y3 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

            drawline(framebuffer,x1,y1,x2,y2,r,g,b,a);
            drawline(framebuffer,x2,y2,x3,y3,r,g,b,a);
            drawline(framebuffer,x3,y3,x1,y1,r,g,b,a);
    }

   void drawcube(std::vector<uint8_t>& framebuffer , int x1, int y1 , int x2 , int y2 , int x3 , int y3,
        int x4 , int y4 , int x5 , int y5 , int x6 ,  int y6 , int x7 , int y7 , int x8 , int y8 ,
        uint8_t r ,  uint8_t g , uint8_t b , uint8_t a){

            drawline(framebuffer,x1,y1,x2,y2,r,g,b,a);
            drawline(framebuffer,x2,y2,x3,y3,r,g,b,a);
            drawline(framebuffer,x3,y3,x4,y4,r,g,b,a);
            drawline(framebuffer,x4,y4,x1,y1,r,g,b,a);
            drawline(framebuffer,x4,y4,x8,y8,r,g,b,a);
            drawline(framebuffer,x1,y1,x5,y5,r,g,b,a);
            drawline(framebuffer,x2,y2,x6,y6,r,g,b,a);
            drawline(framebuffer,x3,y3,x7,y7,r,g,b,a);
            drawline(framebuffer,x5,y5,x6,y6,r,g,b,a);
            drawline(framebuffer,x6,y6,x7,y7,r,g,b,a);
            drawline(framebuffer,x7,y7,x8,y8,r,g,b,a);
            drawline(framebuffer,x8,y8,x5,y5,r,g,b,a);
        }

int main(){

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Window");

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);

    drawtriangle(framebuffer,600,400,700,500,500,500,0,255,255,255);
    drawcube(framebuffer,200,200,400,200,400,400,200,400,100,100,300,100,300,300,100,300,0,0,255,255);

    sf::Texture texture;
    texture.create(WIDTH,HEIGHT);
    texture.update(framebuffer.data());
    sf::Sprite pixelsprite;
    pixelsprite.setTexture(texture);
    
    while (window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
            window.close();
        }

        window.clear();
        window.draw(pixelsprite);
        window.display();
    }
}



