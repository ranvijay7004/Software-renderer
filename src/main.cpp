#include <SFML/Graphics.hpp>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

struct Vec2{
    float x,y;

    Vec2 operator+(const Vec2& other) const{
        Vec2 result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result; 
    }

    Vec2 operator-(const Vec2& other) const{
        Vec2 result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    Vec2 operator*(float scalar) const{
        Vec2 result;
        result.x = x * scalar;
        result.y = y * scalar;
        return result;
    }
};

float dot (const Vec2& a , const Vec2& b){
    return a.x*b.x + a.y*b.y;
}

struct Vec3{
    float x,y,z;

    Vec3 operator+(const Vec3& other) const{
        Vec3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }

    Vec3 operator-(const Vec3& other) const{
        Vec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }

    Vec3 operator*(float scalar){
        Vec3 result;
        result.x = x * scalar;
        result.y = y * scalar;
        result.z = z * scalar;
        return result;
    }

    Vec2 screenpoint(int screenwidth , int screenheight){
        float scale = 200.0f;
        float CenterX = screenwidth / 2.00f;
        float CenterY = screenheight / 2.00f;
        
        Vec2 result;
        result.x = ((x/z) * scale) + CenterX;
        result.y = ((y/z) * scale) + CenterY;
        
        return result;
    }
};

float dot(const Vec3& a ,const Vec3& b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 cross(const Vec3& a , const Vec3& b){
    Vec3 result;
    result.x = a.y * b.z - a.z* b.y;
    result.y = a.z * b.x - b.z * a.x;
    result.z = a.x * b.y - b.x * a.y;
    return result;
}

struct Vec4{
    float x,y,z,w;
};
    

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

   void drawcube(std::vector<uint8_t>& framebuffer , Vec3 vert[8] , uint8_t r , uint8_t g ,  
    uint8_t b , uint8_t a){
        
        Vec2 p[8];

        for(int i = 0 ; i < 8 ; i++){
            p[i] = vert[i].screenpoint(WIDTH,HEIGHT);    
        }

        int edges [12][2] = {
            {0,1} , {1,2} , {2,3} , {3,0}, // front face
            {4,5} , {5,6} , {6,7} , {7,4},  // back face
            {0,4} , {1,5} , {2,6} , {3,7} // connecting edges
        };
        
        for(int i  = 0 ; i < 12 ; i++){
            int indxa = edges[i][0];
            int indxb = edges[i][1];
            drawline(framebuffer,(int)p[indxa].x , (int)p[indxa].y , (int)p[indxb].x
            ,(int)p[indxb].y ,r,g,b,a); 
        };
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

    Vec3 cubeVerts[8] = {
    {-1, -1, 5}, {1, -1, 5}, {1, 1, 5}, {-1, 1, 5},   // indices 0,1,2,3 — front face
    {-1, -1, 8}, {1, -1, 8}, {1, 1, 8}, {-1, 1, 8}    // indices 4,5,6,7 — back face
    };

    drawcube(framebuffer, cubeVerts, 255,255,255,255);
    
    // Triangle 1: top-left, top-right, bottom-right (clockwise)
    //filltriangle(framebuffer, 200, 200, 600, 200, 600, 500, 255, 0, 0, 255);

    // Triangle 2: bottom-right, bottom-left, top-left (clockwise)
    //filltriangle(framebuffer, 600, 500, 200, 500, 200, 200, 0, 255, 0, 255);
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



