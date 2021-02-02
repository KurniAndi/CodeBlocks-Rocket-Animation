#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
typedef struct
{ float x;
float y;
}point2D_t;
typedef struct
{
float v[3];
}vector2D_t;
typedef struct
{
float m[3][3];
} matrix2D_t;
typedef struct
{
float r,g,b;
} color_t;
point2D_t Vector2Point1(vector2D_t vec)
{
point2D_t pnt;
pnt.x=vec.v[0];
pnt.y=vec.v[1];
return pnt;
}
vector2D_t Point2Vector1(point2D_t pnt)
{
vector2D_t vec;
vec.v[0]=pnt.x;
vec.v[1]=pnt.y;
vec.v[2]=1.;
return vec;
}
void setColor(float r,float g,float b)
{
glColor3f(r,g,b);
}
void setColor(color_t col)
{
glColor3f(col.r,col.g,col.b);
}
//fungsi untuk membuat titik
void drawDot(float x, float y)
{
glPointSize(3);
glBegin(GL_POINTS);
glVertex2f(x,y);
glEnd();
}
//fungsi untuk membuat garis
void drawLine(float x1, float y1, float x2, float y2)
{
glBegin(GL_LINES);
glVertex2f(x1,y1);
glVertex2f(x2,y2);
glEnd();
}
//fungsi yang digunakan untuk menggambar garis
void drawPolyline(point2D_t p[], int n)
{
int i;
glBegin(GL_LINE_STRIP);
for(i=0;i<n;i++)
glVertex2f(p[i].x,p[i].y);
glEnd();
}
//fungsi untuk menggambar garis dengan bentuk kurva tertutup
void drawPolygon(point2D_t p[], int n)
{
int i;
glBegin(GL_LINE_LOOP);
for(i=0;i<n;i++)
glVertex2f(p[i].x,p[i].y);
glEnd();
}
//fungsi untuk mewarnai area poligon dengan satu warna
void fillPolygon(point2D_t pnt[],int n, color_t color)
{
int i;
setColor(color);
glBegin(GL_POLYGON);
for (i=0;i<n;i++) {
glVertex2f(pnt[i].x, pnt[i].y);
}
glEnd();
}
//fungsi yang menghasilkan efek gradasi jika digunakan untuk mewarnai pada poligon
void gradatePolygon(point2D_t p[], int n,color_t col[])
{
int i;
glBegin(GL_POLYGON);
for(i=0;i<n;i++)
{
setColor(col[i]);
glVertex2f(p[i].x,p[i].y);
}
glEnd();
}
//fungsi yang menghasilkan efek gradasi jika digunakan untuk mewarnai pada lingkaran
void centerPolygon(point2D_t p[], point2D_t pc,color_t col,color_t colp, int n)
{
int i;
glBegin(GL_LINES);
for(i=0;i<n;i++)
{
setColor(colp);
glVertex2f(pc.x,pc.y);
setColor(col);
glVertex2f(p[i].x,p[i].y);
}
glEnd();
}
// Definisi dan pengolahan matrik
matrix2D_t createIdentity(void) {
matrix2D_t u;
int i,j;
for (i=0;i<3;i++) {
for(j=0;j<3;j++) u.m[i][j]=0.;
u.m[i][i]=1.;
}
return u;
}
// untuk membuat efek translasi(bergerak)
matrix2D_t translationMTX(float dx,float dy)
{
matrix2D_t trans=createIdentity();
trans.m[0][2]=dx;
trans.m[1][2]=dy;
return trans;
}
// untuk membuat efek scaling(berubah ukuran)
matrix2D_t scalingMTX(float mx,float my)
{
matrix2D_t scale=createIdentity();
scale.m[0][0]=mx;
scale.m[1][1]=my;
return scale;
}
// untuk membuat efek rotasi(berputar)
matrix2D_t rotationMTX(float theta)
{
matrix2D_t rotate=createIdentity();
float cs=cos(theta);
float sn=sin(theta);
rotate.m[0][0]=cs; rotate.m[0][1]=-sn;
rotate.m[1][0]=sn; rotate.m[1][1]=cs;
return rotate;
}
matrix2D_t operator * (matrix2D_t a, matrix2D_t b)
{
matrix2D_t c;//c=a*b
int i,j,k;
for (i=0;i<3;i++) for (j=0;j<3;j++) {
c.m[i][j]=0;
for (k=0;k<3;k++)
c.m[i][j]+=a.m[i][k]*b.m[k][j];
}
return c;
}
vector2D_t operator * (matrix2D_t a, vector2D_t b)
{
vector2D_t c;//c=a*b
int i,j;
for (i=0;i<3;i++) {
c.v[i]=0;
for (j=0;j<3;j++)
c.v[i]+=a.m[i][j]*b.v[j];
}
return c;
}
// fungsi untuk menampilkan tulisan
void bitmap_output(int x, int y, char *string, void *font,color_t color)
{
int len, i;
setColor(color);
glRasterPos2f(x, y);
len = (int) strlen(string);
for (i = 0; i < len; i++) {
glutBitmapCharacter(font, string[i]);
}
}
// fungsi yang digunakan untuk membuat ellipse
static void createEllipse(point2D_t p[],point2D_t p0,float r1,float r2)
{
for(int i=0;i<360;i++){
p[i].x=p0.x+r1*cos(i/57.3);
p[i].y=p0.y+r2*sin(i/57.3);
}
}
void userdraw()
{
//backgroud yang digunakan ada 2 yaitu untuk langit(wback) dan untuk tanahnya sendiri(wback2)
matrix2D_t mat;
vector2D_t vec;
static float tick,tick2,tick3,tick4,tick5,t1,t2,t3,t4,t5,temp=0;
int i;
color_t
putih={1,1,1},
hitam={0,0,0},
wback[4]={{0.6,0.8,1.0},{0.6,0.8,1.0},{0.6,0.8,1.0},{1.0,1.0,1.0}},
wback2[4]={{1.0,1.0,1.0},{0.772,0.772,0.670},{0.286,0.220,0.149},{0.772,0.772,0.670}};
point2D_t
backgroud[4]= {{-400,300},{400,300},{400,-180},{-400,-180}},
backgroud2[4]= {{-400,-181},{400,-181},{400,-250},{-400,-250}}
;
gradatePolygon(backgroud,4,wback);
gradatePolygon(backgroud2,4,wback2);
glColor3f(1,1,1);
////////////////////////////// AWAN ///////////////////////////////////////////////////////////////////
color_t wawan ={0.6,0.7,1.0};
point2D_t
//kumpulan awan 1
awan1[360],pawan1={100,100},
awan2[360],pawan2={110,110},
awan3[360],pawan3={90,115},
awan4[360],pawan4={85,90},
awan5[360],pawan5={115,102},
//kumpulan awan 2
awan6[360],pawan6={-220,110},
awan7[360],pawan7={-230,130},
awan8[360],pawan8={-210,135},
awan9[360],pawan9={-205,120},
awan10[360],pawan10={-235,122},
//kumpulan awan 3
awan11[360],pawan11={220,60},
awan12[360],pawan12={230,70},
awan13[360],pawan13={210,75},
awan14[360],pawan14={205,60},
awan15[360],pawan15={235,62};
//implementasi awan 2 dan 3
createEllipse(awan6,pawan6,50,4);
createEllipse(awan7,pawan7,30,14);
createEllipse(awan8,pawan8,90,15);
createEllipse(awan9,pawan9,10,5);
createEllipse(awan10,pawan10,40,10);
createEllipse(awan11,pawan11,100,5);
createEllipse(awan12,pawan12,20,14);
createEllipse(awan13,pawan13,50,15);
createEllipse(awan14,pawan14,10,5);
createEllipse(awan15,pawan15,40,10);
//implementasi untuk kumpulan awan 1 (terdapat efek translasi disini)
//awan1
createEllipse(awan1,pawan1,100,5);
mat=translationMTX(0+tick2,0+tick2);
for(i=0; i<360; i++)
{
vec=Point2Vector1(awan1[i]);
vec=mat*vec;
awan1[i]=Vector2Point1(vec);
}
//awan2
createEllipse(awan2,pawan2,50,10);
mat=translationMTX(0+tick2,0+tick2);
for(i=0; i<360; i++)
{
vec=Point2Vector1(awan2[i]);
vec=mat*vec;
awan2[i]=Vector2Point1(vec);
}
//awan3
createEllipse(awan3,pawan3,30,15);
mat=translationMTX(0+tick2,0+tick2);
for(i=0; i<360; i++)
{
vec=Point2Vector1(awan3[i]);
vec=mat*vec;
awan3[i]=Vector2Point1(vec);
}
//awan4
createEllipse(awan4,pawan4,40,5);
mat=translationMTX(0+tick2,0+tick2);
for(i=0; i<360; i++)
{
vec=Point2Vector1(awan4[i]);
vec=mat*vec;
awan4[i]=Vector2Point1(vec);
}
//awan5
createEllipse(awan5,pawan5,60,15);
mat=translationMTX(0+tick2,0+tick2);
for(i=0; i<360; i++)
{
vec=Point2Vector1(awan5[i]);
vec=mat*vec;
awan5[i]=Vector2Point1(vec);
}
// mewarnai awan
fillPolygon(awan1,360,wawan);
fillPolygon(awan2,360,wawan);
fillPolygon(awan3,360,wawan);
fillPolygon(awan4,360,wawan);
fillPolygon(awan5,360,wawan);
fillPolygon(awan6,360,wawan);
fillPolygon(awan7,360,wawan);
fillPolygon(awan8,360,wawan);
fillPolygon(awan9,360,wawan);
fillPolygon(awan10,360,wawan);
fillPolygon(awan11,360,wawan);
fillPolygon(awan12,360,wawan);
fillPolygon(awan13,360,wawan);
fillPolygon(awan14,360,wawan);
fillPolygon(awan15,360,wawan);
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////BANGUNAN PERTAMA(SAMPING PESAWAT)///////////////////////////////////
//inisialisasi warna
color_t
wvert[4]={{0.8,0.81,0.18},{0.8,0.81,0.18},{1.0,1.0,1.0},{1.0,1.0,1.0}},
whor[4]={{0.8,0.81,0.18},{0.8,0.81,0.18},{1.0,1.0,1.0},{1.0,1.0,1.0}},
wantena={0.901,0.911,0.915},wtam={1,1,1};
glColor3f(1,1,1);
//inisialisasi bangunan arah vertikal
point2D_t
antena[360],pantena={-75,63},
lamppun[4] = {{-48,120},{-48,125},{-52,125},{-52,120}},
puncak[4] = {{-48,120},{-48,65},{-52,65},{-52,120}},
kotak1 [4]= {{-45,65},{-45,45},{-55,45},{-55,65}},
miring1 [4]= {{-20,50},{-23,50},{-45,65},{-45,68}},
miring2 [4]= {{-70,50},{-73,50},{-55,68},{-55,65}},
miring3 [4]= {{-85,45},{-88,45},{-55,67},{-55,67}},
vert1 [4]= {{-20,50},{-20,-180},{-23,-180},{-23,50}},
vert2 [4]= {{-25,50},{-25,-180},{-28,-180},{-28,50}},
vert5 [4]= {{-34,50},{-34,-180},{-37,-180},{-37,50}},
vert6 [4]= {{-40,50},{-40,-180},{-43,-180},{-43,50}},
kotak2 [4]= {{-40,45},{-40,30},{-68,30},{-68,45}},
kotak3 [4]= {{-45,30},{-45,-180},{-63,-180},{-63,30}},
vert3 [4]= {{-70,50},{-70,-180},{-73,-180},{-73,50}},
vert4 [4]= {{-85,45},{-85,-182},{-88,-182},{-88,45}}
;
//warna bangunan arah vertikal
gradatePolygon(puncak,4,wvert); gradatePolygon(kotak1,4,wvert); gradatePolygon(miring1,4,wvert);
gradatePolygon(miring2,4,wvert); gradatePolygon(miring3,4,wvert); gradatePolygon(vert1,4,wvert);
gradatePolygon(vert2,4,wvert); gradatePolygon(vert5,4,wvert); gradatePolygon(vert6,4,wvert);
gradatePolygon(vert2,4,wvert); gradatePolygon(kotak2,4,wvert); gradatePolygon(kotak3,4,wvert);
gradatePolygon(vert3,4,wvert); gradatePolygon(vert4,4,wvert);
//inisialisasi bangunan arah horisontal
point2D_t
hor1 [4]= {{-20,49},{-20,46},{-72,46},{-72,49}},
hor1samping [4]= {{-72,48},{-72,46},{-85,43},{-85,45}},
hor2 [4]= {{-20,31},{-20,28},{-72,28},{-72,31}},
mrg1 [4]= {{-20,31},{-22,28},{-47,46},{-45,46}},
mrg2 [4]= {{-72,28},{-72,31},{-47,46},{-45,46}},
hor2samping [4]= {{-72,30},{-72,28},{-85,23},{-85,25}},
hor3 [4]= {{34,25},{29,15},{-75,15},{-75,25}}, //NB= dibuat translasi ke samping
hor4 [4]= {{-20,11},{-20,8},{-72,8},{-72,11}},
hor3samping [4]= {{-72,10},{-72,8},{-85,3},{-85,5}},
hor5 [4]= {{-20,-11},{-20,-8},{-72,-8},{-72,-11}},
mrg3 [4]= {{-20,-11},{-20,-8},{-47,11},{-47,8}},
mrg4 [4]= {{-72,-8},{-72,-11},{-47,8},{-47,11}},
hor4samping [4]= {{-72,-10},{-72,-8},{-85,-13},{-85,-15}},
hor6 [4]= {{-20,-25},{-20,-28},{-72,-28},{-72,-25}},
mrg5 [4]= {{-20,-25},{-20,-28},{-47,-10},{-47,-8}},
mrg6 [4]= {{-72,-27},{-72,-25},{-47,-8},{-47,-10}},
hor5samping [4]= {{-72,-25},{-72,-27},{-85,-32},{-85,-30}},
hor7 [4]= {{-20,-45},{-20,-48},{-72,-48},{-72,-45}},
hor6bay [4]= {{-5,-22},{-5,-33},{-120,-33},{-120,-22}},//NB: translasi ke samping sebelum hor3
mrg7 [4]= {{-20,-45},{-20,-47},{-47,-27},{-47,-25}},
mrg8 [4]= {{-72,-47},{-72,-45},{-47,-25},{-47,-27}},
hor6samping [4]= {{-72,-45},{-72,-47},{-85,-52},{-85,-50}},
hor8 [4]= {{-20,-65},{-20,-68},{-72,-68},{-72,-65}},
mrg9 [4]= {{-20,-65},{-20,-67},{-47,-47},{-47,-45}},
mrg10 [4]= {{-72,-67},{-72,-65},{-47,-45},{-47,-47}},
hor7samping [4]= {{-72,-65},{-72,-67},{-85,-72},{-85,-70}},
hor9 [4]= {{-20,-85},{-20,-88},{-72,-88},{-72,-85}},
mrg11 [4]= {{-20,-85},{-20,-87},{-47,-67},{-47,-65}},
mrg12 [4]= {{-72,-87},{-72,-85},{-47,-65},{-47,-67}},
hor8samping [4]= {{-72,-85},{-72,-87},{-85,-92},{-85,-90}},
hor10 [4]= {{-20,-105},{-20,-108},{-72,-108},{-72,-105}},
mrg13 [4]= {{-20,-105},{-20,-107},{-47,-87},{-47,-85}},
mrg14 [4]= {{-72,-107},{-72,-105},{-47,-85},{-47,-87}},
hor9samping [4]= {{-72,-105},{-72,-107},{-85,-112},{-85,-110}};
mat=translationMTX(0-tick5,0);
for(i=0; i<4; i++)
{
vec=Point2Vector1(hor3[i]);
vec=mat*vec;
hor3[i]=Vector2Point1(vec);
}
mat=translationMTX(0+t2,0);
for(i=0; i<4; i++)
{
vec=Point2Vector1(hor6bay[i]);
vec=mat*vec;
hor6bay[i]=Vector2Point1(vec);
}
mat=translationMTX(0-t4,0); // gerakan kembali ke posisi awal
for(i=0; i<4; i++)
{
vec=Point2Vector1(hor6bay[i]);
vec=mat*vec;
hor6bay[i]=Vector2Point1(vec);
}
//warna bangunan arah horisontal
gradatePolygon(hor1,4,whor); gradatePolygon(hor1samping,4,whor);
gradatePolygon(hor2,4,whor); gradatePolygon(mrg1,4,whor); gradatePolygon(mrg2,4,whor);
gradatePolygon(hor2samping,4,whor);
gradatePolygon(hor3,4,whor); gradatePolygon(hor4,4,whor); gradatePolygon(hor3samping,4,whor);
gradatePolygon(hor5,4,whor); gradatePolygon(mrg3,4,whor); gradatePolygon(mrg4,4,whor);
gradatePolygon(hor4samping,4,whor);
gradatePolygon(hor6,4,whor); gradatePolygon(mrg5,4,whor); gradatePolygon(mrg6,4,whor);
gradatePolygon(hor5samping,4,whor);
gradatePolygon(hor7,4,whor); gradatePolygon(mrg7,4,whor); gradatePolygon(mrg8,4,whor);
gradatePolygon(hor6samping,4,whor);
gradatePolygon(hor8,4,whor); gradatePolygon(mrg9,4,whor); gradatePolygon(mrg10,4,whor);
gradatePolygon(hor7samping,4,whor);
gradatePolygon(hor9,4,whor); gradatePolygon(mrg11,4,whor); gradatePolygon(mrg12,4,whor);
gradatePolygon(hor8samping,4,whor);
gradatePolygon(hor10,4,whor); gradatePolygon(mrg13,4,whor); gradatePolygon(mrg14,4,whor);
gradatePolygon(hor9samping,4,whor);gradatePolygon(hor6bay,4,whor);
//antena
createEllipse(antena,pantena,13,15);
fillPolygon(antena,360,wantena);
setColor(0.286,0.220,0.149);
drawLine(-80,63,-73,77);
drawLine(-80,63,-73,49);
drawLine(-80,64,-63,62);
drawLine(-80,64,-88,61);
drawDot(-81,63);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BANGUNAN KETIGA (SAMPING BANGUNAN PERTAMA) ////////////////////////////
//inisialisasi
color_t
wbacklift={0,0,0},
wlift={0.901,0.656,0.915};
point2D_t
kotakbag3 [4] ={{-100,0},{-100,-180},{-140,-180},{-140,0}},
kotakbag3a [4] ={{-147,-1},{-147,-180},{-170,-180},{-170,-1}},
lift [4] ={{-147,-170},{-147,-180},{-170,-180},{-170,-170}},//gerakan keatas
liftka [4] ={{-148,-172},{-148,-180},{-158,-180},{-158,-172}},
liftki [4] ={{-159,-172},{-159,-180},{-169,-180},{-169,-172}},
liftsamping [4]= {{-120,-23},{-120,-32},{-110,-32},{-110,-23}},
horbag3 [4] = {{-93,5},{-93,2},{-180,2},{-180,5}},
horbag3a [4] = {{-93,-25},{-93,-27},{-180,-27},{-180,-25}},
horbag3b [4] = {{-93,-65},{-93,-67},{-180,-67},{-180,-65}},
horbag3c [4] = {{-93,-105},{-93,-107},{-180,-107},{-180,-105}},
horbag3d [4] = {{-93,-145},{-93,-147},{-180,-147},{-180,-145}},
verbag3 [4] ={{-93,5},{-93,-180},{-97,-180},{-97,5}},
verbag3a [4] ={{-143,5},{-143,-180},{-146,-180},{-146,5}},
verbag3b [4] ={{-153,5},{-153,-180},{-156,-180},{-156,5}},
verbag3c [4] ={{-172,5},{-172,-180},{-175,-180},{-175,5}}
;
mat=translationMTX(0,0+t1);
for(i=0; i<4; i++)
{
vec=Point2Vector1(lift[i]);
vec=mat*vec;
lift[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+t1);
for(i=0; i<4; i++)
{
vec=Point2Vector1(liftka[i]);
vec=mat*vec;
liftka[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+t1);
for(i=0; i<4; i++)
{
vec=Point2Vector1(liftki[i]);
vec=mat*vec;
liftki[i]=Vector2Point1(vec);
}
mat=translationMTX(0+t2,0); // bergerak bersama dengan t2
for(i=0; i<4; i++)
{
vec=Point2Vector1(liftsamping[i]);
vec=mat*vec;
liftsamping[i]=Vector2Point1(vec);
}
mat=translationMTX(0-t4,0); // bergerak bersama dengan t4
for(i=0; i<4; i++)
{
vec=Point2Vector1(liftsamping[i]);
vec=mat*vec;
liftsamping[i]=Vector2Point1(vec);
}
mat=translationMTX(0+t3,0);
for(i=0; i<4; i++)
{
vec=Point2Vector1(liftsamping[i]);
vec=mat*vec;
liftsamping[i]=Vector2Point1(vec);
}
//implementasi
//lift
fillPolygon(kotakbag3a,4,wbacklift);
fillPolygon(lift,4,wbacklift);
fillPolygon(liftka,4,wlift);
fillPolygon(liftki,4,wlift);
fillPolygon(liftsamping,4,wbacklift);
gradatePolygon(kotakbag3,4,wvert); gradatePolygon(verbag3,4,wvert); gradatePolygon(verbag3a,4,wvert);
gradatePolygon(verbag3b,4,wvert); gradatePolygon(verbag3c,4,wvert); gradatePolygon(horbag3,4,wvert);
gradatePolygon(horbag3a,4,wvert); gradatePolygon(horbag3b,4,wvert); gradatePolygon(horbag3c,4,wvert);
gradatePolygon(horbag3d,4,wvert);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// PESAWAT ////////////////////////////////////////////////////
color_t
wroket={0.93,0.51,0.09},
wroket2={0.93,0.71,0.09},
wroketbaw [4]={{0.93,0.51,0.09},{1,1,1},{1,1,1},{0.8,0.81,0.18}},
wroketsam={0.73,0.41,0.09},whias={1,1,1},
wroketsambaw [4]={{0.73,0.41,0.09},{1,1,1},{1,1,1},{0.73,0.41,0.09}},
wpes={0.901,0.911,0.915},wpesbaw={0,0,0},wsyp={0.901,0.900,0.915},
fire [4] = {{0.93,0.51,0.09},{0.8,0.81,0.18},{0.8,0.81,0.18},{0.93,0.51,0.09}},
fire2 [4] = {{0.93,0.51,0.99},{0.8,0.81,0.68},{0.8,0.81,0.68},{0.93,0.51,0.99}};
point2D_t
//big roket
roket[360],proket={52,0},
p1 [4] = {{36,-107},{36,20},{68,20},{68,-107}},
p2 [4] = {{36,-107},{32,-127},{72,-127},{68,-107}},
p3 [4] = {{28,-140},{36,-127},{71,-127},{80,-140}},
// small roket
roketsam[360],proketsam={60,-18},
psam1 [4] = {{52,0},{68,0},{68,-107},{52,-107}},
psam2 [4] = {{68,-107},{72,-124},{48,-124},{52,-107}},
psam3 [4] = {{74,-134},{70,-125},{50,-125},{46,-134}},
// pesawat
pUtama [4] ={{15,-30},{15,-102},{36,-102},{36,-30}},
pEkor [4] = {{15,-77},{5,-110},{5,-120},{15,-98}},
pSayap [3] ={{35,-64},{45,-87},{35,-95}},
pes[360],ppes={30,-42},
pesbaw1[360],ppesbaw1={19,-104},
pesbaw2[360],ppesbaw2={31,-104},
//garis penghias
p1hias [4] = {{36,22},{36,20},{68,20},{68,22}},
p1hias2 [4] = {{36,-107},{36,-105},{68,-105},{68,-107}},
p1hias3 [4] = {{36,5},{36,-12},{68,-12},{68,5}},
psam1hias [4] = {{52,0},{68,0},{68,-2},{52,-2}},
psam1hias2 [4] = {{52,-105},{68,-105},{68,-107},{52,-107}},
pUtamahias [4] ={{20,-30},{20,-85},{22,-85},{22,-30}};
//implementasi
createEllipse(pes,ppes,15,35);
mat=translationMTX(0,0+tick4);
for(i=0; i<360; i++)
{
vec=Point2Vector1(pes[i]);
vec=mat*vec;
pes[i]=Vector2Point1(vec);
}
createEllipse(roket,proket,16,50);
mat=translationMTX(0,0+tick4);
for(i=0; i<360; i++)
{
vec=Point2Vector1(roket[i]);
vec=mat*vec;
roket[i]=Vector2Point1(vec);
}
createEllipse(roketsam,proketsam,8,35);
mat=translationMTX(0,0+tick4);
for(i=0; i<360; i++)
{
vec=Point2Vector1(roketsam[i]);
vec=mat*vec;
roketsam[i]=Vector2Point1(vec);
}
createEllipse(pesbaw1,ppesbaw1,5,2);
mat=translationMTX(0,0+tick4);
for(i=0; i<360; i++)
{
vec=Point2Vector1(pesbaw1[i]);
vec=mat*vec;
pesbaw1[i]=Vector2Point1(vec);
}
createEllipse(pesbaw2,ppesbaw2,5,2);
mat=translationMTX(0,0+tick4);
for(i=0; i<360; i++)
{
vec=Point2Vector1(pesbaw2[i]);
vec=mat*vec;
pesbaw2[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(p1[i]);
vec=mat*vec;
p1[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<3; i++)
{
vec=Point2Vector1(pSayap[i]);
vec=mat*vec;
pSayap[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(p2[i]);
vec=mat*vec;
p2[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(psam1[i]);
vec=mat*vec;
psam1[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(psam2[i]);
vec=mat*vec;
psam2[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(pUtama[i]);
vec=mat*vec;
pUtama[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(pEkor[i]);
vec=mat*vec;
pEkor[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=1; i<3; i++)
{
vec=Point2Vector1(p3[i]);
vec=mat*vec;
p3[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=1; i<3; i++)
{
vec=Point2Vector1(psam3[i]);
vec=mat*vec;
psam3[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(p1hias[i]);
vec=mat*vec;
p1hias[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(p1hias2[i]);
vec=mat*vec;
p1hias2[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(p1hias3[i]);
vec=mat*vec;
p1hias3[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(psam1hias[i]);
vec=mat*vec;
psam1hias[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(psam1hias2[i]);
vec=mat*vec;
psam1hias2[i]=Vector2Point1(vec);
}
mat=translationMTX(0,0+tick4);
for(i=0; i<4; i++)
{
vec=Point2Vector1(pUtamahias[i]);
vec=mat*vec;
pUtamahias[i]=Vector2Point1(vec);
}
// pewarnaan
fillPolygon(pes,360,wpes);
fillPolygon(roket,360,wroket);
fillPolygon(p1,4,wroket);
fillPolygon(p1hias,4,wpes);
fillPolygon(pSayap,3,wsyp);
gradatePolygon(p2,4,wroketbaw);
fillPolygon(p1hias2,4,wpes);
fillPolygon(p1hias3,4,wroket2);
fillPolygon(roketsam,360,wroketsam);
fillPolygon(psam1,4,wroketsam);
fillPolygon(psam1hias,4,wroket2);
gradatePolygon(psam2,4,wroketsambaw);
fillPolygon(psam1hias2,4,wroket2);
fillPolygon(pUtama,4,wpes);
fillPolygon(pEkor,4,wpes);
fillPolygon(pesbaw1,360,wpesbaw);
fillPolygon(pesbaw2,360,wpesbaw);
fillPolygon(pUtamahias,4,whias);
gradatePolygon(p3,4,fire);
gradatePolygon(psam3,4,fire2);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BANGUNAN KEDUA (BAWAH PESAWAT) //////////////////////////////////////////////
//inisialisasi
point2D_t
horbag2 [4] ={{110,-125},{110,-123},{-72,-123},{-72,-125}},
horbag2a [4] ={{110,-145},{110,-147},{-72,-147},{-72,-145}},
horbag2b [4] ={{110,-153},{110,-157},{-72,-157},{-72,-153}},
horbag2c [4] ={{110,-170},{110,-175},{-72,-175},{-72,-170}},
verbag2 [4] ={{-10,-123},{-10,-180},{-12,-180},{-12,-123}},
verbag2a [4] ={{108,-123},{108,-180},{110,-180},{110,-123}},
kotakbag2a [4] = {{20,-125},{20,-180},{-5,-180},{-5,-125}},
kotakbag2b [4] = {{80,-170},{80,-130},{22,-130},{22,-170}},
kotakbag2c [4] = {{102,-180},{102,-125},{82,-125},{82,-180}},
mrgbag2 [4] = {{110,-123},{100,-123},{70,-110},{70,-105}};
mat=translationMTX(0+tick5,0-tick5);
for(i=2; i<4; i++)
{
vec=Point2Vector1(mrgbag2[i]);
vec=mat*vec;
mrgbag2[i]=Vector2Point1(vec);
}
// implementasi
gradatePolygon(kotakbag2a,4,whor); gradatePolygon(kotakbag2b,4,whor); gradatePolygon(kotakbag2c,4,whor);
gradatePolygon(horbag2,4,whor); gradatePolygon(horbag2a,4,whor); gradatePolygon(horbag2b,4,whor);
gradatePolygon(horbag2c,4,whor); gradatePolygon(verbag2,4,whor); gradatePolygon(verbag2a,4,whor);
gradatePolygon(mrgbag2,4,whor);
////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BANGUNAN KEEMPAT (LANDASAN PESAWAT) /////////////////////////////////////
//warna landasan(dominan coklat)
color_t
wland = {0.772,0.772,0.670},
wland2 [4] ={{1.0,1.0,1.0},{0.772,0.772,0.670},{0.286,0.220,0.149},{0.772,0.772,0.670}},
wrum={0.033,0.585,0.052},bayangan={0.401,0.411,0.415},
wlamp={0.286,0.220,0.149};
point2D_t
land [4] ={{-400,-179},{400,-179},{400,-182},{-400,-182}},
land2 [4] ={{-400,-229},{400,-229},{400,-235},{-400,-235}},
mrgbag4 [4] ={{-300,-182},{-296,-182},{-266,-250},{-270,-250}},
mrgbag4a [4] ={{-100,-182},{-96,-182},{-52,-250},{-56,-250}},
mrgbag4b [4] ={{200,-182},{196,-182},{264,-250},{268,-250}},
//lampu
lamp [7]={{320,-180},{320,-150},{310,-140},{310,-139},{321,-150},{321,-180},{320,-180}},
lamp2[7]={{-320,-180},{-320,-150},{-330,-140},{-330,-141},{-321,-150},{-321,-180},{-320,-180}},
//bayangan
bay1[4]={{-173,-180},{-143,-180},{-113,-250},{-143,-250}},
bay2[4]={{-140,-180},{-101,-180},{-60,-250},{-110,-250}},
bay3[4]={{-96,-180},{-92,-180},{-44,-250},{-54,-250}},
bay4[4]={{-86,-180},{-82,-180},{-34,-250},{-44,-250}},
bay5[4]={{-72,-180},{-68,-180},{-16,-250},{-24,-250}},
bay6[4]={{-62,-180},{-43,-180},{6,-250},{-14,-250}},
bay7[4]={{-42,-180},{-40,-180},{10,-250},{7,-250}},
bay8[4]={{-35,-180},{-33,-180},{17,-250},{15,-250}},
bay9[4]={{-27,-180},{-25,-180},{26,-250},{24,-250}},
bay10[4]={{-23,-180},{-21,-180},{30,-250},{28,-250}},
bay11[4]={{-12,-180},{-10,-180},{40,-250},{38,-250}},
bay12[4]={{-6,-180},{20,-180},{74,-250},{44,-250}},
bay13[4]={{29,-190},{87,-190},{140,-250},{77,-250}},
bay14[4]={{81,-180},{100,-180},{162,-250},{142,-250}},
bay15[4]={{107,-180},{110,-180},{172,-250},{169,-250}},
bay16[4]={{-173,-200},{-82,-200},{-70,-202},{-171,-202}},
bay17[4]={{-62,-200},{128,-200},{130,-202},{-64,-202}},
bay18[4]={{-58,-210},{138,-210},{140,-212},{-54,-212}}
;
//rumput
point2D_t
rum1 [15]={{-400,-250},{-400,-230},{-390,-220},{-370,-215},{-360,-225},{-355,-223},{-350,-220},
{-340,-230},{-330,-225},{-320,-220},{-300,-225},{-290,-235},{-270,-215},{-260,-220},
{-260,-400}},
rum2 [15]={{-260,-400},{-260,-220},{-240,-225},{-220,-220},{-190,-222},{-180,-228},{-160,-232},
{-150,-235},{-140,-238},{-120,-235},{-100,-230},{-80,-228},{-60,-225},{-40,-225},
{-40,-400}},
rum3 [15]={{-40,-400},{-40,-225},{-20,-215},{0,-230},{20,-225},{40,-215},{50,-230},
{60,-220},{90,-210},{120,-215},{130,-220},{150,-225},{170,-230},{190,-228},{190,-400}},
rum4 [16]={{190,-400},{190,-228},{192,-225},{195,-223},{205,-220},{210,-215},{230,-218},
{250,-210},{270,-215},{290,-218},{310,-222},{320,-225},{340,-230},{350,-222},{380,-230},
{400,-250}};
gradatePolygon(land,4,wland2);
gradatePolygon(land2,4,wland2);
fillPolygon(mrgbag4,4,wland);
fillPolygon(mrgbag4a,4,wland);
fillPolygon(mrgbag4b,4,wland);
//pagar
setColor(0.286,0.220,0.149);
drawLine(-300,-180,-300,-164);
drawLine(-100,-180,-100,-164);
drawLine(100,-180,100,-164);
drawLine(300,-180,300,-164);
drawLine(-400,-170,400,-170);
drawLine(-400,-175,400,-175);
drawPolygon(lamp,7);
drawPolygon(lamp2,7);
//bayangan
setColor(0,0,0);
drawLine(-300,-180,-294,-194);
drawLine(-100,-180,-94,-194);
drawLine(100,-180,106,-194);
drawLine(300,-180,306,-194);
drawLine(-400,-190,400,-190);
drawLine(-400,-185,400,-185);
drawLine(320,-180,330,-200);
drawLine(330,-200,325,-210);
drawLine(-320,-180,-310,-200);
drawLine(-310,-200,-315,-210);
drawDot(310,-140);
drawDot(-330,-141);
fillPolygon(bay1,4,bayangan);
fillPolygon(bay2,4,bayangan);
fillPolygon(bay3,4,bayangan);
fillPolygon(bay4,4,bayangan);
fillPolygon(bay5,4,bayangan);
fillPolygon(bay6,4,bayangan);
fillPolygon(bay7,4,bayangan);
fillPolygon(bay8,4,bayangan);
fillPolygon(bay9,4,bayangan);
fillPolygon(bay10,4,bayangan);
fillPolygon(bay11,4,bayangan);
fillPolygon(bay12,4,bayangan);
fillPolygon(bay13,4,bayangan);
fillPolygon(bay14,4,bayangan);
fillPolygon(bay15,4,bayangan);
fillPolygon(bay16,4,bayangan);
fillPolygon(bay17,4,bayangan);
fillPolygon(bay18,4,bayangan);
//bagian rumput
setColor(0,1,0);
fillPolygon(rum1,15,wrum);
fillPolygon(rum2,15,wrum);
fillPolygon(rum3,15,wrum);
fillPolygon(rum4,16,wrum);
////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// ASAP ////////////////////////////////////////////////////////
color_t wasap={1,1,1},
wasap2={0,0,0};
point2D_t
asap[360],pasap={50,-131},
asap2[360],pasap2={70,-142},
asap3[360],pasap3={34,-150},
asap4[360],pasap4={23,-162},
asap5[360],pasap5={-15,-173},
asap6[360],pasap6={-70,-180},
asap7[360],pasap7={-120,-160},
asap11[360],pasap11={-150,-180},
asap8[360],pasap8={114,-160},
asap9[360],pasap9={120,-175},
asap10[360],pasap10={180,-185},
asap12[360],pasap12={250,-170},
asap13[360],pasap13={280,-175};
/////////asap tengah/////////////
if(tick5>31)
{
createEllipse(asap,pasap,27,4);
fillPolygon(asap,360,wasap);
}
if(tick5>32)
{
createEllipse(asap2,pasap2,60,10);
fillPolygon(asap2,360,wasap);
}
////////////////////////////////////
//////////////// asap kiri////////////
if(tick5>33)
{
createEllipse(asap3,pasap3,30,10);
fillPolygon(asap3,360,wasap);
}
if(tick5>34)
{
createEllipse(asap4,pasap4,50,10);
fillPolygon(asap4,360,wasap);
}
if(tick5>35)
{
createEllipse(asap5,pasap5,31,10);
fillPolygon(asap5,360,wasap);
}
if(tick5>36)
{
createEllipse(asap6,pasap6,40,15);
fillPolygon(asap6,360,wasap);
}
if(tick5>37)
{
createEllipse(asap7,pasap7,50,20);
fillPolygon(asap7,360,wasap);
}
if(tick5>38)
{
createEllipse(asap11,pasap11,50,20);
fillPolygon(asap11,360,wasap);
}
////////////////////////////////////
////////// asap kanan ///////////////
if(tick5>33)
{
createEllipse(asap8,pasap8,60,15);
fillPolygon(asap8,360,wasap);
}
if(tick5>34)
{
createEllipse(asap9,pasap9,110,10);
fillPolygon(asap9,360,wasap);
}
if(tick5>35)
{
createEllipse(asap10,pasap10,40,10);
fillPolygon(asap10,360,wasap);
}
if(tick5>36)
{
createEllipse(asap12,pasap12,40,10);
fillPolygon(asap12,360,wasap);
}
if(tick5>37)
{
createEllipse(asap13,pasap13,45,15);
fillPolygon(asap13,360,wasap);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// RUMPUT YANG BERGERAK //////////////////////////////////////////
color_t
wgras [3] ={{0.033,0.585,0.052},{0.233,0.685,0.252},{0.033,0.785,0.052}};
point2D_t
gras1 [3] ={{300,-250},{100,-130},{280,-250}},
gras2 [3] ={{305,-250},{270,-190},{285,-250}},
gras3 [3] ={{-300,-250},{-350,-150},{-280,-250}},
gras4 [3] ={{-305,-250},{-370,-190},{-285,-250}},
gras5 [3] ={{-295,-250},{-270,-200},{-285,-250}};
mat=translationMTX(0+tick,0+tick);
for(i=1; i<2; i++)
{
vec=Point2Vector1(gras1[i]);
vec=mat*vec;
gras1[i]=Vector2Point1(vec);
}
gradatePolygon(gras1,3,wgras);
mat=translationMTX(0+tick3,0+tick3);
for(i=1; i<2; i++)
{
vec=Point2Vector1(gras2[i]);
vec=mat*vec;
gras2[i]=Vector2Point1(vec);
}
gradatePolygon(gras2,3,wgras);
mat=translationMTX(0+tick3,0+tick3);
for(i=1; i<2; i++)
{
vec=Point2Vector1(gras3[i]);
vec=mat*vec;
gras3[i]=Vector2Point1(vec);
}
gradatePolygon(gras3,3,wgras);
mat=translationMTX(0+tick3,0+tick3);
for(i=1; i<2; i++)
{
vec=Point2Vector1(gras4[i]);
vec=mat*vec;
gras4[i]=Vector2Point1(vec);
}
gradatePolygon(gras4,3,wgras);
gradatePolygon(gras5,3,wgras);
////////////////////////////////////////////////////////////////////////////////////////////////////////
bitmap_output(-390,-240, "Fa",GLUT_BITMAP_TIMES_ROMAN_10,putih);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
tick+=0.5; //rumput panjang
if(tick==30)
tick=0;
tick2+=0.04; //awan
tick3+=0.5; //rumput kecil
if(tick3==10)
tick3=0;
if(tick5==40)
{
tick4+=0.5; //pesawat
}
if(t4==36)
{
tick5+=0.3; //pangkalan atas
if(tick5>40) tick5=40;
}
t1+=0.5; //lift
if(t1>170) t1=170;
if(t1==170)
{
t2+=0.09; //pangkalan bawah
if(t2>36) t2=36;
}
if(t2==36)
{
t3+=0.09; //lift kecil ke pesawat
if(t3>104) t3=104;
}
if(t3==104)
{
t4+=0.09; // lift balik
if(t4>36) t4=36;
}
}
void display(void)
{
glClear(GL_COLOR_BUFFER_BIT);
userdraw();
glutSwapBuffers();
}
int main (int argc, char ** argv)
{
// insert code here...
glutInit(&argc,argv); //inisialisasi toolkit
glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
glutInitWindowPosition(100,100);
glutInitWindowSize(800,600); // besar ukuran windows
glutCreateWindow("Grafika Komputer - Andi Kurniawan - 180100036");
glClearColor(0.6,0.8,1.0,0); // warna background biru muda/tua
gluOrtho2D(-400,400,-250,250); // ukuran layar yang dipake
glutIdleFunc(display);
glutDisplayFunc(display);
glutMainLoop();
return 0;
}
