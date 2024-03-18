#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
                                          QMainWindow(parent),
                                          m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  m_scene = new NGLScene(this);
  m_ui->m_mainWindowGridLayout->addWidget(m_scene,0,1,6,3);

  //to choose colour palettes
 connect(m_ui->resetButton,SIGNAL(clicked(bool)),this,SLOT(resetCloth()));
 connect(m_ui->patternBox, SIGNAL(clicked()), m_scene, SLOT(setPatternTrue()));
 connect(m_ui->pointsBox, SIGNAL(clicked()), m_scene, SLOT(setPoints()));
 connect(m_ui->filledBox, SIGNAL(clicked()), m_scene, SLOT(setFilled()));
 connect(m_ui->blueButton, SIGNAL(clicked()), m_scene, SLOT(backgroundColourBlue()));
 connect(m_ui->blackButton, SIGNAL(clicked()), m_scene, SLOT(backgroundColourBlack()));
 connect(m_ui->orangeButton, SIGNAL(clicked()), m_scene, SLOT(backgroundColourOrange()));
 connect(m_ui->ballButton, SIGNAL(clicked()), m_scene, SLOT(setBallCollision()));
 connect(m_ui->noBallButton, SIGNAL(clicked()), m_scene, SLOT(setNoBallCollision()));
 connect(m_ui->gridButton, SIGNAL(clicked()), m_scene, SLOT(setGrid()));
 connect(m_ui->noGridButton, SIGNAL(clicked()), m_scene, SLOT(setNoGrid()));
 connect(m_ui->gravitySlider,SIGNAL(valueChanged(int)), m_scene, SLOT(setGravitySlider(int)));
 connect(m_ui->windXSlider,SIGNAL(valueChanged(int)), m_scene, SLOT(setWindXSlider(int)));
 connect(m_ui->windZSlider,SIGNAL(valueChanged(int)), m_scene, SLOT(setWindZSlider(int)));
 connect(m_ui->iterationSlider,SIGNAL(valueChanged(int)), m_scene, SLOT(setIterationSlider(int)));

}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_scene;
}


void MainWindow::resetCloth()
{
    m_scene->resetCloth();
    m_ui->gravitySlider->setValue(30);
    m_ui->windXSlider->setValue(45);
    m_ui->windZSlider->setValue(45);
    m_ui->iterationSlider->setValue(15);

}

void MainWindow::keyPressEvent(QKeyEvent *_event)
{
    //key press events are happening in our NGLScene
    m_scene->keyPressEvent(_event);
}

