//
//  Primitive.cpp
//  LAIG - P1
//

#include "Primitive.h"
#include "math.h"

    // DECLARACOES RELACIONADAS COM OS "EVALUATORS"
    //   atenção à ordem dos pontos que nao e' circular...
    // Coordenadas dos 4 pontos de controlo (podem ser entendidas como
    //   sendo as coordenadas dos cantos do polígono a visualizar):
    
    // control points of the plane
    GLdouble planeControlPoints[4][3] = {
        {0.5, 0.0, 0.5},
        {-0.5, 0.0, 0.5},
        
        {0.5, 0.0, -0.5},
        {-0.5, 0.0, -0.5}
    };
    
    // normals for each control point of the plane
    GLfloat planeNormals[4][3] = {
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 0.0} };
    
    // colors for each control point of the plane
    GLfloat planeColors[4][4] = {	{ 0.0, 0.2, 0.2, 0},
        { 0.0, 0.0, 0.2, 0},
        { 0.0, 0.2, 0.0, 0},
        { 0.2, 0.0, 0.0, 0} };
    
    // colors for each control point of the plane -> gray
    GLfloat planeGrayColors[4][4] = {	{ 0.2, 0.2, 0.2, 0},
        { 0.1, 0.1, 0.1, 0},
        { 0.2, 0.2, 0.2, 0},
        { 0.3, 0.3, 0.3, 0} };

    Primitive::Primitive() {
        
    }
    
    Primitive::Primitive(string id) {
        this->id = id;
    }
    
    Primitive::~Primitive() {
        
    }
    
    void Primitive::setId(string id) {
        this->id = id;
    }
    
    void Primitive::setTexlength_s(float texlength_s) {
        this->texlength_s = texlength_s;
    }
    
    void Primitive::setTexlength_t(float texlength_t) {
        this->texlength_t = texlength_t;
    }
    
    string Primitive::getId() {
        return this->id;
    }
    
    float Primitive::getTexlength_s() {
        return this->texlength_s;
    }
    
    float Primitive::getTexlength_t() {
        return this->texlength_t;
    }
    
    Cylinder::~Cylinder() {
        gluDeleteQuadric(cylinderQuadric);
    }
    
    Rectangle::Rectangle(string id, float x1, float y1, float x2, float y2) : Primitive(id) {
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
        this->verticesRectangle = new float*[4];
        this->verticesRectangle[0] = new float[3];
        this->verticesRectangle[1] = new float[3];
        this->verticesRectangle[2] = new float[3];
        this->verticesRectangle[3] = new float[3];
        this->verticesRectangle[0][0] = x1;
        this->verticesRectangle[0][1] = y1;
        this->verticesRectangle[0][2] = 0;
        this->verticesRectangle[1][0] = x2;
        this->verticesRectangle[1][1] = y1;
        this->verticesRectangle[1][2] = 0;
        this->verticesRectangle[2][0] = x2;
        this->verticesRectangle[2][1] = y2;
        this->verticesRectangle[2][2] = 0;
        this->verticesRectangle[3][0] = x1;
        this->verticesRectangle[3][1] = y2;
        this->verticesRectangle[3][2] = 0;
        this->normalRectangle = get_normal_newell(verticesRectangle, 4);
        
        this->length = fabs(x2-x1);
        this->height = fabs(y2-y1);
        
    }
    
    Triangle::Triangle(string id, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) : Primitive(id) {
        this->x1 = x1;
        this->x2 = x2;
        this->x3 = x3;
        this->y1 = y1;
        this->y2 = y2;
        this->y3 = y3;
        this->z1 = z1;
        this->z2 = z2;
        this->z3 = z3;
        
        // calculate triangle normal
        this->verticesTriangle = new float*[3];
        this->verticesTriangle[0] = new float[3];
        this->verticesTriangle[1] = new float[3];
        this->verticesTriangle[2] = new float[3];
        this->verticesTriangle[0][0] = x1;
        this->verticesTriangle[0][1] = y1;
        this->verticesTriangle[0][2] = z1;
        this->verticesTriangle[1][0] = x2;
        this->verticesTriangle[1][1] = y2;
        this->verticesTriangle[1][2] = z2;
        this->verticesTriangle[2][0] = x3;
        this->verticesTriangle[2][1] = y3;
        this->verticesTriangle[2][2] = z3;
        
        this->normalTriangle = get_normal_newell(verticesTriangle, 3);
        
        // calculate triangle texture
        this->a = sqrt(pow(x1-x3,2) + pow(y1-y3,2) + pow(z1-z3,2));
        this->b = sqrt(pow(x2-x1,2) + pow(y2-y1,2) + pow(z2-z1,2));
        this->c = sqrt(pow(x3-x2,2) + pow(y3-y2,2) + pow(z3-z2,2));
        
        this->cos_beta = (pow(this->a,2) - pow(b,2) + pow(c,2))/(2*a*c);
        
        //sin_beta = sqrt(1 - pow(cos_beta, 2));
        sin_beta = acos(cos_beta);
        
        this->p0[0] = c - (a * cos_beta);
        this->p0[1] = a * sin_beta;
        
        this->p1[0] = 0;
        this->p1[1] = 0;
        
        this->p2[0] = c;
        this->p2[1] = 0;
        
    }
    
    Cylinder::Cylinder(string id, float base, float top, float height, int slices, int stacks) : Primitive(id) {
        this->base = base;
        this->top = top;
        this->height = height;
        this->slices = slices;
        this->stacks = stacks;
        
        this->cylinderQuadric = gluNewQuadric();
        gluQuadricDrawStyle(cylinderQuadric, GLU_FILL);
        gluQuadricNormals(cylinderQuadric, GLU_SMOOTH);
        gluQuadricOrientation(cylinderQuadric, GLU_OUTSIDE);
        gluQuadricTexture(cylinderQuadric, GL_TRUE);
    }
    
    Sphere::Sphere(string id, float radius, int slices, int stacks) : Primitive(id) {
        this->radius = radius;
        this->slices = slices;
        this->stacks = stacks;
        
        this->sphereQuadric = gluNewQuadric();
        gluQuadricDrawStyle(sphereQuadric, GLU_FILL);
        gluQuadricNormals(sphereQuadric, GLU_SMOOTH);
        gluQuadricOrientation(sphereQuadric, GLU_OUTSIDE);
        gluQuadricTexture(sphereQuadric, GL_TRUE);
    }
    
    Torus::Torus(string id, float inner, float outer, int slices, int loops) : Primitive(id) {
        this->inner = inner;
        this->outer = outer;
        this->slices = slices;
        this->loops = loops;
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    }
    
    Plane::Plane(string id, int parts) : Primitive(id)
    {
        this->parts = parts;
        /** texture points */
        this->textPoints[1][0] = 0;
        this->textPoints[1][1] = 0;
        this->textPoints[0][0] = 0;
        this->textPoints[0][1] = 1;
        this->textPoints[3][0] = 1;
        this->textPoints[3][1] = 0;
        this->textPoints[2][0] = 1;
        this->textPoints[2][1] = 1;
    }
    
    Patch::Patch(string id, int order, int partsU, int partsV, string compute, vector<vector<float>> controlPoints) : Primitive(id)
    {
        this->order = order;
        this->partsU = partsU;
        this->partsV = partsV;
        this->compute = compute;
        setControlPoints(controlPoints);
        
        if(order == 1)
        {
            /** control points when the order is 1 */
            for (int i = 0; i < this->controlPoints.size(); i++)
            {
                vector<float> controlPoint = this->controlPoints[i];
                
                for (int k = 0; k < controlPoint.size(); k++)
                {
                    this->vecControlPointsOrder1[i][k] = controlPoint[k];
                }
            }
        }
        else
            if(order == 2)
            {
                /** control points when the order is 1 */
                for (int i = 0; i < this->controlPoints.size(); i++)
                {
                    vector<float> controlPoint = this->controlPoints[i];
                    
                    for (int k = 0; k < controlPoint.size(); k++)
                    {
                        this->vecControlPointsOrder2[i][k] = controlPoint[k];
                    }
                }
            }
            else
                if(order == 3)
                {
                    /** control points when the order is 1 */
                    for (int i = 0; i < this->controlPoints.size(); i++)
                    {
                        vector<float> controlPoint = this->controlPoints[i];
                        
                        for (int k = 0; k < controlPoint.size(); k++)
                        {
                            this->vecControlPointsOrder3[i][k] = controlPoint[k];
                        }
                    }
                }

        /** texture points */
        this->textPoints[1][0] = 0;
        this->textPoints[1][1] = 0;
        this->textPoints[0][0] = 0;
        this->textPoints[0][1] = 1;
        this->textPoints[3][0] = 1;
        this->textPoints[3][1] = 0;
        this->textPoints[2][0] = 1;
        this->textPoints[2][1] = 1;
        
    }
    
    void Patch::setControlPoints(vector<vector<float>> controlPoints)
    {
        for(int i = 0; i < controlPoints.size(); i++)
        {
            this->controlPoints.push_back(controlPoints[i]);
        }
        
    }
    
    vector<vector<float>> Patch::getControlPoints()
    {
        return this->controlPoints;
    }
    
    void Rectangle::draw() {
        
        glBegin(GL_QUADS);
        
        glNormal3f(normalRectangle[0], normalRectangle[1], normalRectangle[2]);
        
        glTexCoord2f(0, 0);
        glVertex3f(x1, y1, 0);
        glTexCoord2f(length/getTexlength_s(), 0);
        glVertex3f(x2, y1, 0);
        glTexCoord2f(length/getTexlength_s(), height/getTexlength_t());
        glVertex3f(x2, y2, 0);
        glTexCoord2f(0, height/getTexlength_t());
        glVertex3f(x1, y2, 0);
        
        glEnd();
    }
    
    void Triangle::draw() {
        
        glBegin(GL_TRIANGLES);
        
        glNormal3f(normalTriangle[0], normalTriangle[1], normalTriangle[2]);
        
        glTexCoord2f(p0[0]/getTexlength_s(), p0[1]/getTexlength_t());
        glVertex3f(x1, y1, z1);
        glTexCoord2f(p1[0], p1[1]);
        glVertex3f(x2, y2, z2);
        glTexCoord2f(p2[0]/getTexlength_s(), p2[1]);
        glVertex3f(x3, y3, z3);
        
        glEnd();
        
    }
    
    void Cylinder::draw() {
        glEnable(GL_NORMALIZE);
        
        // top of the cylinder
        glPushMatrix();
        
        gluDisk(cylinderQuadric, 0, base, slices, stacks);
        glTranslated(0, 0, height);
        gluDisk(cylinderQuadric, 0, top, slices, stacks);
        
        glPopMatrix();
        
        // side of the cylinder
        gluCylinder(cylinderQuadric, base, top, height, slices, stacks);
        
        // base of the cylinder
        glPushMatrix();
        glRotated(180, 0, 1, 0);
        gluDisk(cylinderQuadric, 0, base, slices, stacks);
        glPopMatrix();
    }
    
    void Sphere::draw() {
        gluSphere(sphereQuadric, radius, slices, stacks);
    }
    
    void Torus::draw() {
        
        glEnable(GL_NORMALIZE);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        
        glutSolidTorus(inner, outer, slices, loops);
        
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        
    }
    
    void Plane::draw() {
    
        
        // permissao de atribuicao directa de cores
        // para objectos que nao tem material atribuido, como
        // e' o caso dos eixos e da esfera que simboliza a fonte de luz...
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        //glEnable(GL_COLOR_MATERIAL);
        
        // INICIALIZACOES RELACIONADAS COM OS "EVALUATORS"
        
        // declaram-se quatro interpoladores, de coordenadas, de
        //     normais, de cores e de texturas:
        // o parâmetro de controlo dos interpoladors varia de 0 a 1,
        //     tanto em U como em V
        // os strides (ordem de visita no array de dados final) são:
        //     3 e 6 para o interpol. de coord. (respectivamente U e V)
        //     3 e 6 para o interpol. de normais (respectivamente U e V)
        //     4 e 8 para o interpolador de cores (respectivamente U e V)
        //     2 e 4 para o interpolador de texturas (respectivamente U e V)
        // a interpolação é linear (de grau 1) pelo que se coloca o
        //     valor "2" (grau + 1) nos quatro casos
        
        glColor3f(1.0,1.0,1.0);
        
                glMap2d(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2, 0.0, 1.0, 3*2, 2, &planeControlPoints[0][0]);

        
        glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 3*2, 2,  &planeNormals[0][0]);
        glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 4*2, 2,  &planeColors[0][0]);
        glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2, 0.0, 1.0, 2*2, 2,  &textPoints[0][0]);
        
        
        // When we actually go to evaluate and render the evaluator map as a 2D mesh, we need to indicate the number of partitions of U and V (number of grid rows and columns) and over what region of the parametric U and V domain we will iterate across. So we tell OpenGL to iterate across the full 0.0 to 1.0 range setup above with 5 rows and 6 columns. This is done with glMapGrid2f:
        glMapGrid2d(parts, 0.0, 1.0,parts, 0.0, 1.0);
        
        // os interpoladores activam-se:
        glEnable(GL_MAP2_VERTEX_3);
        glEnable(GL_MAP2_NORMAL);
        glEnable(GL_MAP2_COLOR_4);
        glEnable(GL_MAP2_TEXTURE_COORD_2);
        glEnable(GL_TEXTURE_2D);
        
        //glBindTexture(GL_TEXTURE_2D, 2);
        
        //After this setup is performed, a single OpenGL command evaluates and renders the specified grid as an evaluator mesh:
        glEvalMesh2(GL_FILL, 0.0, parts, 0.0, parts);
        
        // so' para referencia visual... onde estao os quatro pontos
        // de controlo:
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0, 1.0, 0.0);
        for (int i = 0; i < 4; i++)
        {
            glRasterPos3f(planeControlPoints[i][0],planeControlPoints[i][1],planeControlPoints[i][2]);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i);
        }
        
        glDisable(GL_MAP2_VERTEX_3);
        glDisable(GL_MAP2_NORMAL);
        glDisable(GL_MAP2_TEXTURE_COORD_2);
        glDisable(GL_TEXTURE_2D);
        
        //glDisable(GL_COLOR_MATERIAL);
    }

    void Patch::draw() {
        
        // permissao de atribuicao directa de cores
        // para objectos que nao tem material atribuido, como
        // e' o caso dos eixos e da esfera que simboliza a fonte de luz...
        //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        
        // INICIALIZACOES RELACIONADAS COM OS "EVALUATORS"
        
        // declaram-se quatro interpoladores, de coordenadas, de
        //     normais, de cores e de texturas:
        // o parâmetro de controlo dos interpoladors varia de 0 a 1,
        //     tanto em U como em V
        // os strides (ordem de visita no array de dados final) são:
        //     3 e 6 para o interpol. de coord. (respectivamente U e V)
        //     3 e 6 para o interpol. de normais (respectivamente U e V)
        //     4 e 8 para o interpolador de cores (respectivamente U e V)
        //     2 e 4 para o interpolador de texturas (respectivamente U e V)
        // a interpolação é linear (de grau 1) pelo que se coloca o
        //     valor "2" (grau + 1) nos quatro casos
        
        glColor3f(1.0,1.0,1.0);
        
        if(order == 1)
        {
            glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, this->order, 0.0, 1.0, 3*(this->order), this->order, &(this->vecControlPointsOrder1[0][0]));
           
            // so' para referencia visual... onde estao os quatro pontos
            // de controlo:
            glDisable(GL_TEXTURE_2D);
            glColor3f(1.0, 1.0, 0.0);
            for (int i = 0; i < 4; i++)
            {
                glRasterPos3f(this->vecControlPointsOrder1[i][0],this->vecControlPointsOrder1[i][1],this->vecControlPointsOrder1[i][2]);
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i);
            }
        }
        else
            if(order == 2)
            {
                glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, this->order, 0.0, 1.0, 3*(this->order), this->order, &(this->vecControlPointsOrder2[0][0]));
                
                // so' para referencia visual... onde estao os quatro pontos
                // de controlo:
                glDisable(GL_TEXTURE_2D);
                glColor3f(1.0, 1.0, 0.0);
                for (int i = 0; i < 4; i++)
                {
                    glRasterPos3f(this->vecControlPointsOrder2[i][0],this->vecControlPointsOrder2[i][1],this->vecControlPointsOrder2[i][2]);
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i);
                }

            }
            else
                if(order == 3)
                {
                    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, this->order, 0.0, 1.0, 3*(this->order), this->order, &(this->vecControlPointsOrder3[0][0]));

                    // so' para referencia visual... onde estao os quatro pontos
                    // de controlo:
                    glDisable(GL_TEXTURE_2D);
                    glColor3f(1.0, 1.0, 0.0);
                    for (int i = 0; i < 4; i++)
                    {
                        glRasterPos3f(this->vecControlPointsOrder3[i][0],this->vecControlPointsOrder3[i][1],this->vecControlPointsOrder3[i][2]);
                        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i);
                    }
                }
        
        glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, this->order,  0.0, 1.0, 3*(this->order), this->order,  &planeNormals[0][0]);
        glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, this->order,  0.0, 1.0, 4*(this->order), this->order,  &planeColors[0][0]);
        glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, this->order, 0.0, 1.0, 2*(this->order), this->order,  &textPoints[0][0]);
        
        
        // When we actually go to evaluate and render the evaluator map as a 2D mesh, we need to indicate the number of partitions of U and V (number of grid rows and columns) and over what region of the parametric U and V domain we will iterate across. So we tell OpenGL to iterate across the full 0.0 to 1.0 range setup above with 5 rows and 6 columns. This is done with glMapGrid2f:
        glMapGrid2d(this->partsU, 0.0, 1.0,this->partsV, 0.0, 1.0);
        
        // os interpoladores activam-se:
        glEnable(GL_MAP2_VERTEX_3);
        glEnable(GL_MAP2_NORMAL);
        glEnable(GL_MAP2_COLOR_4);
        glEnable(GL_MAP2_TEXTURE_COORD_2);
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, 4);
        
        //After this setup is performed, a single OpenGL command evaluates and renders the specified grid as an evaluator mesh:
        if(this->compute == "point")
        {
            glEvalMesh2(GL_POINT, 0.0, this->partsU, 0.0, this->partsV);
        }
        else
            if(this->compute == "line")
            {
                glEvalMesh2(GL_LINE, 0.0, this->partsU, 0.0, this->partsV);
            }
            else if(this->compute == "fill")
            {
                glEvalMesh2(GL_FILL, 0.0, this->partsU, 0.0, this->partsV);
            }
        
        glDisable(GL_MAP2_VERTEX_3);
        glDisable(GL_MAP2_NORMAL);
        glDisable(GL_MAP2_TEXTURE_COORD_2);
        glDisable(GL_TEXTURE_2D);
        
        glDisable(GL_COLOR_MATERIAL);
        
    }
    
    Plane::~Plane()
    {
    }
    
    float *Primitive::get_normal_newell(float **vertices, int size) {
        float *normal = (float*) malloc(sizeof (float)*3);
        float *vertex_act, *vertex_next;
        
        for (int vert = 0; vert < size; ++vert) {
            vertex_act = vertices[vert];
            vertex_next = vertices[(vert + 1) % size];
            /*
             Set Normal.x to Sum of Normal.x and (multiply (Current.y minus Next.y) by (Current.z plus Next.z))
             Set Normal.y to Sum of Normal.y and (multiply (Current.z minus Next.z) by (Current.x plus Next.x))
             Set Normal.z to Sum of Normal.z and (multiply (Current.x minus Next.x) by (Current.y plus Next.y))
             */
            normal[0] = normal[0]+(vertex_act[1] - vertex_next[1])*(vertex_act[2] + vertex_next[2]);
            normal[1] = normal[1]+(vertex_act[2] - vertex_next[2])*(vertex_act[0] + vertex_next[0]);
            normal[2] = normal[2]+(vertex_act[0] - vertex_next[0])*(vertex_act[1] + vertex_next[1]);
            
            // Normalize (divide by root of dot product)
            float tmp = (float) sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
            normal[0] /= tmp;
            normal[1] /= tmp;
            normal[2] /= tmp;
        }
        return normal;
    }
    

