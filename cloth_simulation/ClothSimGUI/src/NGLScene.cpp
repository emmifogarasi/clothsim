//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.cpp
/// @brief contains all NGL drawing elements,
/// allows for viusalisation of landscapes through generations
//----------------------------------------------------------------------------------------------------------------------

#include "NGLScene.h"
#include "Cloth.h"
#include "particle.h"
#include "Constraint.h"
#include <ngl/NGLInit.h>
#include <QDebug>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>
#include <iostream>
#include <QMouseEvent>



NGLScene::NGLScene(QWidget *_parent) : QOpenGLWidget(_parent)
{
    timer.start(100, this);
}


constexpr auto shaderProgram = "PBR";

void NGLScene::initializeGL()
{
  //calling this first before any other GL commands to load and link the
  //GL commands from the lib, if not, the program will crash
  ngl::NGLInit::initialize();

  startTimer(0);

  cloth1 = Cloth(1.0,1.0,cloth_n,cloth_m); // one Cloth object of the Cloth class
  ball_pos = (0,0,0); // the center of our one ball
  float ball_radius; // the radius of our one ball

  getIndices();

  ngl::Vec3 from(0.0f,2.0f,2.0f);
  ngl::Vec3 to(0.0f,-2.0f,0.0f);
  ngl::Vec3 up(0.0f,2.0f,-2.0f);
  m_view=ngl::lookAt(from,to,up);
  m_projection=ngl::perspective(45.0f,static_cast<float>(width())/height(),0.01f,100.0f);

  // createVAO for grid and Sphere
  ngl::VAOPrimitives::createLineGrid("grid",20,20,20);
  ngl::VAOPrimitives::createSphere("sphere",0.25,50);

  ngl::ShaderLib::use("nglDiffuseShader");
  ngl::ShaderLib::setUniform("Colour",0.6f,0.1f,0.7f,1.0f);
  ngl::ShaderLib::setUniform("lightPos",0.0f,1.0f,1.0f);
  ngl::ShaderLib::setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

}


void NGLScene::resizeGL( int _width, int _height )
{

  //m_project=ngl::perspective(45.0f, static_cast<float>( _width ) / _height, 0.05f, 350.0f );
   m_win.width  = static_cast<int>( _width * devicePixelRatio() );
   m_win.height = static_cast<int>( _height * devicePixelRatio() );

}



void NGLScene::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId())
    {
        update();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}


void NGLScene::paintGL()
{

    if(isBlue==true)
    {
        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
    }

    if(isBlack==true)
    {
         glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    if(isOrange==true)
    {
         glClearColor(1.0f, 0.8f, 0.69f, 1.0f);
    }

    glEnable( GL_DEPTH_TEST );
    //enable multisampling for smoother drawing
    glEnable( GL_MULTISAMPLE );
    glViewport( 0, 0, m_win.width, m_win.height );
    int s = cloth1.constraints.size();
    qDebug()<<"CONSTRAINT SIZE: "<<s<<'\n';

    int size_vertexData = vertexData.size();
    int size_indices = indices.size();

    ngl::Mat4 rotX;
    ngl::Mat4 rotY;
    // create the rotation matrices
    rotX.rotateX( m_win.spinXFace );
    rotY.rotateY( m_win.spinYFace );
    // multiply the rotations
    m_mouseGlobalTX = rotX * rotY;
    // add the translations
    m_mouseGlobalTX.m_m[ 3 ][ 0 ] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[ 3 ][ 1 ] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[ 3 ][ 2 ] = m_modelPos.m_z;
    ngl::Mat4 tx;
    tx.translate(0.0f,-0.45f,0.0f);
    ngl::Mat4 MVP=m_projection*m_view*m_mouseGlobalTX*tx;
    ngl::Mat3 normalMatrix=m_view*m_mouseGlobalTX;
    normalMatrix.inverse().transpose();
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::ShaderLib::setUniform("normalMatrix",normalMatrix);

    draw();

    GLuint VertexB = 0;
    glGenBuffers(1, &VertexB);
    glBindBuffer(GL_ARRAY_BUFFER, VertexB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cloth::Vertex)*size_vertexData, &vertexData[0].x, GL_DYNAMIC_DRAW);

    GLuint IndexB = 0;
    glGenBuffers(1, &IndexB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*size_indices, &(indices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VertexB);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Cloth::Vertex), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Cloth::Vertex), BUFFER_OFFSET(12));
   // glEnableVertexAttribArray(2);
   // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Cloth::Vertex), BUFFER_OFFSET(24));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexB);

    glPointSize(4);

    glLineWidth(3);


    if(isPatterned==true)
    {
        if(isPointy==false)
        {
            isFilled = false;
            glDrawElements(GL_TRIANGLES, size_indices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        }

    }

    if(isPointy==true)
    {
        if(isPatterned==false)
        {
            isFilled = false;
             glDrawElements(GL_POINTS, size_indices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        }
    }

    if(isFilled==true)
    {
         glDrawElements(GL_TRIANGLE_STRIP, size_indices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    }


    if(isGrid==true)
    {
        ngl::VAOPrimitives::draw("grid");
    }
    else
    {

    }


    if(isBall==true)
    {
         ngl::VAOPrimitives::draw("sphere");
    }
    else
    {

    }

}


void NGLScene::getIndices()
{
    for (int j = 0; j < cloth_m-1; j++)
    {
        int index;
        if (j > 0)
        {
            indices.push_back(j * cloth_n);
        }
        for (int i = 0; i <= cloth_n-1; i++)
        {
            index = j * cloth_n + i;
            indices.push_back(index);
            indices.push_back(index + cloth_n);
        }
        if (j + 1 <= cloth_m-1)
        {
            indices.push_back(index + cloth_n);
        }
    }

}


void NGLScene::draw()
{
    cloth1.addForce(ngl::Vec3(0, -1*gravity,0)*TIME_STEPSIZE2);
    cloth1.windForce(ngl::Vec3(windX,0,windZ)*TIME_STEPSIZE2);
    cloth1.timeStep();
    cloth1.getVertexData(vertexData);
    if(isBall==true)
    {
         cloth1.ballCollision((ball_pos),(ball_radius));
    }
    else
    {

    }

}


void NGLScene::keyPressEvent( QKeyEvent* _event )
{
  //this method is called every time the main window recives a key event.
  //we then switch on the key value and set the camera in the GLWindow
  switch ( _event->key() )
  {
  //escape key to quit
  case Qt::Key_Escape:
      QGuiApplication::exit( EXIT_SUCCESS );
      break;
  default:
      break;
  }
  //update the GLWindow and re-draw
  update();

}


//graphic user interface driven functions

void NGLScene::setPatternTrue()
{
    isPatterned = true;
    isPointy = false;
    isFilled = false;
    qDebug()<<"TRUE"<<isPatterned<<'\n';

}

void NGLScene::setPatternFalse()
{
    isPatterned = false;
    isPointy = false;
    isFilled = true;


}

void NGLScene::setFilled()
{
    isPatterned = false;
    isPointy = false;
    isFilled = true;

}
void NGLScene::setPoints()
{
    isPointy = true;
    isPatterned = false;
    isFilled = false;
}


void NGLScene::setGravitySlider(int g)
{
 gravity = g/100.0f;

 update();

}

void NGLScene::setWindXSlider(int wx)
{
 windX = wx/100.0f;

 update();

}

void NGLScene::setWindZSlider(int wz)
{
 windZ = wz/100.0f;

 update();

}

void NGLScene::setIterationSlider(int it)
{
    cloth1.CONSTRAINT_ITERATIONS = it;
    update();
}

void NGLScene::resetCloth()
{
    cloth1 = Cloth(1.0f, 1.0f, cloth_n, cloth_m);
  gravity = 0.2;
  windX = 0.5;
  windZ = 0.5;
  update();
}

void NGLScene::backgroundColourBlue()
{
    isBlue = true;
    isBlack = false;
    isOrange = false;
    update();
}

void NGLScene::backgroundColourBlack()
{
    isBlue = false;
    isBlack = true;
    isOrange = false;
    update();
}

void NGLScene::backgroundColourOrange()
{
    isBlue = false;
    isBlack = false;
    isOrange = true;
    update();
}

void NGLScene::setBallCollision()
{
    isBall = true;
}

void NGLScene::setNoBallCollision()
{
    isBall = false;
}

void NGLScene::setGrid()
{
    isGrid = true;
}

void NGLScene::setNoGrid()
{
    isGrid = false;
}
