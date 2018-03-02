### My Learning Experiences of Morden OpenGL ###

_ _ _

Some results of each sections(updating...):

---  

26 SSAO:  

| WITH SSAO | WITHOUT SSAO |
| --- | --- |
| ![](Resources/Textures/demo/26-6.png)  | ![](Resources/Textures/demo/26-7.png)  |


| Not Random Rotation | Not Range Check | Not Blur | Full SSAO | With Diffuse |
| --- | --- | --- | --- | --- |
| ![](Resources/Textures/demo/26-3.png)  | ![](Resources/Textures/demo/26-4.png)  | ![](Resources/Textures/demo/26-2.png)  | ![](Resources/Textures/demo/26-5.png)  | ![](Resources/Textures/demo/26-1.png)  |

_ _ _

25 Deferred Shading:  

![](Resources/Textures/demo/25-1.png)

G-Buffer Viewing

| ![](Resources/Textures/demo/25-Pos.png)  | ![](Resources/Textures/demo/25-Nor.png) |
| --- | --- |
|  ![](Resources/Textures/demo/25-Dif.png)  |  ![](Resources/Textures/demo/25-Spec.png) |

In my nVidia GT650m with 128 point lights in the scene.

Models were drawed from back to front.

Forward shading is not stable, since fragment shaders were call differ from view position.

| Deferred Shaidng | Forward Shading |
| --- | --- |
|  stable at 19ms/frame  |  27 ~ 37ms/frame |
_ _ _

24 PBR:  


![image](Resources/Textures/demo/24-1.png)  
![image](Resources/Textures/demo/24-2.png)  
![image](Resources/Textures/demo/24-3.png)  
![image](Resources/Textures/demo/24-4.png)  
![image](Resources/Textures/demo/24-5.png)  

_ _ _

23 Bloom:  


| Without Bloom | With Bloom |
| --- | --- |
| ![image](Resources/Textures/demo/23-1.png)   |![image](Resources/Textures/demo/23-2.png)   | 

_ _ _

22 HDR:  


| Without HDR | With HDR |
| --- | --- |
| ![image](Resources/Textures/demo/22-1.png)   |![image](Resources/Textures/demo/22-2.png)   | 

_ _ _


21 Parallax Mapping

| Only Diffuse | Diffuse + Normal Mapping | Diffuse + Normal + Parallax Mapping |
| --- | --- | --- |
| ![image](Resources/Textures/demo/21-1.png)   | ![image](Resources/Textures/demo/21-2.png)   | ![image](Resources/Textures/demo/21-3.png)  
| ![image](Resources/Textures/demo/21-4.png)   | ![image](Resources/Textures/demo/21-5.png)   | ![image](Resources/Textures/demo/21-6.png)  

_ _ _

20 Omnidirectional Shadow Maps



Shadow from a point light at (0, 0, 0): 
![](Resources/Textures/demo/20-1.png)

_ _ _

19 Shadow Mapping:  


Shadow and lighting with point light:
![](Resources/Textures/demo/19-1.png)

Shadow and lighting with directional light: 
![](Resources/Textures/demo/19-2.png)
_ _ _

18 Normal Mapping:  


| Without Normal Mapping | With Normal Mapping |
| --- | --- |
| ![image](Resources/Textures/demo/18-NNM.png)   |![image](Resources/Textures/demo/18-NM.png)   | 

_ _ _


17 Gamma Correction:  


| Linear Attenuation, No Gamma Correction | Quadratic Attenuation, No Gamma Correction | Linear Attenuation, Gamma Correction | Quadratic Attenuation, Gamma Correction|
| --- | --- | --- | --- |
| ![image](Resources/Textures/demo/17-LNG.png)   |![image](Resources/Textures/demo/17-QNG.png)   | ![image](Resources/Textures/demo/17-LG.png)   | ![image](Resources/Textures/demo/17-QG.png)   |

_ _ _

16 Advanced Lighting:

Pure Phong Shading:  
![image](Resources/Textures/demo/16-1.png)  
Blinn-Phong Shading:  
![image](Resources/Textures/demo/16-2.png)  

_ _ _

Whims: Animation《Magic of Stella》's ending scene.

![image](Resources/Textures/demo/w02.png)
_ _ _

15 GeometryShader:

![image](Resources/Textures/demo/15-1.png)
![image](Resources/Textures/demo/15-2.gif)

_ _ _

14 CubeMaps:

![image](Resources/Textures/demo/14-1.png)
![image](Resources/Textures/demo/14-2.png)
![image](Resources/Textures/demo/14-3.png)
![image](Resources/Textures/demo/14-4.png)
_ _ _
  
Whims: A Mirror! 

![image](Resources/Textures/demo/w01.png)
_ _ _

13 Model Loading:

![image](Resources/Textures/demo/13-1.png)

![image](Resources/Textures/demo/13-2.png)
_ _ _

12 MultipleLights:

![image](Resources/Textures/demo/12.png)
_ _ _

11 LightCaster(Different kinds of light):

![image](Resources/Textures/demo/11-1.png)

![image](Resources/Textures/demo/11-2.png)

![image](Resources/Textures/demo/11-3.png)
_ _ _
10 LightMaps:

![image](Resources/Textures/demo/10.png)
_ _ _
09 Materials:

![image](Resources/Textures/demo/09.png)
_ _ _
08 BasicLighting

![image](Resources/Textures/demo/08.png)
_ _ _
07 Colors

![image](Resources/Textures/demo/07.png)
_ _ _
06 Camera

![image](Resources/Textures/demo/06.gif)
_ _ _
05 CoordinateSystem

![image](Resources/Textures/demo/05.gif)
_ _ _
04 Transformations

![image](Resources/Textures/demo/04.gif)
_ _ _
03 Textures

![image](Resources/Textures/demo/03.png)
_ _ _
02 Shaders

![image](Resources/Textures/demo/02.gif)
_ _ _
01 HelloTriangle

![image](Resources/Textures/demo/01.png)
_ _ _
