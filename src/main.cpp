#include <SFML/Graphics.hpp>
#include <cmath>

const int WIDTH = 1920;
const int HEIGHT = 1080;

struct Vec2{
    float x,y;

    Vec2 operator+(const Vec2& other) const {
        Vec2 result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    Vec2 operator-(const Vec2& other) const {
        Vec2 result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }

    Vec2 operator*(float scale) const {
        Vec2 result;
        result.x = x * scale;
        result.y = y * scale;
        return result;
    }
};

float dot(Vec2 a , Vec2 b) {
    return a.x*b.x + a.y*b.y;
}

struct Vec3{
    float x,y,z;

    Vec3 operator+(const Vec3& other) const {
        Vec3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }

    Vec3 operator-(const Vec3& other) const {
        Vec3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }

    Vec3 operator*(float scale) const {
        Vec3 result;
        result.x = x * scale;
        result.y = y * scale;
        result.z = z * scale;
        return result;

    }
};

float length(Vec3& a){
    return std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vec3 normalise(Vec3& a){
    float len = length(a);
    if(len == 0) return a;
    return {a.x/len , a.y/len , a.z/len};
}

float dot(Vec3 a , Vec3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 cross(Vec3 a , Vec3 b){
    Vec3 result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

struct Vec4{
    float x,y,z,w;
};

float dot(Vec4& a , Vec4& b){
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

struct mat4{

    Vec4 row0 , row1 , row2 , row3;
    
    Vec4 column0(){return {row0.x , row1.x , row2.x , row3.x};}
    Vec4 column1(){return {row0.y , row1.y , row2.y , row3.y};}
    Vec4 column2(){return {row0.z , row1.z , row2.z , row3.z};}
    Vec4 column3(){return {row0.w , row1.w , row2.w , row3.w};}

    Vec4 multiply(Vec4& a){
        Vec4 result;
        result.x = dot(row0,a);
        result.y = dot(row1,a);
        result.z = dot(row2,a);
        result.w = dot(row3,a);
        return result;
    }

    mat4 multiply(mat4& a){

        Vec4 c0 = a.column0();
        Vec4 c1 = a.column1();
        Vec4 c2 = a.column2();
        Vec4 c3 = a.column3();

        mat4 result;
        result.row0 = {dot(row0,c0) , dot(row0,c1) , dot(row0,c2) , dot(row0,c3)};
        result.row1 = {dot(row1,c0) , dot(row1,c1) , dot(row1,c2) , dot(row1,c3)};
        result.row2 = {dot(row2,c0) , dot(row2,c1) , dot(row2,c2) , dot(row2,c3)};
        result.row3 = {dot(row3,c0) , dot(row3,c1) , dot(row3,c2) , dot(row3,c3)};
        return result; 
    }

    static mat4 transaltion(float dx , float dy , float dz){
        mat4 result;
        result.row0 = {1,0,0,dx};
        result.row1 = {0,1,0,dy};
        result.row2 = {0,0,1,dz};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 scale(float dx , float dy , float dz){
        mat4 result;
        result.row0 = {dx,0,0,0};
        result.row1 = {0,dy,0,0};
        result.row2 = {0,0,dz,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 rotationZ(float angledegree){

        float angleradians = angledegree * (3.14159265f / 180.0f);
        float c = cos(angleradians);
        float s = sin(angleradians);

        mat4 result;
        result.row0 = {c,-s,0,0};
        result.row1 = {s,c,0,0};
        result.row2 = {0,0,1,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 rotationY(float angledegree){

        float angleradians = angledegree * (3.14159265f / 180.0f);
        float c = cos(angleradians);
        float s = sin(angleradians);

        mat4 result;
        result.row0 = {c,0,-s,0};
        result.row1 = {0,1,0,0};
        result.row2 = {s,0,c,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 rotationX(float angledegree){

        float angleradians = angledegree * (3.14159265f / 180.0f);
        float c = cos(angleradians);
        float s = sin(angleradians);

        mat4 result;
        result.row0 = {1,0,0,0};
        result.row1 = {0,c,s,0};
        result.row2 = {0,-s,c,0};
        result.row3 = {0,0,0,1};
        return result;
    }

    static mat4 perspective(float p){
        mat4 result;
        result.row0 = {p,0,0,0};
        result.row1 = {0,p,0,0};
        result.row2 = {0,0,1,0};
        result.row3 = {0,0,1,0};
        return result; 
    }
};

void setpixel(std::vector<uint8_t>& framebuffer , int x , int y , uint8_t r , uint8_t g , uint8_t b , uint8_t a ){
    
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

    int index = (y * WIDTH + x) * 4;

    framebuffer[index + 0] = r;
    framebuffer[index + 1] = g;
    framebuffer[index + 2] = b;
    framebuffer[index + 3] = a;
}

void drawline(std::vector<uint8_t>& framebuffer , int x1 , int y1 , 
    int x2 , int y2 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

        bool steep = std::abs(y2-y1) > std::abs(x2-x1);

        if(steep){
            std::swap(x1,y1);
            std::swap(x2,y2);
        }

        int dx , dy , cx , cy;
        
        if(x2>x1) cx = +1;
        else cx = -1;

        if(y2>y1) cy = +1;
        else cy = -1;

        dx = std::abs(x2-x1);
        dy = std::abs(y2-y1);

        int x = x1;
        int y = y1;

        int error = 0;
        
        for(int i = 0 ; i <= dx ; i++){

            if(steep) setpixel(framebuffer,y,x,r,g,b,a);
            else setpixel(framebuffer,x,y,r,g,b,a);

            error += dy;
            if(error >= dx){
                y += cy;
                error -= dx;
            }
            x += cx;
        }
}

void drawtriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 
    ,int x3 , int y3 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

        drawline(framebuffer,x1,y1,x2,y2,r,g,b,a);
        drawline(framebuffer,x2,y2,x3,y3,r,g,b,a);
        drawline(framebuffer,x3,y3,x1,y1,r,g,b,a);
}


int checkinside(int ax , int ay ,int bx ,int by , int px , int py){
    return (bx-ax)*(py-ay) - (by-ay)*(px-ax);
}

bool istopleft(int ax , int ay , int bx , int by){

    bool istop = ay == by && bx > ax;
    bool isleft = ay > by;

    return istop || isleft; 
}

void filltriangle(std::vector<uint8_t>& framebuffer , int x1 , int y1 , int x2 , int y2 
        ,int x3 , int y3 , uint8_t r , uint8_t g , uint8_t b , uint8_t a){

    int minX = std::min({x1,x2,x3});
    int maxX = std::max({x1,x2,x3});
    int minY = std::min({y1,y2,y3});
    int maxY = std::max({y1,y2,y3});

    int bias1 = istopleft(x1,y1,x2,y2);
    int bias2 = istopleft(x2,y2,x3,y3);
    int bias3 = istopleft(x3,y3,x1,y1);

    for(int x = minX ; x < maxX ; x++){
        for(int y = minY ; y < maxY ; y++){

            int w1 = checkinside(x1,y1,x2,y2,x,y);
            int w2 = checkinside(x2,y2,x3,y3,x,y);
            int w3 = checkinside(x3,y3,x1,y1,x,y);
                
            bool inside = (w1 + bias1 > 0 && w2 + bias2 > 0 && w3 + bias3 > 0);

            if(inside){
                setpixel(framebuffer,x,y,r,g,b,a);
            }
        }
    }
}

bool toscreen(mat4& x , Vec3& p , Vec2& out){
    Vec4 k = {p.x , p.y , p.z , 1};
    k = x.multiply(k);

    if(k.w <= 0.5f) return false;
    out = {k.x/k.w + WIDTH/2 , k.y/k.w + HEIGHT/2};
    return true;
}

void drawcube(std::vector<uint8_t>& framebuffer , Vec3 vert[8] ,
    float angle1 , float angle2 , float angle3){

    Vec2 sp[8];
    bool visible[8];
    float camX = 0 , camY = 0  , camZ = -20;

    mat4 rotateY = mat4::rotationY(angle1);
    mat4 rotateX = mat4::rotationX(angle2);
    mat4 rotateZ = mat4::rotationZ(angle3);
    mat4 view = mat4::transaltion(-camX , -camY , -camZ);
    mat4 pers = mat4::perspective(800.0f);
    
    mat4 x = pers.multiply(view)
            .multiply(rotateY)
            .multiply(rotateX)
            .multiply(rotateZ);

    for(int i  = 0 ; i < 8 ; i++){
        visible[i] = toscreen(x,vert[i],sp[i]);
    }

    int faces [6][4] = {
        {0,1,2,3} , {1,5,6,2},
        {5,4,7,6} , {4,0,3,7},
        {1,0,4,5} , {3,2,6,7}
    };

    int colors[6][4] = {
        {255,107,107,255} , {255,217,61,255},
        {107,203,119,255} , {77,150,255,255},
        {199,125,255,255} , {255,159,67,255}
    };

    for(int i = 0 ; i < 6 ; i++){
        int v1 = faces[i][0];
        int v2 = faces[i][1];
        int v3 = faces[i][2];
        int v4 = faces[i][3];

        int c1 = colors[i][0];
        int c2 = colors[i][1];
        int c3 = colors[i][2];
        int c4 = colors[i][3];

        if(visible[v1] && visible[v2] && visible[v3] && visible[v4]){
            filltriangle(framebuffer,sp[v1].x , sp[v1].y , sp[v2].x
            , sp[v2].y , sp[v3].x,sp[v3].y , c1, c2 , c3 ,c4);

            filltriangle(framebuffer,sp[v1].x , sp[v1].y , sp[v3].x
            , sp[v3].y , sp[v4].x,sp[v4].y , c1 , c2 , c3 , c4);
        }
    }
}

int main(){

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);

    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT) , "renderer");
    window.setFramerateLimit(60);

    Vec3 cubevert[8] = {
        {-3,-3,-3} , {3,-3,-3} , {3,3,-3} , {-3,3,-3}, //front face
        {-3,-3,3} , {3,-3,3} , {3,3,3} , {-3,3,3} //backface
    };

    float angle1 = 0;
    float angle2 = 35.264;
    float angle3 = 45;
    

    sf::Texture pixeltexture;
    pixeltexture.create(WIDTH,HEIGHT);
    sf::Sprite pixelsprite;
    pixelsprite.setTexture(pixeltexture);

    sf::Event event;

    while(window.isOpen()){
        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
   
        drawcube(framebuffer,cubevert, angle1 , angle2 , angle3);
        angle1 += 1.0f;

        pixeltexture.update(framebuffer.data());
        std::fill(framebuffer.begin() , framebuffer.end() , 0);

        window.clear();
        window.draw(pixelsprite);
        window.display();
    }
    return 0;
}