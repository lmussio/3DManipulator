#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "object.h"
#include <vector>
#include <QListWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<Object> obj;
    int obj_index;
    bool obj_deleted;
public slots:
    void exit();
    void about();
    void setList(std::vector<Object>);
    void refreshObjSettings();
private slots:
    void on_objList_clicked(const QModelIndex &obj_index);
    void on_rbCube_clicked();
    void on_rbSphere_clicked();
    void on_rbFlag_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_comboAnimation_currentIndexChanged(int index);
    void on_cbFWaving_stateChanged(int arg1);
    void on_cbSRandom_stateChanged(int arg1);
    void on_cbRainbow_stateChanged(int arg1);
    void on_sbGreen_valueChanged(int arg1);
    void on_sbRed_valueChanged(int arg1);
    void on_sbBlue_valueChanged(int arg1);
    void on_sbSX_valueChanged(double arg1);
    void on_sbSY_valueChanged(double arg1);
    void on_sbSZ_valueChanged(double arg1);
    void on_rbLight_clicked();
    void on_cbDefaultLight_stateChanged(int arg1);
    void on_sbX_valueChanged(double arg1);
    void on_sbY_valueChanged(double arg1);
    void on_sbZ_valueChanged(double arg1);
    void on_sbRX_valueChanged(double arg1);
    void on_sbRY_valueChanged(double arg1);
    void on_sbRZ_valueChanged(double arg1);
    void on_objList_doubleClicked(const QModelIndex &index);
    void on_btnSelect_clicked();
    void on_pbSearchTexture_clicked();
    void on_pbTextureClear_clicked();
};

#endif // MAINWINDOW_H
