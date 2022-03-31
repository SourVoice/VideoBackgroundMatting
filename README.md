# 简易视频背景处理功能

实现了部分图像处理功能和视频内人物抠除背景功能

<!-- PROJECT SHIELDS -->

<!-- [![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url] -->

<!-- PROJECT LOGO -->
<br />

<p align="center">
  <a href="https://github.com/SourVoice/VideoBackgroundMatting/">
    <img src="/images/logo.png" alt="Logo" width="72" height="60">
  </a>

  <h3 align="center">视频人像抠图应用</h3>
    <br />
    <a href="https://github.com/SourVoice/VideoBackgroundMatting">查看Demo</a>
    ·
    <a href="https://github.com/SourVoice/VideoBackgroundMatting/issues">报告Bug</a>
    ·
    <a href="https://github.com/SourVoice/VideoBackgroundMatting/issues">提出新特性</a>
  </p>

</p>

## 目录

- [简易视频背景处理功能](#简易视频背景处理功能)
  - [目录](#目录)
    - [上手指南](#上手指南)
      - [功能实现](#功能实现)
      - [**安装步骤**](#安装步骤)
    - [开发的架构](#开发的架构)
    - [使用到的框架和第三方库](#使用到的框架和第三方库)
    - [贡献者](#贡献者)
      - [如何参与开源项目](#如何参与开源项目)
    - [版本控制](#版本控制)
    - [作者](#作者)
    - [版权说明](#版权说明)
    - [鸣谢](#鸣谢)

### 上手指南
------
#### 功能实现

1.图像实现了 RGB,对比度等调整

"工具栏"->"文件"->"打开图片":进行编辑

- 编辑图像

  <center class="half">
  <img src="/images/ImageProcess.png" width=600/>
  </center>
  
2.视频人物扣取

- 打开视频

  <center class="half">
  <img src="/images/VideoPlayWindow.png" width=600/>
  </center>
  
- 剪切视频内人物对象

   "工具栏"->"文件"->"打开视频"->播放页右上方"剪切对象":等待处理

  <center class="half">
  <img src="/images/VideoBGMattingWindow.png" width=600/>
  </center>

3.视频结果截图
  <center class="half">
      输入
      <img src="/images/Input.png" width=400/>
      输出
      <img src="/images/Output.png" width=400/>
  </center>

4.视频处理性能指标

CPU：i5-7300HQ  4核2.5GHz  GPU: 集成显卡Intel(R) HD Graphics 630

主线程编码

| 编码工具库    | 编码设备  | 视频分辨率 | CPU占用 | GPU占用 | 内存占用 | 编码平均耗时（ms/frame） |
| ------------- | --------- | ---------- | ------- | ------- | -------- | ------------------------ |
| opencv(MPEG4) | cpu       | 1440*1080  | 50%     | <1%     | 127MB    | 11.28                    |
| ffmpeg(H264)  | cpu       | 1440*1080  | 90%     | <1%     | 310MB    | 19.47                    |
| ffmpeg(H264)  | intel gpu | 1440*1080  | 71%     | 38.9%   | 188MB    | 6.28                     |



opencv解码 + 网络处理 + 解码  单线程 22.2.3

| 编码工具库   | 编码设备  | 视频分辨率 | CPU占用 | GPU占用 | 编码平均耗时（ms/frame） |
| ------------ | --------- | ---------- | ------- | ------- | ------------------------ |
| opencv       | cpu       | 1440*1080  | 82%     | 32.8%   | 58.51 - > 56.17          |
| ffmpeg(H264) | intel gpu | 1440*1080  | 96%     | 37.8%   | 50.60 -> 47.94           |


#### **安装步骤**
-------
1.Clone the repo

```sh
git clone https://github.com/SourVoice/VideoBackgroundMatting.git
```

2.使用 VS2019/2017 打开.sln 文件

3.为项目配置所需环境

[需要在项目的属性配置以下第三方库](#使用到的框架和第三方库)

### 开发的架构
-----
使用 Visual Studio 2019 + QtCreator 进行开发

### 使用到的框架和第三方库

[Qt5.11.3] GUI 框架

[ncnn-20210322] 高性能神经网络前向计算框架

[VulkanSDK] ncnn 依赖

[Opencv4.0.1] ncnn 依赖

[ffmpeg] 视频解码依赖

### 贡献者

请阅读**CONTRIBUTING.md** 查阅为该项目做出贡献的开发者。

#### 如何参与开源项目

贡献使开源社区成为一个学习、激励和创造的绝佳场所。你所作的任何贡献都是**非常感谢**的。

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### 版本控制

该项目使用 Git 进行版本管理。您可以在 repository 参看当前可用版本和项目进度

### 作者

@SourVoice
@shaoeric
@luoshiyong

_您也可以在贡献者名单中参看所有参与该项目的开发者。_

### 版权说明

该项目签署了 MIT 授权许可，详情请参阅 [LICENSE.txt](https://github.com/SourVoice/VideoBackgroundMatting/blob/master/LICENSE.txt)

### 鸣谢

- [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
- [Img Shields](https://shields.io)
- [Choose an Open Source License](https://choosealicense.com)
- [GitHub Pages](https://pages.github.com)
- [Animate.css](https://daneden.github.io/animate.css)

<!-- links-depentent -->

[qt5.11.3]: https://download.qt.io/new_archive/qt/5.11/
[ncnn-20210322]: https://github.com/Tencent/ncnn/releases/tag/20210322
[vulkansdk]: https://vulkan.lunarg.com/sdk/home#windows
[opencv4.0.1]: https://opencv.org/releases
[ffmpeg]: https://github.com/BtbN/FFmpeg-Builds

<!-- links -->

[your-project-path]: https://github.com/SourVoice/VideoBackgroundMatting
[contributors-shield]: https://img.shields.io/github/contributors/SourVoice/VideoBackgroundMatting.svg?style=flat-square
[contributors-url]: https://github.com/SourVoice/VideoBackgroundMatting/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/SourVoice/VideoBackgroundMatting.svg?style=flat-square
[forks-url]: https://github.com/SourVoice/VideoBackgroundMatting/network/members
[stars-shield]: https://img.shields.io/github/stars/SourVoice/VideoBackgroundMatting.svg?style=flat-square
[stars-url]: https://github.com/SourVoice/VideoBackgroundMatting/stargazers
[issues-shield]: https://img.shields.io/github/issues/SourVoice/VideoBackgroundMatting.svg?style=flat-square
[issues-url]: https://img.shields.io/github/issues/SourVoice/VideoBackgroundMatting.svg
[license-shield]: https://img.shields.io/github/license/SourVoice/VideoBackgroundMatting.svg?style=flat-square
[license-url]: https://github.com/SourVoice/VideoBackgroundMatting/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/shaojintian
