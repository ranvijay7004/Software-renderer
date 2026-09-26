#include <SFML/Graphics.hpp>
#include <iostream>

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

    int edgefunction(int ax , int ay , int bx , int by , int px , int py){
        return (bx-ax)*(py-ay) - (by-ay)*(px-ax);
    }

    bool isTopLeft(int ax , int ay , int bx , int by){
                bool istop = (ay == by) && (bx > ax);
                bool isleft = (ay >by);
                return istop || isleft;
            }

    //always have clockwise direction to form triangle
    void filltriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2
        ,int x3 , int y3 , uint8_t r ,  uint8_t g , uint8_t b , uint8_t a){

            int xmax = std::max({x1,x2,x3});
            int xmin = std::min({x1,x2,x3});
            int ymax = std::max({y1,y2,y3});
            int ymin = std::min({y1,y2,y3});

            for(int x = xmin ; x <= xmax ; x++){
                for(int y = ymin ; y <= ymax ; y++){
                    int w0 = edgefunction(x1,y1,x2,y2,x,y);
                    int w1 = edgefunction(x2,y2,x3,y3,x,y);
                    int w2 = edgefunction(x3,y3,x1,y1,x,y);

                    int bias0 = isTopLeft(x1,y1,x2,y2) ? 0 : -1;
                    int bias1 = isTopLeft(x2,y2,x3,y3) ? 0 : -1;
                    int bias2 = isTopLeft(x3,y3,x1,y1) ? 0 : -1;

                    bool inside = (w0 + bias0 >= 0 && w1 + bias1 >= 0
                        && w2 + bias2 >= 0);
                    
                    if(inside){setpixel(framebuffer,x,y,r,g,b,a);}  
                        
                }
            }
        }

int main(){

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Window");

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);

    
    // Triangle 1: top-left, top-right, bottom-right (clockwise)
    filltriangle(framebuffer, 200, 200, 600, 200, 600, 500, 255, 0, 0, 255);

    // Triangle 2: bottom-right, bottom-left, top-left (clockwise)
    filltriangle(framebuffer, 600, 500, 200, 500, 200, 200, 0, 255, 0, 255);
    //drawcube(framebuffer,200,200,400,200,400,400,200,400,
    //100,100,300,100,300,300,100,300,0,0,255,255);

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



