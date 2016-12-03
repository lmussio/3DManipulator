# 3DManipulator
Projeto de Computação Gráfica 6º semestre do Centro Universitário da FEI. Projeto compilado com Qt 5.7.0 (GCC 6.2.1 20160830, 64 bit) no Linux.

### Mouse 3D - Spacenav
Foi utilizada a biblioteca [spacenav](http://spacenav.sourceforge.net/) para o projeto atual.

Para o mouse 3D funcionar no Linux com a spacenav, é necessário compilar e executar o binário spacenavd:
```sh
sudo spacenavd -d -v
```
Existe um port da spacenav para Windows chamada de `Spacenav Win32`.

### Mouse 3D - GLUT
A GLUT possui 3 funções responsáveis por capturar os comandos gerados pelo mouse 3D:
- glutSpaceballButtonFunc
- glutSpaceballMotionFunc
- glutSpaceballRotateFunc

Para o mouse 3D funcionar com a GLUT, é necessário baixar o [driver](http://www.3dconnexion.com/service/drivers.html) da 3Dconnexion e rodar o binário 3dxsrv.

Exemplo de uso do 3dxsrv no Linux:
```sh
sudo /etc/3DxWare/daemon/3dxsrv -d USB
```

### Mouse 3D - 3Dconnexion
A 3Dconnexion possui um [SDK](http://www.3dconnexion.com/service/software-developer.html) contendo bibliotecas para desenvolvimento de software para seus mouses 3D.
