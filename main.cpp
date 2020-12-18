#include <windows.h>
#include <math.h>
#include <list>
#include <GL\glut.h>


//глобальные переменные

static float segments_count = 50; //кол-во сегментов (число разбиений сферы)
static GLint width = 600; //ширина экрана
static GLint height = 600; //высота экрана

//классы

class ring_t
{
public:
    float r, g, b; //цвет
    float inner_radius, outer_radius; //внутренний и внешний радиусы
    float ax; //углы поворота кольца относительно осей z, x (исходная позиция - в плоскости xy) [град]
    ring_t(float r, float g, float b, float inner_radius, float outer_radius, float ax):
        r(r), g(g), b(b), inner_radius(inner_radius), outer_radius(outer_radius), ax(ax)
    {}

    void render(void)
    {
        glPushMatrix();
        glRotatef(ax, 1, 0, 0);
        GLUquadricObj *q = gluNewQuadric();
        gluQuadricDrawStyle(q, GLU_FILL);
        glColor3f(r, g, b);
        gluDisk(q, inner_radius, outer_radius, segments_count, segments_count);
        gluDeleteQuadric(q);
        glPopMatrix();
    }
};

class planet_t;
static planet_t *core;

class planet_t
{
public:
    float r, g, b; //цвет
    float planet_radius; //радиус планеты
    float orbit_radius; //радиус орбиты (окружность)
    float ax; //углы поворота орбиты относительно осей z, x (исходная позиция - в плоскости xy) [град]

    std::list<ring_t> rings; //кольца
    std::list<planet_t> pla; //планета

    planet_t(float r, float g, float b, float planet_radius, float orbit_radius, float ax):
        r(r), g(g), b(b), planet_radius(planet_radius), orbit_radius(orbit_radius), ax(ax)
    {}

    void render(const float t)
    {
        glRotatef(ax, 1, 0, 0);
        render_planet();
        render_rings(t);
    }

    void render_planet(void)
    {
        GLUquadricObj *q = gluNewQuadric();
        gluQuadricDrawStyle(q, GLU_FILL);
        glColor3f(r, g, b);
        gluSphere(q, planet_radius, segments_count, segments_count);
        gluDeleteQuadric(q);
    }

    void render_rings(const float t)
    {
        for
        (
            std::list<ring_t>::iterator i = rings.begin();
            i != rings.end();
            i++
        )
            i->render();

        for
        (
            std::list<planet_t>::iterator i = pla.begin();
            i != pla.end();
            i++
        )
            i->render(t);
    }
};

//функции отображения

static void Display(void)
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    core->render(1.0);
    glFlush();
    glutSwapBuffers();
}

static void Reshape(GLint width, GLint height)
{
    ::width = width;
    ::height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3, 3, -3, 3, -100, 100);
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

using namespace std;

int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("Saturn");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    planet_t core(1, 1, 1, 1, -1, 0);
    ::core = &core;

    core.pla.push_back(planet_t(0, 1, 0, 0.2, 3, 0));
    core.pla.back().rings.push_back(ring_t(1, 1, 1, 2.7, 2.8, 80));
    core.pla.back().rings.push_back(ring_t(1, 1, 1, 2.85, 2.9, 80));
    core.pla.back().rings.push_back(ring_t(1, 1, 1, 2.95, 3, 80));

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMainLoop();
    return 0;
}
