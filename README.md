# 介绍
自己写着玩的可视化工具包。

# TODO
- [x] 组件
- [ ] 查找依赖
- [ ] option USE_GLFW_WINDOW
- [ ] 可配置的验证层
- [ ] 可配置的设备扩展
- [ ] Renderer 放在 UI 还是 Core ？
- [ ] Rendering目录新增Vulkan，和Vulkan相关的都放在该目录下，Core不依赖Vulkan任何东西（可以扩展OpenGL等）
- [ ] 隐藏平台相关头文件，自动根据不同平台创建不同类型的窗口
- [ ] Pipeline 应该放在 Device or Window
- [ ] Window 中的 Device 调用 Render() 时初始化
- [ ] Renderer 是否应该持有 Window 的 weak_ptr，考虑将同一个 Renderer 添加到不同的 Window
- [ ] PipelineCache 的 pipeline 改为 unique_ptr
- [x] 使用UniformColor着色时，设置了顶点颜色，顶点着色器警告：inColor没有使用
- [ ] 添加ShaderGeneratorCache，着色器代码相同时不用重复生成SPV
- [ ] 共享vk::Buffer，而不仅仅是共享 DataArray
- [ ] Actor颜色应该使用描述符设置，不能在shader中写死
