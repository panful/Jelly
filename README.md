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
- [x] Actor颜色应该使用描述符设置，不能在shader中写死
- [x] shared_from_this 修改为 weak_from_this
- [x] Object类添加 Modified Changed needUpdate 类似变量
- [ ] 窗口较小时，图元某些片段没有像素 commit de5ed317ecd33d57cc4e45c866a6a7aa108f821e 之后才出现这个问题，Viewer 尺寸固定为较大时，这种情况会很少出现
- [x] uniform 使用 CopyToDevice 设置数据时，应该只 mapMemory 一次，保证释放 uniform 之前 unMapMemory 就行
- [ ] 当相机到特殊位置后，actor 还是会被相机近平面裁剪，导致一部分看不到
