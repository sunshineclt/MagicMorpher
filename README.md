# MagicMorpher
- A tool for face morphing
- 良好的README阅读体验请使用[Github](https://github.com/sunshineclt/MagicMorpher)（Github上README中图片已经渲染好了）
- 算法参考[这个网址](http://www.learnopencv.com/face-morph-using-opencv-cpp-python/#download)
- 大体来说就是先做面部特征检测，之后根据检测到的特征点做三角剖分，最后将两张图的所有三角分别对应地blend画到目标图片上
- 代码注释很详细，可以直接看源代码

## 效果展示
### 原图（希拉里和特朗普）
<img src="./pic/clinton.jpg" alt="希拉里" width=200px/>
<img src="./pic/trump.jpg" alt="特朗普" width=200px/>

### 特征检测
<img src="./pic/特征检测.png" alt="特征检测" width=400px/>

### 三角剖分
<img src="./pic/三角剖分.png" alt="三角剖分" width=400px/>

### Morph结果
morph结果：
<br />
<img src="./pic/morphed_face.png" alt="morph结果" width=400px/>

没有boundary的morph效果（usage中会解释）：
<br />
<img src="./pic/no_boundary_morphed_face.png" alt="没有boundary的morph效果" width=400px/>

## Usage
`./MagicMorpher /path/shape_predictor_68_face_landmarks.dat /path/1.jpg /path/2.jpg /path/target.jpg 0.5 1 0`

- 第一个参数是面部特征检测训练好的训练集，可从[http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2](http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2)下载
- 第二个参数是要morph的第一张图片
- 第三个参数是要morph的第二张图片
- 第四个参数是要morph的目标图片
- 第五个参数是alpha是指，在0到1之间，越大表示目标图片越接近第一张图片
- 第六个参数是addBoundary，为0或1，表示是否加上边界。如果不加上的morph出来的结果只有脸，没有头发、衣服等，如果加上的话由于没有对这些特征的检测所以可能morph效果不好。（训练集目前只能够检测正脸，我也没有数据去做训练集所以暂时没有办法）
- 第七个参数是isDemoMode，为0或1，表示是否为演示模式。如果为演示模式则会展示出特征检测、三角剖分的过程，否则不会。

## Code
### 库依赖
- 依赖opencv3.2.0, dlib, libjpeg
- 目前项目中配置的依赖均为绝对路径，若要本地编译请修改header search paths, framework search paths和library search paths以适应本地的opencv, dlib, libjpeg路径

### 代码结构
- FaceLandmarkGetter类负责提取面部特征
- Trianglationer类负责从面部特征做三角剖分
- 最后在main.cpp中完成每一个三角到目标图像的映射

## 缺点
- 目前主要缺陷就是特征点不够多导致只有眉毛以下下巴以上的脸部分可以较好的morphing，其他地方没有特征检测所以morph的效果较差，希望能有人提供检测头发，头颈，衣服等特征的数据集，那样就可以Morphing得更好啦！
- 此外目前还要求两张脸必须都是正脸，最好大小也类似，这样morphing的效果会最好，这方面也可以做一些改进（或许可以自动地旋转脸、缩放脸）

