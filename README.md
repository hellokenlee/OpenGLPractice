### My Learning Experiences of Morden OpenGL ###

_ _ _

Some results of each sections(updating...):

---  

26 SSAO:  

| WITH SSAO | WITHOUT SSAO |
| --- | --- |
| ![](textures/demo/26-6.png)  | ![](textures/demo/26-7.png)  |


| Not Random Rotation | Not Range Check | Not Blur | Full SSAO | With Diffuse |
| --- | --- | --- | --- | --- |
| ![](textures/demo/26-3.png)  | ![](textures/demo/26-4.png)  | ![](textures/demo/26-2.png)  | ![](textures/demo/26-5.png)  | ![](textures/demo/26-1.png)  |

_ _ _

25 Deferred Shading:  

![](textures/demo/25-1.png)

G-Buffer Viewing

| ![](textures/demo/25-Pos.png)  | ![](textures/demo/25-Nor.png) |
| --- | --- |
|  ![](textures/demo/25-Dif.png)  |  ![](textures/demo/25-Spec.png) |

In my nVidia GT650m with 128 point lights in the scene.

Models were drawed from back to front.

Forward shading is not stable, since fragment shaders were call differ from view position.

| Deferred Shaidng | Forward Shading |
| --- | --- |
|  stable at 19ms/frame  |  27 ~ 37ms/frame |
_ _ _

24 PBR:  


![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/24-1.png)  
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/24-2.png)  
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/24-3.png)  
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/24-4.png)  
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/24-5.png)  

_ _ _

23 Bloom:  


| Without Bloom | With Bloom |
| --- | --- |
| ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/23-1.png)   |![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/23-2.png)   | 

_ _ _

22 HDR:  


| Without HDR | With HDR |
| --- | --- |
| ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/22-1.png)   |![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/22-2.png)   | 

_ _ _


21 Parallax Mapping

| Only Diffuse | Diffuse + Normal Mapping | Diffuse + Normal + Parallax Mapping |
| --- | --- | --- |
| ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/21-1.png)   | ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/21-2.png)   | ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/21-3.png)  
| ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/21-4.png)   | ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/21-5.png)   | ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/21-6.png)  

_ _ _

20 Omnidirectional Shadow Maps



Shadow from a point light at (0, 0, 0): 
![](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/20-1.png)

_ _ _

19 Shadow Mapping:  


Shadow and lighting with point light:
![](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/19-1.png)

Shadow and lighting with directional light: 
![](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/19-2.png)
_ _ _

18 Normal Mapping:  


| Without Normal Mapping | With Normal Mapping |
| --- | --- |
| ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/18-NNM.png)   |![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/18-NM.png)   | 

_ _ _


17 Gamma Correction:  


| Linear Attenuation, No Gamma Correction | Quadratic Attenuation, No Gamma Correction | Linear Attenuation, Gamma Correction | Quadratic Attenuation, Gamma Correction|
| --- | --- | --- | --- |
| ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/17-LNG.png)   |![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/17-QNG.png)   | ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/17-LG.png)   | ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/17-QG.png)   |

_ _ _

16 Advanced Lighting:

Pure Phong Shading:  
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/16-1.png)  
Blinn-Phong Shading:  
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/16-2.png)  

_ _ _

Whims: Animation《Magic of Stella》's ending scene.

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/w02.png)
_ _ _

15 GeometryShader:

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/15-1.png)
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/15-2.gif)

_ _ _

14 CubeMaps:

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/14-1.png)
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/14-2.png)
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/14-3.png)
![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/14-4.png)
_ _ _
  
Whims: A Mirror! 

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/w01.png)
_ _ _

13 Model Loading:

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/13-1.png)

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/13-2.png)
_ _ _

12 MultipleLights:

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/12.png)
_ _ _

11 LightCaster(Different kinds of light):

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/11-1.png)

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/11-2.png)

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/11-3.png)
_ _ _
10 LightMaps:

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/10.png)
_ _ _
09 Materials:

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/09.png)
_ _ _
08 BasicLighting

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/08.png)
_ _ _
07 Colors

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/07.png)
_ _ _
06 Camera

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/06.gif)
_ _ _
05 CoordinateSystem

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/05.gif)
_ _ _
04 Transformations

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/04.gif)
_ _ _
03 Textures

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/03.png)
_ _ _
02 Shaders

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/02.gif)
_ _ _
01 HelloTriangle

![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/01.png)
_ _ _
