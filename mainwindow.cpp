#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QButtonGroup>
#include <QRadioButton>
#include <QColor>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->objSettings->hide();
    ui->cbFWaving->hide();
//    this->showFullScreen();
    this->showMaximized();
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exit()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->myGLWidget,SIGNAL(setList(std::vector<Object>)),this,SLOT(setList(std::vector<Object>)));
    obj_index = 0;
    obj_deleted = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exit()
{
    QMessageBox msgBox;
    msgBox.setText("Exit?");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes){
        this->close();
    }
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setText("The MIT License (MIT)\r\n\r\nCopyright (c) 2016 Leandro Duque Mussio\r\n\r\n"
                   "Permission is hereby granted, free of charge, to any person obtaining a copy "
                   "of this software and associated documentation files (the \"Software\"), to deal "
                   "in the Software without restriction, including without limitation the rights "
                   "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
                   "copies of the Software, and to permit persons to whom the Software is "
                   "furnished to do so, subject to the following conditions:\r\n\r\n"
                   "The above copyright notice and this permission notice shall be included in all "
                   "copies or substantial portions of the Software.\r\n\r\n"
                   "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
                   "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
                   "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
                   "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
                   "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
                   "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
                   "SOFTWARE.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::setList(std::vector<Object> obj)
{
    if(this->obj.size()>obj.size()){
        obj_deleted = true;
    }
    this->obj = obj;
    ui->objList->clear();
    if(this->obj.size()==0)return;
    int icube = 0;
    int iflag = 0;
    int isphere = 0;
    int ilight = 0;
    std::vector<Object>::iterator it;
    for(it=this->obj.begin(); it!=this->obj.end(); ++it){
        if(it->getPrimitive()==CUBE){
            ui->objList->addItem("Cube "+QString::number(icube));
            icube++;
        }else if(it->getPrimitive()==SPHERE){
            ui->objList->addItem("Sphere "+QString::number(isphere));
            isphere++;
        }else if(it->getPrimitive()==FLAG){
            ui->objList->addItem("Flag "+QString::number(iflag));
            iflag++;
        }else if(it->getPrimitive()==LIGHT){
            ui->objList->addItem("Light "+QString::number(ilight));
            ilight++;
        }
    }
    if(!obj_deleted) {
        ui->objList->item(obj_index)->setSelected(true);
        refreshObjSettings();
    }
}

void MainWindow::refreshObjSettings()
{
    switch(obj[obj_index].getPrimitive()){
    case CUBE:
        ui->rbCube->setChecked(true);
        ui->cbFWaving->hide();
        break;
    case SPHERE:
        ui->rbSphere->setChecked(true);
        ui->cbFWaving->hide();
        break;
    case LIGHT:
        ui->rbLight->setChecked(true);
        ui->cbFWaving->hide();
        break;
    case FLAG:
        ui->rbFlag->setChecked(true);
        ui->cbFWaving->show();
        break;
    }
    ui->leTexture->setText(obj[obj_index].getTexturePath());

    if(ui->myGLWidget->hasLight()) ui->cbDefaultLight->setEnabled(false);
    else ui->cbDefaultLight->setEnabled(true);
    ui->comboAnimation->setCurrentIndex(obj[obj_index].getAnimation());
    ui->cbRainbow->blockSignals(true);
    ui->cbSRandom->blockSignals(true);
    ui->cbFWaving->blockSignals(true);
    if(obj[obj_index].getRainbow()==1) ui->cbRainbow->setChecked(true);
    else ui->cbRainbow->setChecked(false);
    if(obj[obj_index].getScreenRandom()==1) ui->cbSRandom->setChecked(true);
    else ui->cbSRandom->setChecked(false);
    if(obj[obj_index].getFlagWaving()==1) ui->cbFWaving->setChecked(true);
    else ui->cbFWaving->setChecked(false);
    ui->cbRainbow->blockSignals(false);
    ui->cbSRandom->blockSignals(false);
    ui->cbFWaving->blockSignals(false);

    float r,g,b;
    obj[obj_index].getColorRGB(r,g,b);
    ui->sbRed->blockSignals(true);
    ui->sbGreen->blockSignals(true);
    ui->sbBlue->blockSignals(true);
    ui->sbRed->setValue((int)(r*255));
    ui->sbGreen->setValue((int)(g*255));
    ui->sbBlue->setValue((int)(b*255));
    ui->sbRed->blockSignals(false);
    ui->sbGreen->blockSignals(false);
    ui->sbBlue->blockSignals(false);

    float sx,sy,sz;
    obj[obj_index].getScale(sx,sy,sz);
    ui->sbSX->blockSignals(true);
    ui->sbSY->blockSignals(true);
    ui->sbSZ->blockSignals(true);
    ui->sbSX->setValue(sx);
    ui->sbSY->setValue(sy);
    ui->sbSZ->setValue(sz);
    ui->sbSX->blockSignals(false);
    ui->sbSY->blockSignals(false);
    ui->sbSZ->blockSignals(false);

    float px,py,pz;
    obj[obj_index].getPos(px,py,pz);
    ui->sbX->blockSignals(true);
    ui->sbY->blockSignals(true);
    ui->sbZ->blockSignals(true);
    ui->sbX->setValue(px);
    ui->sbY->setValue(py);
    ui->sbZ->setValue(pz);
    ui->sbX->blockSignals(false);
    ui->sbY->blockSignals(false);
    ui->sbZ->blockSignals(false);

    float rx,ry,rz;
    obj[obj_index].getRot(rx,ry,rz);
    ui->sbRX->blockSignals(true);
    ui->sbRY->blockSignals(true);
    ui->sbRZ->blockSignals(true);
    ui->sbRX->setValue(rx);
    ui->sbRY->setValue(ry);
    ui->sbRZ->setValue(rz);
    ui->sbRX->blockSignals(false);
    ui->sbRY->blockSignals(false);
    ui->sbRZ->blockSignals(false);

}

void MainWindow::on_cbDefaultLight_stateChanged(int arg1)
{
    if(arg1==0) ui->myGLWidget->default_light = false;
    else ui->myGLWidget->default_light = true;
}

void MainWindow::on_objList_clicked(const QModelIndex &index)
{
    obj_deleted = false;
    obj_index = index.row();
    ui->myGLWidget->setObject(obj_index);
    ui->objSettings->show();
    refreshObjSettings();
}

void MainWindow::on_objList_doubleClicked(const QModelIndex &index)
{
    ui->myGLWidget->setObjectCam();
}

void MainWindow::on_rbCube_clicked()
{
    ui->myGLWidget->setObjectGeometry(CUBE);
}

void MainWindow::on_rbSphere_clicked()
{
    ui->myGLWidget->setObjectGeometry(SPHERE);
}

void MainWindow::on_rbLight_clicked()
{
    int ret = ui->myGLWidget->setObjectGeometry(LIGHT);
    if(ret==-1){
        switch(obj[obj_index].getPrimitive()){
        case CUBE:
            ui->rbCube->click();
            break;
        case SPHERE:
            ui->rbSphere->click();
            break;
        case FLAG:
            ui->rbFlag->click();
            break;
        }
        QMessageBox msgBox;
        msgBox.setText("Maximum light limit reached.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_rbFlag_clicked()
{
    ui->myGLWidget->setObjectGeometry(FLAG);
}


void MainWindow::on_btnSelect_clicked()
{
    ui->myGLWidget->setObjectCam();
}

void MainWindow::on_btnAdd_clicked()
{
    ui->myGLWidget->addObject();
}

void MainWindow::on_btnDelete_clicked()
{
    obj_index = 0;
    if(ui->myGLWidget->delObject()){
        obj_deleted = true;
        ui->objSettings->hide();
        setList(ui->myGLWidget->getObjects());
        refreshObjSettings();
    }
}

void MainWindow::on_comboAnimation_currentIndexChanged(int index)
{
    ui->myGLWidget->setObjectAnimation(index);
}


void MainWindow::on_cbFWaving_stateChanged(int arg1)
{
    if(arg1==0){
        ui->myGLWidget->resetObjectFlagWaving();
    }
    else{
        ui->myGLWidget->setObjectFlagWaving();
    }
}

void MainWindow::on_cbSRandom_stateChanged(int arg1)
{
    if(arg1==0){
        ui->myGLWidget->resetObjectScreenRandom();
    }
    else{
        ui->myGLWidget->setObjectScreenRandom();
    }
}

void MainWindow::on_cbRainbow_stateChanged(int arg1)
{
    if(arg1==0){
        ui->myGLWidget->resetObjectRainbow();
    }
    else{
        ui->myGLWidget->setObjectRainbow();
    }
}

void MainWindow::on_sbRed_valueChanged(int arg1)
{
    ui->myGLWidget->setObjectColorRGB((float)arg1/255.0,(float)ui->sbGreen->value()/255.0,(float)ui->sbBlue->value()/255.0);
}

void MainWindow::on_sbGreen_valueChanged(int arg1)
{
    ui->myGLWidget->setObjectColorRGB((float)ui->sbRed->value()/255.0,(float)arg1/255.0,(float)ui->sbBlue->value()/255.0);
}

void MainWindow::on_sbBlue_valueChanged(int arg1)
{
    ui->myGLWidget->setObjectColorRGB((float)ui->sbRed->value()/255.0,(float)ui->sbGreen->value()/255.0,(float)arg1/255.0);
}

void MainWindow::on_sbSX_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectScale(arg1,ui->sbSY->value(),ui->sbSZ->value());
}

void MainWindow::on_sbSY_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectScale(ui->sbSX->value(),arg1,ui->sbSZ->value());
}

void MainWindow::on_sbSZ_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectScale(ui->sbSX->value(),ui->sbSY->value(),arg1);
}

void MainWindow::on_sbX_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectX(arg1);
}

void MainWindow::on_sbY_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectY(arg1);
}

void MainWindow::on_sbZ_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectZ(arg1);
}

void MainWindow::on_sbRX_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectRotX(arg1);
}

void MainWindow::on_sbRY_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectRotY(arg1);
}

void MainWindow::on_sbRZ_valueChanged(double arg1)
{
    ui->myGLWidget->setObjectRotZ(arg1);
}

void MainWindow::on_pbSearchTexture_clicked()
{
    QString image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), obj[obj_index].getTexturePath(), tr("Image Files (*.png *.jpg *.bmp)"),0,QFileDialog::DontUseNativeDialog);
    if(image_path!=""){
        ui->leTexture->setText(image_path);
        ui->myGLWidget->setObjectTexture(image_path);
    }
}

void MainWindow::on_pbTextureClear_clicked()
{
    ui->leTexture->setText("");
    ui->myGLWidget->clearObjectTexture();
}
