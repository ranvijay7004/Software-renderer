#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

const int WIDTH = 960;
const int HEIGHT = 540;
const float NEAR_W = 1.0f;

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

Vec3 facenormal(Vec3 a , Vec3 b , Vec3 c){
    Vec3 e1 = b-a;
    Vec3 e2 = c-a;
    Vec3 n = cross(e2,e1);
    return normalise(n);
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

struct triangle{
    int x , y , z;
    int tx ,ty ,tz;
};

struct texture{
  int w ,h;

  std::vector<uint8_t> data;
};

struct color{
    uint8_t r,g,b;
};

color sampletexture(texture& tex , float u  , float v){
    
    u = u - std::floor(u);

    int x = (int)(u*tex.w);
    int y = (int)(v*tex.h);

    if(x > tex.w -1) x = tex.w - 1;
    if(y > tex.h -1) y = tex.h - 1;

    int indx = (y * tex.w + x) *3;

    return {tex.data[indx] , tex.data[indx+1] , tex.data[indx+2]};
}

void parsecorner(const std::string& token , int& v , int& t){

    v = std::stoi(token) -1;
    t = -1;

    size_t slash = token.find('/');
    
    if(slash != std::string::npos && slash +1 < token.size() && token[slash+1] != '/'){
        t = std::stoi(token.substr(slash + 1)) -1;
    }
}

bool loadobj(const std::string& path , std::vector<Vec3>& Vertices , std::vector<Vec2>& uvs , std::vector<triangle>& triangles){

    std::ifstream file(path);
    if(!file.is_open()) return false;
    
    std::string line;
    
    while(std::getline(file,line)){
        std::istringstream ss(line);
        std::string tag;
        ss>>tag;
        
        if(tag == "v"){
            Vec3 v;
            ss>>v.x>>v.y>>v.z;
            Vertices.push_back(v);
        }

        else if(tag == "vt"){
            Vec2 uv;
            ss>>uv.x>>uv.y;
            uv.y = 1.0f - uv.y;
            uvs.push_back(uv);
        }

        else if(tag == "f"){

            std::vector<int> vs , ts;
            std::string tokens;

            while(ss>>tokens){
                int v , t;
                parsecorner(tokens , v , t);
                vs.push_back(v);
                ts.push_back(t);
            }
            
            for(int k = 1 ; k+1 < vs.size() ; k++){
                triangle tri;

                tri.x = vs[0] , tri.y = vs[k] , tri.z = vs[k+1];
                tri.tx = ts[0] , tri.ty = ts[k] , tri.tz = ts[k+1];

                triangles.push_back(tri);
            }
        } 
    }
    return true;
}

bool loadppm(const std::string& path , texture& tex){

  std::ifstream file(path,std::ios::binary);
  
  if(!file.is_open()) return false;

  std::string magic;
  int maxval;
  
  file>>magic>>tex.w>>tex.h>>maxval;

  if(magic != "P6" || maxval != 255) return false;

  file.get();
  tex.data.resize(tex.w * tex.h * 3);
  file.read((char*)tex.data.data() , tex.data.size());
  return !file.fail();
}

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

void filltriangle(std::vector<uint8_t>& framebuffer , std::vector<float>& zbuffer , int x1 , int y1 , int x2 , int y2 ,
    int x3 , int y3 ,float z1 , float z2 , float z3 , float s1 , float s2 , float s3 ,float u1 , float v1 ,
    float u2 , float v2 , float u3 , float v3 , texture& tex , uint8_t r , uint8_t g , uint8_t b , uint8_t a){


    int minX = std::max(std::min({x1,x2,x3}) , 0);
    int maxX = std::min(std::max({x1,x2,x3}) , WIDTH);
    int minY = std::max(std::min({y1,y2,y3}) , 0);
    int maxY = std::min(std::max({y1,y2,y3}) , HEIGHT);

    int bias1 = istopleft(x1,y1,x2,y2);
    int bias2 = istopleft(x2,y2,x3,y3);
    int bias3 = istopleft(x3,y3,x1,y1);

    for(int x = minX ; x < maxX ; x++){
        for(int y = minY ; y < maxY ; y++){

            if(x >= WIDTH || x < 0 || y >= HEIGHT || y < 0) continue;

            int w1 = checkinside(x1,y1,x2,y2,x,y);
            int w2 = checkinside(x2,y2,x3,y3,x,y);
            int w3 = checkinside(x3,y3,x1,y1,x,y);
                
            bool inside = (w1 + bias1 > 0 && w2 + bias2 > 0 && w3 + bias3 > 0);

            if(inside){
                float A = w1 + w2 + w3;
                if(A == 0) continue;
                float l1 = (float)w2/A;
                float l2 = (float)w3/A;
                float l3 = (float)w1/A;

                float depth = l1/z1 + l2/z2 + l3/z3;

                int idx = (y * WIDTH) + x;
                if(depth > zbuffer[idx]){
                    zbuffer[idx] = depth;

                    float shade = l1*s1 + l2*s2 + l3*s3;
                    if(shade < 0) shade = 0 ;
                    if(shade > 1) shade = 1;

                    float u = (l1*u1/z1 + l2*u2/z2 + l3*u3/z3) / depth;
                    float v = (l1*v1/z1 + l2*v2/z2 + l3*v3/z3) / depth;

                    color c = sampletexture(tex , u , v);

                    setpixel(framebuffer,x,y,(uint8_t)(c.r*shade),(uint8_t)(c.g*shade),(uint8_t)(c.b*shade),a);
                }
            }
        }
    }
}

bool toscreen(mat4& x , Vec3& p , Vec2& out , float& depth){
    Vec4 k = {p.x , p.y , p.z , 1};
    k = x.multiply(k);

    if(k.w <= 0.5f) return false;
    depth = k.w;
    out = {k.x/k.w + WIDTH/2 , k.y/k.w + HEIGHT/2};
    return true;
}

struct Corner{
    float x,y,w;
    float u,v;
    float shade;
};

Corner lerpcorner(const Corner& a , const Corner& b , float t){

    Corner r;

    r.x = a.x + t * (b.x - a.x);
    r.y = a.y + t * (b.y - a.y);
    r.w = a.w + t * (b.w - a.w);
    r.u = a.u + t * (b.u - a.u);
    r.v = a.v + t * (b.v - a.v);
    r.shade = a.shade + t * (b.shade - a.shade);

    return r;
}

int clipnear(const Corner tri[3] , Corner kept[4]){

    int n = 0;

    for(int i = 0; i < 3 ; i++){

        const Corner& cur = tri[i];
        const Corner& next = tri[(i+1) % 3];

        bool curin = cur.w >= NEAR_W;
        bool nextin = next.w >= NEAR_W;

        if(curin && nextin){
            kept[n++] = next;
        }

        else if(curin && !nextin){
            float t = (NEAR_W - cur.w) / (next.w - cur.w);
            kept[n++] = lerpcorner(cur , next , t);
        }

        else if(!curin && nextin){
            float t = (NEAR_W - cur.w) / (next.w - cur.w);
            kept[n++] = lerpcorner(cur , next , t);
            kept[n++] = next;
        }
    }
    return n;
}

void drawclipped(std::vector<uint8_t>& framebuffer , std::vector<float>& zbuffer , const Corner& a ,
     const Corner& b, const Corner& c , texture& tex , int r , int g , int bl){


    int x1 = (int)(a.x/a.w + WIDTH/2);
    int y1 = (int)(a.y/a.w + HEIGHT/2);
    int x2 = (int)(b.x/b.w + WIDTH/2);
    int y2 = (int)(b.y/b.w + HEIGHT/2);
    int x3 = (int)(c.x/c.w + WIDTH/2);
    int y3 = (int)(c.y/c.w + HEIGHT/2);

    filltriangle(framebuffer , zbuffer , x1 , y1 , x2 , y2 , x3 , y3 ,
                 a.w , b.w , c.w , 
                 a.shade , b.shade  , c.shade ,
                 a.u , a.v , b.u , b.v , c.u , c.v,
                 tex , r , g , bl , 255);
}

void drawmesh(std::vector<uint8_t>& framebuffer , std::vector<float>& zbuffer ,  std::vector<Vec3>& Vertices , std::vector<triangle>& triangles
    ,std::vector<Vec3>& vnormal , texture& tex , std::vector<Vec2>& uvs , float angle1 , float camX , float camY ,
    float camZ , float yaw , float pitch , int color =-1){

    Vec3 light = {-0.6f , -0.6f , -0.7f};
    light = normalise(light);
    float ambient = 0.12f;

    mat4 rotateY = mat4::rotationY(angle1);
    mat4 slide = mat4::transaltion(-camX , -camY , -camZ);
    mat4 pers = mat4::perspective(800.0f);
    mat4 turnY = mat4::rotationY(yaw);
    mat4 turnX = mat4::rotationX(pitch);

    mat4 view = turnX.multiply(turnY).multiply(slide);

    mat4 model = rotateY;
    
    mat4 x = pers.multiply(view).multiply(model);
    

    std::vector<float> vbright(Vertices.size());

    std::vector<Vec4> clip(Vertices.size());


    int colors[6][4] = {
        {255,107,107,255} , {255,217,61,255},
        {107,203,119,255} , {77,150,255,255},
        {199,125,255,255} , {255,159,67,255}
    };
    

    for(int i = 0 ; i < Vertices.size() ; i++){
        Vec4 k  = {Vertices[i].x , Vertices[i].y , Vertices[i].z , 1};
        clip[i] = x.multiply(k);
    }


    for(int i = 0 ; i < Vertices.size() ; i++){

        Vec4 k = {vnormal[i].x , vnormal[i].y , vnormal[i].z , 0};
        k = model.multiply(k);
        Vec3 nr = {k.x , k.y , k.z};

        float d = dot(nr,light);
        if(d < 0) d = 0;

        float b = ambient + (1 - ambient) * d;
        if(b > 1) b =1;

        vbright[i] = b;
    }

    for(int i = 0 ; i < triangles.size() ; i++){
        triangle t = triangles[i];
        
        Vec2 uv1 = {0,0} , uv2 = {0,0} , uv3 = {0,0};

        if(t.tx >= 0){
            uv1 = uvs[t.tx];
            uv2 = uvs[t.ty];
            uv3 = uvs[t.tz];
        }

        int c = (color >= 0) ? color : i/2 % 6;

        Corner tri[3];

        tri[0] = {clip[t.x].x, clip[t.x].y , clip[t.x].w , uv1.x , uv1.y , vbright[t.x]};
        tri[1] = {clip[t.y].x, clip[t.y].y , clip[t.y].w , uv2.x , uv2.y , vbright[t.y]};
        tri[2] = {clip[t.z].x, clip[t.z].y , clip[t.z].w , uv3.x , uv3.y , vbright[t.z]};

        Corner kept[4];
        int n = clipnear(tri , kept);

        for(int k = 1 ; k+1 < n ; k++){
            drawclipped(framebuffer , zbuffer , kept[0] , kept[k] , kept[k+1] , tex ,
                        colors[c][0] , colors[c][1] , colors[c][2]);
        }
    }  
}

std::vector<Vec3> vertexnormals(std::vector<Vec3>& Vertices , std::vector<triangle>& Triangles){
    
        std::vector<Vec3> sum(Vertices.size(), Vec3({0,0,0}));
        for(int i = 0 ; i < Triangles.size() ; i++){
            triangle t = Triangles[i];

            Vec3 n = facenormal(Vertices[t.x] , Vertices[t.y] , Vertices[t.z]);
            sum[t.x] = sum[t.x] + n;
            sum[t.y] = sum[t.y] + n;
            sum[t.z] = sum[t.z] + n;
        }

        for(int i = 0 ; i < sum.size() ; i++){
            sum[i] = normalise(sum[i]);
        }

        return sum;
    };

int main(){

    std::vector<uint8_t> framebuffer(WIDTH*HEIGHT*4);
    std::vector<float> zbuffer(WIDTH*HEIGHT , 0.0f);

    sf::RenderWindow window(sf::VideoMode(1920,1080) , "renderer");
    window.setFramerateLimit(60);
    
    std::vector<triangle> Triangles;
    std::vector<Vec3> Vertices;
    std::vector<Vec2> uvs;
    texture tex;

    if(!loadobj("assets/spiky_uv.obj", Vertices , uvs , Triangles)){
        std::cerr<<"couldn't load model"<<std::endl;
        return 1;
    }
    
    if(!loadppm("assets/checker.ppm" , tex)){
      std::cerr<<"couldn't load texture"<<std::endl;
      return 1;
    }

    std::vector<Vec3> vnormals = vertexnormals(Vertices,Triangles);

    
    float angle1 = 0;


    sf::Texture pixeltexture;
    pixeltexture.create(WIDTH,HEIGHT);
    sf::Sprite pixelsprite;
    pixelsprite.setScale(2.0f , 2.0f);
    pixelsprite.setTexture(pixeltexture);

    sf::Font font;
    if(!font.loadFromFile("assets/font.TTF")) return 1;     // put any .ttf there, or change the path

    sf::Text hud;
    hud.setFont(font);
    hud.setCharacterSize(18);
    hud.setFillColor(sf::Color::White);
    hud.setPosition(10 , 10);

    sf::Text manual;
    manual.setFont(font);
    manual.setCharacterSize(16);
    manual.setFillColor(sf::Color::White);
    manual.setString("CONTROLS\nClick window - lock mouse\nEsc - unlock mouse\nMouse - look around\nW A S D - walk\nSpace - go up\nShift - go down");
    sf::FloatRect mb = manual.getLocalBounds();
    manual.setPosition(window.getSize().x - mb.width - 20 , window.getSize().y - mb.height - mb.top - 20);

    sf::Clock clock;

    sf::Event event;
    
    float speed = 15.0f;
    float turnspeed = 50.0f;

    float camX = 0;
    float camY = 0;
    float camZ = -20.0f;

    float yaw = 0;
    float pitch  = 0;
    
    bool mouselook = false;
    float sensitivity = 0.1f;

    while(window.isOpen()){
        while(window.pollEvent(event)){

            if(event.type == sf::Event::Closed){
                window.close();
            }

            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                mouselook = true;
                window.setMouseCursorVisible(false);
                window.setMouseCursorGrabbed(true);
                sf::Vector2u size = window.getSize();
                sf::Mouse::setPosition(sf::Vector2i(size.x / 2 , size.y / 2) , window);
            }

            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
                mouselook = false;
                window.setMouseCursorVisible(true);
                window.setMouseCursorGrabbed(false);
            }

        }
 

        float dt = clock.restart().asSeconds();
         

        if(mouselook && window.hasFocus()){
            sf::Vector2u size = window.getSize();
            sf::Vector2i center(size.x / 2 , size.y / 2);
            sf::Vector2i pos = sf::Mouse::getPosition(window);

            yaw   += (pos.x - center.x) * sensitivity;
            pitch -= (pos.y - center.y) * sensitivity;

            sf::Mouse::setPosition(center , window);
        }

        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;

        float radians = yaw * 3.14159265f/180.0f;
        float s = std::sin(radians);
        float c = std::cos(radians);


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){camX += s * speed * dt; camZ += c * speed * dt;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){camX -= s * speed * dt; camZ -= c * speed * dt;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){camX -= c * speed * dt; camZ += s * speed * dt;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){camX += c * speed * dt; camZ -= s * speed * dt;}
         
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))  camY -= speed * dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) camY += speed * dt;  

        drawmesh(framebuffer, zbuffer , Vertices , Triangles ,  vnormals , tex ,
        uvs , angle1 , camX , camY , camZ , yaw , pitch , 3);

        angle1 += 1.0f;

        pixeltexture.update(framebuffer.data());
        std::fill(framebuffer.begin() , framebuffer.end() , 0);
        std::fill(zbuffer.begin() , zbuffer.end() , 0);

        window.clear();
        window.draw(pixelsprite);

        hud.setString("FPS " + std::to_string((int)(1 / dt))
        + "\nMS " + std::to_string((int)(1000 * dt))
        + "\nX " + std::to_string((int)camX)
        + "\nY " + std::to_string((int)camY)
        + "\nZ " + std::to_string((int)camZ)
        + "\nYaw " + std::to_string((int)yaw)
        + "\nPitch " + std::to_string((int)pitch));

        window.draw(hud);
        window.draw(manual);
        window.display();
    }
    return 0;
}
 